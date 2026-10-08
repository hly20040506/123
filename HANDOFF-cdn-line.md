# 交接文档:在代理服务器上加一条 Cloudflare CDN 备用线路

> 给另一个有 SSH 权限的环境(Claude Code 或人工)使用。
> **本文档不含任何密码、Token、UUID。登录凭据由用户在新环境里直接提供,不要写进任何文件或提交到仓库。**

## 1. 目标
在 `186.244.241.32` 上,为现有的 VLESS+Reality 和 Hysteria2 之外,增加一条备用线路:
**VLESS + WebSocket + TLS,经 Cloudflare CDN(橙云),域名 `cdn.hanly.cloud`,端口 8443**。
客户端是 FlClash(mihomo 内核)。

## 2. 已确认的现状
- 系统 Ubuntu 22.04,systemd,KVM。
- 已在运行:`xray.service`、`hysteria-server.service`。**没有** Caddy / Nginx / 面板(x-ui 等)。
- IP-Sentinel(Master + Agent)已装好,Agent 的 Webhook 端口是 **16097**,**不要占用、不要改动** `/opt/ip_sentinel`。
- Reality 大概率占 443。本方案用 **8443**(Cloudflare 橙云支持的端口之一)。
- Cloudflare 里 `hanly.cloud` 已是 Active。

## 3. 用户需在 Cloudflare 网页完成(新环境无法代劳,先确认已做)
1. DNS:`A` 记录,名称 `cdn`,内容 `186.244.241.32`,**橙云开启**;删掉指向 `47.93.140.93` 的旧记录。
2. SSL/TLS 加密模式:**Full (strict)**。
3. SSL/TLS → Origin Server 生成证书,主机名 `cdn.hanly.cloud`,保存到服务器:
   - 证书 `/etc/ssl/cf-origin.pem`
   - 私钥 `/etc/ssl/cf-origin.key`

## 4. 服务器端步骤(逐步执行,每步检查输出)

### 4.1 只读侦察
```bash
systemctl cat xray | grep -E "ExecStart|^User="
ss -tlnp | grep -E "xray|hysteria"; ss -ulnp | grep hysteria
ls -l /etc/ssl/cf-origin.pem /etc/ssl/cf-origin.key
```
确认:Xray 配置文件路径(通常 `/usr/local/etc/xray/config.json`)、Xray 运行用户、**8443 未被占用**、两个证书文件存在。

### 4.2 让 Xray 用户能读私钥
若 Xray 以 `nobody` 运行(看 4.1 的 `User=`),私钥需对其可读:
```bash
chgrp nogroup /etc/ssl/cf-origin.key && chmod 640 /etc/ssl/cf-origin.key
```
(若用户是别的,改成对应的组。不要把私钥设成所有人可读。)

### 4.3 备份并追加入站
```bash
CONF=/usr/local/etc/xray/config.json     # 以 4.1 为准
cp -a "$CONF" "$CONF.bak.$(date +%s)"
UUID=$(xray uuid)
WSPATH="/$(openssl rand -hex 8)"
jq --arg id "$UUID" --arg p "$WSPATH" '.inbounds += [{
  "tag":"vless-ws-cdn","listen":"0.0.0.0","port":8443,"protocol":"vless",
  "settings":{"clients":[{"id":$id}],"decryption":"none"},
  "streamSettings":{"network":"ws","security":"tls",
    "tlsSettings":{"certificates":[{"certificateFile":"/etc/ssl/cf-origin.pem","keyFile":"/etc/ssl/cf-origin.key"}]},
    "wsSettings":{"path":$p}}
}]' "$CONF" > /tmp/xray.new && xray run -test -c /tmp/xray.new && cp /tmp/xray.new "$CONF"
```
- `xray run -test` 失败则**不要覆盖**,保留原配置。
- 现有配置若不是 `inbounds` 数组结构,先 `jq . "$CONF"` 查看再调整。

### 4.4 防火墙放行 TCP 8443
按实际用的:`ufw allow 8443/tcp` 或 `firewall-cmd --permanent --add-port=8443/tcp && firewall-cmd --reload` 或 iptables。
注意 IP-Sentinel 安装器可能已管理过规则,不要清空现有规则。

### 4.5 重启并验证
```bash
systemctl restart xray && sleep 1 && systemctl is-active xray
ss -tlnp | grep 8443
curl -sk -m 8 -o /dev/null -w "%{http_code}\n" https://127.0.0.1:8443/   # 非 WS 请求,返回 400/404 之类即说明 TLS 在工作
curl -s -m 8 -o /dev/null -w "%{http_code}\n" https://cdn.hanly.cloud:8443/  # 经 Cloudflare
```
Reality 和 Hysteria2 不应受影响:确认 `systemctl is-active hysteria-server`,并用户端实测原线路。
**回滚**:`cp "$CONF.bak.<时间戳>" "$CONF" && systemctl restart xray`。

### 4.6 把客户端配置交给用户
只在最终回复里**一次性**输出下面内容(UUID 与路径用 4.3 的变量),不要写入文件:
```yaml
- name: cdn-ws
  type: vless
  server: cdn.hanly.cloud
  port: 8443
  uuid: <UUID>
  udp: true
  tls: true
  servername: cdn.hanly.cloud
  network: ws
  client-fingerprint: chrome
  ws-opts:
    path: <WSPATH>
    headers:
      Host: cdn.hanly.cloud
```
并建议用户在 FlClash 里把 Reality、Hysteria2、cdn-ws 放进同一个 `fallback` 或 `url-test` 代理组。

## 5. 安全要求(必须遵守)
- 不要读取、打印或外传私钥内容、Bot Token、`/opt/ip_sentinel` 下的配置。
- 不要修改或停止 Reality、Hysteria2、IP-Sentinel。只增加一个入站。
- 做完提醒用户:**修改 root 密码**(曾在聊天里出现过),并 **用 @BotFather `/revoke` 更换 Telegram Bot Token**(也曾泄露),再用 Bot 菜单的"全舰队切换 Bot 凭证"同步。
- 建议改为 SSH 密钥登录并禁用密码登录。

## 6. 已知风险(告知用户)
- `hanly.cloud` 在阿里云且已备案,NS 改到 Cloudflare 并解析到海外代理可能有合规风险;更稳妥的是另买未备案的海外域名,届时把上文 `cdn.hanly.cloud` 与证书主机名一起替换即可。
- 大流量长期走 Cloudflare 免费版有被限制的可能,本线路定位为**备用**。
