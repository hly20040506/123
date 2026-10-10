import json, re, glob
W = '/home/user/123/work2'
L = json.load(open(f'{W}/list.json'))
bad = {}
for f in sorted(glob.glob(f'{W}/results/*.json')):
    d = json.load(open(f)); i = d['idx']; x = L[i - 1]
    n = x['pdf'].rsplit('/', 1)[1][:-4]
    t = open(f"{W}/txt/{x['id']}_{n}.txt", errors='ignore').read()
    tn = re.sub(r'\s+', '', t)
    miss = []
    for m in re.findall(r'\d+\.\d+', d['metrics']):
        if m in t or m in tn: continue
        # allow leading-dot variants like .9330 written as 0.9330 and vice versa
        alt = m[1:] if m.startswith('0.') else None
        if alt and alt in t: continue
        miss.append(m)
    if miss: bad[i] = miss
for i, m in bad.items(): print(i, m)
print('papers with untraced decimals:', len(bad))
