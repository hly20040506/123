#include <stdio.h>
#include <graphics.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

// 账号和密码最大长度
#define MAX_LEN 20

// 账号和密码的文件名
const char* FILE_NAME = "account.txt";

// 定义账号结构体
typedef struct {
    char username[MAX_LEN + 1];
    char password[MAX_LEN + 1];
} Account;

// 定义链表结构体
struct Node {
    int data;
    struct Node* next;
};

#undef UNICODE
#undef _UNICODE

#define FILENAME "1.bin"
#define MEAN 16.8 // 总奖励值期望
#define VARIANCE 158.7 // 总奖励值方差
#define TEXT_LEN 50


IMAGE bk, pt, xy, ss, cs, kp,dr,end;
#pragma comment(lib,"Winmm.lib")
// 卡牌数据结构
typedef struct card 
{
    int id;
    int rarity;  // 卡牌稀有度
    int gong;
    int xue;
    int fei;
    struct card* next;  // 下一张卡牌
} Card;


typedef struct 
{
    int gong;
    int xue;
    int xiaoguo;
}Enemy;

int n=0;
char text[TEXT_LEN];
int i, j;//循环变量
int count = 0;//计步数
int mode;//选择模式
int temp;//交换二维数组数值的变量
int arr[5][5];//定义一个二维数组
int mousex, mousey;
int lastx, lasty;
bool isDragging = false;

void drawButton(int count);
int randnum(int r);
void bxyao();
void updateFrame(int x, int y, double w, double h);
void loadmage();
void drawCardImage(Card* p);
void drawCardAttributes(Card* p);
void drawCardUI(Card* p);
Card* drawCard(Card* head, int* count, int cards[]);
Card* createCardListHead();
void addCard(Card* head, Card* card);
void NewCard(Card* card, int id);
Card* initCardList(int count);
void freeCardList(Card* head);
void resetCardList(Card* head, int count);
void showCards(int cards[]);
void saveCards(Card* p);
double normal_cdf(double x);
double gailv(int user_reward);
struct card* maopao(struct card* head);
void drawMainMenu();
void myenemy();
int startfight(int a1, int a2, int a3, int b1, int b2, int b3);
int fight(int guan);
int choose();
void putpicture(int dstx, int dsty, IMAGE* img, COLORREF color);
void loadbk();
int enemy(int guan, Enemy di[3]);
Card* loadCards();
void Showguan(int guan);
void fightui();
void initGame();
void mainMenu();
void game();
void chooseMode();
void createMap();
void startGame();
void direction(int x);
void contralGame();
void createCube();
void rebuild();
void judge();
void printNumber(int x);
void printsteps();
void huarongdao();
void first();
int main();


void drawButton(int count)
{
    // 绘制按钮区域
    setfillcolor(WHITE);
    fillrectangle(240, 400, 400, 450);
    setfillcolor(DARKGRAY);
    fillrectangle(242, 402, 398, 448);

    // 绘制按钮文字
    settextstyle(20, 0, _T("微软雅黑"));
    settextcolor(WHITE);
    if (count < 10) {
        char text[128];
        snprintf(text, sizeof(text), "已抽取 %d 张卡牌", count);
        outtextxy(260, 415, text);
    }
    else {
        settextcolor(YELLOW);
        outtextxy(285, 413, _T("开启对战"));
    }
}
int randnum(int r)
{
    if (r <= 27)
        return 1;
    else if (r <= 36)
        return 2;
    else if (r <= 39)
        return 3;
    else
        return 4;
}
// 加载并旋转图片
void bxyao() 
{
    // 加载图片
    IMAGE bx;
    loadimage(&bx, ".\\卡牌素材\\bx.jpg", 100, 100);

    // 显示图片并延时
    rotateimage(&bx, &bx, 270);
    putimage(250, 380, &bx);
    Sleep(100);

    rotateimage(&bx, &bx, 180);
    putimage(250, 380, &bx);
    Sleep(100);

    rotateimage(&bx, &bx, 360);
    putimage(250, 380, &bx);
    Sleep(100);

    rotateimage(&bx, &bx, 90);
}
// 单帧更新画面
void updateFrame(int x, int y, double w, double h) 
{
    // 清空屏幕
    cleardevice();
    // 加载背景图和前景图
    IMAGE bk, img;
    loadimage(&bk, ".\\卡牌素材\\bk.jpg", w, h);

    // 绘制图片
    putimage(250, 380, &img);
    putimage(x, y, &bk);

    // 刷新缓存区
    FlushBatchDraw();
}
// 进行动画
void loadmage() {
    IMAGE img;
    int x = 250;
    int y = 480;
    double w = 0;
    double h = 2;
    // 每帧上升的像素数
    int dy = -26;
    // 每帧增加的宽度和高度
    int dw = 10;
    double dh = 15;
    for(int i=0;i<2;i++)
        bxyao();
    loadimage(&img, ".\\卡牌素材\\bx.jpg", 100, 100);
    while (y > 89) 
    {
        // 更新画面
        updateFrame(x, y, w, h);

        // 更新位置和大小
        y += dy;
        w += dw;
        h += dh;

        // 延时一段时间
        Sleep(50);
    }
}
// 绘制卡牌图片
void drawCardImage(Card* p)
{
    IMAGE kp;
    switch (p->rarity)
    {
    case 1:
        loadimage(&kp, ".\\卡牌素材\\pt.jpg");
        break;
    case 2:
        loadimage(&kp, ".\\卡牌素材\\xy.jpg");
        mciSendString(_T("close xy"), NULL, 0, NULL);
        mciSendString(_T("open .\\卡牌素材\\xy.mp3 alias xy"), NULL, 0, NULL);
        mciSendString(_T("play xy"), NULL, 0, NULL);
        break;
    case 3:
        loadimage(&kp, ".\\卡牌素材\\ss.jpg");
        mciSendString(_T("close ss"), NULL, 0, NULL);
        mciSendString(_T("open .\\卡牌素材\\ss.mp3 alias ss"), NULL, 0, NULL);
        mciSendString(_T("play ss"), NULL, 0, NULL);
        break;
    case 4:
        loadimage(&kp, ".\\卡牌素材\\cs.jpg");
        mciSendString(_T("close cs"), NULL, 0, NULL);
        mciSendString(_T("open .\\卡牌素材\\cs.mp3 alias cs"), NULL, 0, NULL);
        mciSendString(_T("play cs"), NULL, 0, NULL);
        break;
    default:
        loadimage(&kp, ".\\卡牌素材\\cs.jpg");
        break;
    }
    mciSendString(_T("play kp"), NULL, 0, NULL);
    Sleep(500);
    cleardevice();
    putpicture(250, 90, &kp,BLACK);
}
// 绘制卡牌属性
void drawCardAttributes(Card* p)
{
    char fei[10];
    char xue[10];
    char gong[10];
    char rarityStr[100];
    switch (p->rarity)
    {
    case 1:
        strcpy(rarityStr, "普通");
        break;
    case 2:
        strcpy(rarityStr, "稀有");
        break;
    case 3:
        strcpy(rarityStr, "史诗");
        break;
    case 4:
        strcpy(rarityStr, "传说");
        break;
    default:
        strcpy(rarityStr, "金色传说！！！");
        break;
    }
    sprintf(fei, "%d", p->fei);
    sprintf(gong, "%d", p->gong);
    sprintf(xue, "%d", p->xue);
    settextcolor(WHITE);
    settextstyle(20, 0, "微软雅黑");
    sprintf(text, "id:%d,稀有度：%s", p->id, rarityStr);
    outtextxy(80, 150, text);
    settextstyle(38, 0, _T("Consolas"));
    outtextxy(258, 105, fei);
    settextcolor(WHITE);
    settextstyle(35, 0, _T("Consolas"));
    outtextxy(258, 280, gong);
    outtextxy(365, 280, xue);
}
// 绘制卡牌UI
void drawCardUI(Card* p)
{
    setbkmode(TRANSPARENT);
    loadmage();
    drawCardImage(p);
    drawCardAttributes(p);
    while (p->next == NULL)
    {
        MOUSEMSG msg = GetMouseMsg();
        if (msg.uMsg == WM_LBUTTONDOWN)
            break;
    }
}
//模拟抽卡卡牌
Card* drawCard(Card* head, int* count, int cards[])
{
    Card* p = head->next;
    Card* prev = head;
    while (p != NULL) 
    {
        if (prev->next == p) 
        { // 判断当前卡牌是否已经被抽过
            prev->next = p->next;
            cards[*count] = p->rarity; // 存储抽到的卡牌稀有度
            (*count)++;
            return p;
        }
        prev = p;
        p = p->next;
    }
    return NULL;
}
// 创建并返回一个新的链表头节点
Card* createCardListHead()
{
    Card* head = (Card*)malloc(sizeof(Card));
    head->next = NULL;
    return head;
}
// 将新卡牌添加到链表末尾
void addCard(Card* head, Card* card)
{
    Card* prev = head;
    while (prev->next != NULL) {
        prev = prev->next;
    }
    prev->next = card;
}
// 生成具有随机值的新卡牌
void NewCard(Card* card, int id)
{
    int rarity = randnum(id);
    card->id = id;
    card->rarity = rarity;
    switch (rarity)
    {
    case 1:
        card->fei = 3;
        card->gong = rand() % 15 + 3;
        card->xue = 30 - card->gong;
        break;
    case 2:
        card->fei = 5;
        card->gong = rand() % 8 + 2;
        card->xue = 16 - card->gong;
        break;
    case 3:
        card->fei = 7;
        card->gong = rand() % 12 + 3;
        card->xue = 24 - card->gong;
        break;
    case 4:
        card->fei = 10;
        card->gong = rand() % 15 + 3;
        card->xue = 30 - card->gong;
        break;
    default:
        break;
    }
    card->next = NULL;
}
// 初始化卡牌链表
Card* initCardList(int count)
{
    Card* head = createCardListHead();
    Card* prev = head;
    for (int i = 0; i < count; i++)
    {
        Card* card = (Card*)malloc(sizeof(Card));
        int r = rand() % 40 + 1;
        NewCard(card, r);
        addCard(head, card);
        prev = card;
    }
    return head;
}
// 释放链表中所有节点所占用的内存
void freeCardList(Card* head)
{
    Card* p = head->next;
    while (p != NULL)
    {
        Card* tmp = p->next;
        free(p);
        p = tmp;
    }
    head->next = NULL;
}
// 用新卡牌重置链表
void resetCardList(Card* head, int count)
{
    freeCardList(head);
    for (int i = 0; i < count; i++)
    {
        Card* card = (Card*)malloc(sizeof(Card));
        int r = rand() % 40 + 1;
        NewCard(card, r);
        addCard(head, card);
    }
}
void showCards(int cards[])
{
    cleardevice();
    int a = 0, b = 0, c = 0, d = 0;
    for (int i = 0; i < 10; i++) 
    {
        char rarityStr[10];
        switch (cards[i]) 
        {
        case 1:
            a += 1;
            break;
        case 2:
            b += 1;
            break;
        case 3:
            c += 1;
            break;
        case 4:
            d += 1;
            break;
        default:
            sprintf(rarityStr, "金色传说！！！！！！！！！！！！！！！！！！！！！！！！！！");
            break;
        }
    }
    settextstyle(20, 0, "微软雅黑");
    outtextxy(60, 100, "抽卡结果：");
    sprintf(text, "普通：%d,稀有：%d,史诗：%d,传说：%d", a, b, c, d);
    outtextxy(120, 130, text);
    int sum = a + 3 * b + 9 * c + 27 * d;
    double pob = gailv(sum);
    sprintf(text, "你得了%d分,已超过了世界上%.2lf%%的人了", sum, pob);
    outtextxy(120, 160, text);
    FILE* fp;
    if ((fp = fopen("score.bin", "ab")) == NULL) {
        printf("Cannot open file %s!\n", "score.bin");
        return;
    }
    fprintf(fp, "%d\n", sum);
    fclose(fp);

}
void saveCards(Card* p) {
    FILE* fp;
    if ((fp = fopen(FILENAME, "ab"))== NULL) {
        printf("Cannot open file %s!\n", FILENAME);
        return;
    }
        fwrite(p, sizeof(Card), 1, fp);
    fclose(fp);
}
// 计算标准正态分布的累积分布函数值
double normal_cdf(double x)
{
    return 0.5 * (1+ erf(x / sqrt(2)));
}
// 计算抽奖获得总奖励值超过其他人的概率（以百分比形式返回）
double gailv(int user_reward)
{
    // 配置参数
    double standard_deviation = sqrt(VARIANCE);
    double z_score = (user_reward - MEAN) / standard_deviation;
    double prob = normal_cdf(z_score);
    return prob * 100;
}
struct card* maopao(struct card* head)
{
    if (head == NULL)
        return head;
    if (head->next == NULL)
        return head;
    struct card* tail = NULL;
    while (tail != head)
    {
        struct card* last_swap = head;
        for (struct card* p = head; p != tail; p = p->next)
        {
            if (p->next && p->id < p->next->id) {  // 如果相邻元素非递减，则交换它们
                int temp_id = p->id;
                int temp_rarity = p->rarity;
                int temp_gong = p->gong;
                int temp_xue = p->xue;
                int temp_fei = p->fei;
                p->id = p->next->id;
                p->rarity = p->next->rarity;
                p->gong = p->next->gong;
                p->xue = p->next->xue;
                p->fei = p->next->fei;
                p->next->id = temp_id;
                p->next->rarity = temp_rarity;
                p->next->gong = temp_gong;
                p->next->xue = temp_xue;
                p->next->fei = temp_fei;
                last_swap = p;  // 记录最后一次交换的位置
            }
            else
            {
                last_swap = p;
            }
        }
        tail = last_swap;  // 更新尾指针
    }

    return head;
}

void drawMainMenu()
{
    settextstyle(50, 0, "微软雅黑");
    settextcolor(WHITE);
    outtextxy(170, 150, "抽卡游戏");
    setfillcolor(WHITE);
    fillroundrect(180, 200, 460, 370, 20, 20);
    setfillcolor(RGB(0, 204, 204));
    fillroundrect(190, 210, 450, 360, 20, 20);
    settextstyle(30, 0, "微软雅黑");
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    outtextxy(220, 240, "开始游戏");
    outtextxy(270, 310, "退出游戏");
}
void myenemy()
{
    IMAGE ene;
    setbkmode(TRANSPARENT);
    setbkcolor(WHITE);
    Card* p = loadCards();
    maopao(p);
    char fei[200];
    char gong[200];
    char xue[200];
    for (int i = 0;p!=NULL; i++)
    {
        switch (p->rarity)
        {
        case 1:
            loadimage(&ene, ".\\卡牌素材\\pt.jpg", 64, 97);
            break;
        case 2:
            loadimage(&ene, ".\\卡牌素材\\xy.jpg", 64, 97);
            break;
        case 3:
            loadimage(&ene, ".\\卡牌素材\\ss.jpg", 64, 97);
            break;
        case 4:
            loadimage(&ene, ".\\卡牌素材\\cs.jpg", 64, 97);
            break;
        default:
            break;
        }
        settextcolor(WHITE);
        settextstyle(15, 0, _T("Consolas"));
        sprintf(fei, "%d", p->fei);
        sprintf(gong, "%d", p->gong);
        sprintf(xue, "%d", p->xue);
        putpicture(64 * i, 370, &ene,BLACK);
        outtextxy(6 + i * 64, 378, fei);
        outtextxy(4 + i * 64, 453, gong);
        outtextxy(45 + i * 64, 453, xue);
        p = p->next;
    }

}
int startfight(int a1, int a2,int a3,int b1, int b2,int b3)
{
    if (a3 == 4 && b3 ==2)
        return 1;
    else if (a3 == 4 && b3 == 3)
        return 1;
    else if (a3 == 2 && b3 == 1)
        return 1;
    else if (a3 == 2 && b3 == 2)
        return 1;
    else if (a3 == 1 && b3 == 2)
        return 2;
    else if (a3 == 3 && b3 == 2)
        return 1;
    else if (a3 == 2 && b3 == 3)
        return 2;
    else if (a3 == 2 && b3 == 4)
        return 2;
    else if (a3 == 3 && b3 == 4)
        return 2;
    if (a3 == 3)
        a2 *= 2;
    if (b3 == 3)
        b2 *= 2;
    while (1)
    {
        a2 -= b1;
        b2 -= a1;
        if (a2 <= 0)
            return 2;
        else if (b2 <= 0)
            return 1;
    }

}
int fight(int guan)
{
    Enemy di[5];
    IMAGE cho,dead;
    char fei[200];
    char gong[200];
    char xue[200];
    char egong[200];
    char exue[200];
    cleardevice();
    Showguan(guan);
    Sleep(1500);
    loadbk();
    int r=enemy(guan,di);
    myenemy();
    int a=choose();
    Card* q= loadCards();
    maopao(q);
    int i = 0;
    for (i = 0; i < a - 1; i++)
    {
        q = q->next;
    }
    cleardevice();
    loadbk();
    putpicture(290, 75, &dr, BLACK);
    sprintf(egong, "%d", di[guan].gong);
    sprintf(exue, "%d", di[guan].xue);
    outtextxy(293, 173, egong);
    outtextxy(348, 173, exue);
    switch (q->rarity)
    {
    case 1:
        loadimage(&cho, ".\\卡牌素材\\pt.jpg", 75,114);
        break;
    case 2:
        loadimage(&cho, ".\\卡牌素材\\xy.jpg", 75, 114);
        break;
    case 3:
        loadimage(&cho, ".\\卡牌素材\\ss.jpg", 75, 114);
        break;
    case 4:
        loadimage(&cho, ".\\卡牌素材\\cs.jpg", 75, 114);
        break;
    default:
        break;
    }
    settextcolor(WHITE);
    settextstyle(15, 0, _T("Consolas"));
    sprintf(fei, "%d", q->fei);
    sprintf(gong, "%d", q->gong);
    sprintf(xue, "%d", q->xue);
    putpicture(290, 200, &cho, BLACK);
    outtextxy(293, 210, fei);
    outtextxy(293, 298, gong);
    outtextxy(348, 298, xue);
    int ga = startfight(q->gong, q->xue, q->rarity, di[guan].gong, di[guan].xue,r);
    loadimage(&dead, ".\\卡牌素材\\dead.png", 50, 50);
    BeginBatchDraw();
    for (int w = 0; w <= 100; w++)
    {

        loadbk();
        putpicture(220+w, 170, &cho, BLACK);
        putpicture(440-w, 170, &dr, BLACK);
        FlushBatchDraw();
    }
    EndBatchDraw();
    cleardevice();
    loadbk();
    Sleep(400);
    putpicture(290, 75, &dr, BLACK);
    sprintf(egong, "%d", di[guan].gong);
    sprintf(exue, "%d", di[guan].xue);
    outtextxy(293, 173, egong);
    outtextxy(348, 173, exue);
    sprintf(fei, "%d", q->fei);
    sprintf(gong, "%d", q->gong);
    sprintf(xue, "%d", q->xue);
    putpicture(290, 200, &cho, BLACK);
    outtextxy(293, 210, fei);
    outtextxy(293, 298, gong);
    outtextxy(348, 298, xue);
    if (ga == 1)
    {
        putpicture(295,122 , &dead, BLACK);
        sprintf(text, "你…赢了");
        outtextxy(370, 130, text);
        Sleep(3000);
    }
    else
    {
        putpicture(295, 277, &dead, BLACK);
        sprintf(text, "你不堪一击");
        outtextxy(370, 130, text);
        mciSendString(_T("stop bkmusic"), NULL, 0, NULL);
        mciSendString(_T("open .\\卡牌素材\\dead.mp3 alias dead"), NULL, 0, NULL);
        mciSendString(_T("start dead"), NULL, 0, NULL);
        Sleep(3000);
    }
    free(q);
    return ga;

}
int choose()
{
    int a = 1;
    while (1) {
        if (MouseHit()) {  // 检测鼠标事件
            MOUSEMSG msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN) {   // 左键按下
                lastx = mousex = msg.x;
                lasty = mousey = msg.y;
            }
            else if (msg.uMsg == WM_LBUTTONUP) 
            {  // 左键释放
                if (lasty>=370)
                {
                    for (int i = 0; i < 100; i++)
                    {
                        if (lastx < 64)
                        {
                            return a;
                        }
                        lastx -= 64;
                        a += 1;
                    }
                }
            }

        }
    }
}
void putpicture(int dstx, int dsty, IMAGE* img, COLORREF color)
{
    DWORD* src = GetImageBuffer(img);
    DWORD* dst = GetImageBuffer(GetWorkingImage());
    int w1 = img->getwidth(), h1 = img->getheight(), w2;
    if (GetWorkingImage() == NULL)
        w2 = getwidth();
    else
        w2 = GetWorkingImage()->getwidth();
    for (int i = 0; i < w1; i++)
        for (int j = 0; j < h1; j++)
        {
            if (src[j * w1 + i] != color)
                dst[(j + dsty) * w2 + i + dstx] = src[j * w1 + i];
        }
    if (!GetWorkingImage())
        FlushBatchDraw();
}
void loadbk()
{
    IMAGE dt, ren, ren1;
    setbkmode(TRANSPARENT);
    loadimage(&dt, ".\\卡牌素材\\1.jpg");
    loadimage(&ren, ".\\卡牌素材\\1.png", 75, 90);
    cleardevice();
    putimage(0, 0, &dt);
    putpicture(290, 305, &ren, BLACK);
}
int enemy(int guan,  Enemy di[3])
{
    char text[128], gong[128], xue[128];
    IMAGE ene;
    int num = rand() % 40 + n;
    int r = randnum(num);
    switch (r)
    {
    case 1:
        sprintf(text, "看我强壮的身体，碾压你！");
        di[guan].gong = rand() % 13 + 3;
        di[guan].xue = 26- di[guan].gong;
        di[guan].xiaoguo = 1;
        loadimage(&dr, ".\\卡牌素材\\pt.jpg",75,114);
        break;
    case 2:
        sprintf(text, "一击毙命！");
        di[guan].gong = rand() % 6 + 3;
        di[guan].xue = 12 - di[guan].gong;
        di[guan].xiaoguo = 2;
        loadimage(&dr, ".\\卡牌素材\\xy.jpg", 75, 114);
        break;
    case 3:
        sprintf(text, "再战！");
        di[guan].gong = rand() % 8 + 3;
        di[guan].xue = 16 - di[guan].gong;
        di[guan].xiaoguo = 3;
        loadimage(&dr, ".\\卡牌素材\\ss.jpg", 75, 114);
        break;
    case 4:
        sprintf(text, "我，是无敌的！");
        di[guan].gong = rand() % 13 + 3;
        di[guan].xue = 26 - di[guan].gong;
        di[guan].xiaoguo = 3;
        loadimage(&dr, ".\\卡牌素材\\cs.jpg", 75, 114);
        break;
    default:
        break;
    }
    putpicture(290, 105, &dr, BLACK);
    settextcolor(WHITE);
    settextstyle(15, 0, _T("Consolas"));
    outtextxy(370,130,text);
    settextcolor(WHITE);
    sprintf(gong, "%d", di[guan].gong);
    sprintf(xue, "%d", di[guan].xue);
    outtextxy(293, 203, gong);
    outtextxy(348, 203, xue);
    return r;
}
Card* loadCards() {
    FILE* fp;
    if ((fp = fopen(FILENAME, "rb")) == NULL) {
        printf("Cannot open file %s!\n", FILENAME);
        return NULL;
    }

    Card* head = NULL;
    Card* tail = NULL;

    while (!feof(fp)) {
        Card* p = (Card*)malloc(sizeof(Card));
        if (fread(p, sizeof(Card), 1, fp) == 1) {
            p->next = NULL;
            if (head == NULL) {
                head = p;
            }
            else {
                tail->next = p;
            }
            tail = p;
        }
        else {
            free(p);
        }
    }

    fclose(fp);
    return head;
}
void Showguan(int guan)
{
    char text4[100];
    setlinecolor(BLACK);
    settextcolor(BLACK);
    setfillcolor(BLACK);
    settextstyle(40, 0, _T("微软雅黑"));
    sprintf(text4, "第%d关", guan);
    outtextxy(250, 30, text4);
}
void fightui()
{
    cleardevice();
    int guan = 1;
    char text1[1000], text2[1000], text3[100];
    setbkmode(TRANSPARENT);
    sprintf(text1, "你将用你刚才抽到的十张卡牌与对手对战。");
    sprintf(text2, "选择一张卡牌上阵，但不要凭借稀有度而选择，有些牌会出现奇迹。");
    settextcolor(WHITE);
    outtextxy(180, 30, text1);
    outtextxy(80, 50, text2);
    sprintf(text3, "开始对战");
    settextcolor(YELLOW);
    outtextxy(280, 400, text3);
    while (1)
    {
        MOUSEMSG msg = GetMouseMsg();
        if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 260 && msg.x <= 360 && msg.y > 290 && msg.y < 420)
        {
            cleardevice();
            break;
        }
    }
    setbkcolor(WHITE);
    cleardevice();
    for (int i = 1; i <= 4; i++)
    {
        if (fight(i) != 1)
        {
            cleardevice();
            setfillcolor(BLACK);
            IMAGE sb;
            loadimage(&sb, ".\\卡牌素材\\shibai.jpg");
            putimage(0, 0, &sb);
            Sleep(5000);
            remove(FILENAME);
            exit(0);
        }
    }
    settextcolor(WHITE);
    loadimage(&end, ".\\卡牌素材\\end.jpg", 640, 480);
    putimage(0, 0, &end);
    sprintf(text, "你成功的逃离了追击，这次探险收获满满，你期待下一次的有趣探险");
    outtextxy(200, 300, text);
    Sleep(5000);
    exit(0);
}
void initGame() 
{
    initgraph(640, 480);
    setbkmode(TRANSPARENT);
}
void mainMenu() 
{
    huarongdao();

}
void game() {
    remove(FILENAME);
    IMAGE bx;
    outtextxy(260, 115, "点击宝箱开始抽卡");
    loadimage(&bx, ".\\卡牌素材\\bx.jpg", 100, 100);
    putpicture(250, 380, &bx, BLACK);
    // 创建一个包含10张卡牌的卡牌列表，并使用冒泡排序算法对其进行排序
    Card* head = initCardList(10);
    int count = 0;
    int cards[10] = { 0 }; // 存储抽到的卡牌稀有度

    while (1)
    {
        if (MouseHit())
        {
            MOUSEMSG msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 250 && msg.x <= 390 && msg.y >= 400 && msg.y <= 450)
            {
                cleardevice();
                Card* p = drawCard(head, &count, cards);
                saveCards(p);
                if (p != NULL)
                {
                    drawCardUI(p);
                    drawButton(count);
                }
                if (count == 10)
                {
                    showCards(cards);
                    drawButton(count);
                    Sleep(3000);
                    resetCardList(head, 10);
                    count = 0;
                    while (1)
                    {
                        MOUSEMSG msg1 = GetMouseMsg();
                        if (msg1.uMsg == WM_LBUTTONDOWN && msg1.x >= 250 && msg1.x <= 390 && msg1.y >= 400 && msg1.y <= 450)
                        {
                            fightui();
                            break;
                        }
                    }
                }
            }
        }
        else if (GetAsyncKeyState(VK_ESCAPE))
        {
            resetCardList(head, 10);
            free(head);
            closegraph();
            remove(FILENAME);
            exit(0);
        }
    }
}
void chooseMode()
{
    setfillcolor(RGB(250, 248, 239));
    solidrectangle(0, 0, 640, 480);
    setfillcolor(RGB(244, 177, 121));
    solidroundrect(140, 60, 500, 160, 15, 15);
    solidroundrect(140, 190, 500, 290, 15, 15);
    solidroundrect(140, 320, 500, 420, 15, 15);
    RECT a{ 140, 60, 500, 160 };
    RECT b{ 140, 190, 500, 290 };
    RECT c{ 140, 320, 500, 420 };
    RECT d{ 140,0,500,60 };
    setbkcolor(RGB(244, 177, 121));
    settextstyle(60, 34, _T("楷体"));
    drawtext(_T("3×3"), &a, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    drawtext(_T("4×4"), &b, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    drawtext(_T("5×5"), &c, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    settextstyle(30, 17, _T("楷体"));
    solidroundrect(192, 12, 452, 52, 15, 15);
    drawtext(_T("数字华容道"), &d, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    settextstyle(60, 34, _T("楷体"));
    RECT n{ 14,60,130,420 };
    solidroundrect(14, 60, 130, 420, 15, 15);
    drawtext(_T("鼠标选择模式"), &n, DT_CENTER | DT_WORDBREAK);
    MOUSEMSG m;
    while (1)
    {
        m = GetMouseMsg();
        if (m.uMsg == WM_LBUTTONDOWN)
        {
            if (m.x >= 140 && m.x <= 500 && m.y >= 60 && m.y <= 160)
            {
                mode = 3;
                break;
            }
            if (m.x >= 140 && m.x <= 500 && m.y >= 190 && m.y <= 290)
            {
                mode = 4;
                break;
            }
            if (m.x >= 140 && m.x <= 500 && m.y >= 320 && m.y <= 420)
            {
                mode = 5;
                break;
            }
        }
    }
}
void createMap()
{
    count = 0;
    setfillcolor(RGB(187, 173, 160));
    solidroundrect(140, 60, 500, 420, 15, 15);
    setfillcolor(RGB(205, 193, 180));
    if (mode == 3)
    {
        for (i = 0; i < mode; i++)
        {
            for (j = 0; j < mode; j++)
            {
                solidroundrect(140 + 9 * (i + 1) + 108 * i, 60 + 9 * (j + 1) + 108 * j, 140 + 9 * (i + 1) + 108 * (i + 1), 60 + 108 * (j + 1) + 9 * (j + 1), 15, 15);
            }
        }
    }
    if (mode == 4)
    {
        for (i = 0; i < mode; i++)
        {
            for (j = 0; j < mode; j++)
            {
                solidroundrect(140 + 8 * (i + 1) + 80 * i, 60 + 8 * (j + 1) + 80 * j, 140 + 8 * (i + 1) + 80 * (i + 1), 60 + 80 * (j + 1) + 8 * (j + 1), 15, 15);
            }
        }
    }
    if (mode == 5)
    {
        for (i = 0; i < mode; i++)
        {
            for (j = 0; j < mode; j++)
            {
                solidroundrect(140 + 5 * (i + 1) + 66 * i, 60 + 5 * (j + 1) + 66 * j, 140 + 5 * (i + 1) + 66 * (i + 1), 60 + 66 * (j + 1) + 5 * (j + 1), 15, 15);
            }
        }
    }

    for (i = 0; i < mode; i++)
    {
        for (j = 0; j < mode; j++)
        {
            arr[i][j] = 0;
        }
    }
    settextstyle(30, 17, _T("楷体"));
    settextcolor(RGB(245, 249, 241));
    setbkcolor(RGB(143, 122, 102));
    setfillcolor(RGB(143, 122, 102));
    solidroundrect(192, 12, 452, 52, 15, 15);
    RECT a{ 140,0,500,60 };
    drawtext(_T("数字华容道"), &a, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    settextstyle(20, 13, _T("楷体"));
    RECT m{ 14,60,130,420 };
    solidroundrect(14, 60, 130, 420, 15, 15);
    drawtext(_T("ESC\n\n主菜单\n\n\n空格键\n\n开始游戏\n\n\nW上\n\nS下\n\nA左\n\nD右"), &m, DT_CENTER | DT_WORDBREAK);
    printsteps();
}
void startGame()
{
    createCube();
}
void direction(int x)
{
    int c = 0;
    if (x == 1)
    {
        for (i = 0; i < mode; i++)
        {
            for (j = 0; j < mode - 1; j++)
            {
                if (arr[j][i] == 0)
                {
                    c = 1;
                    break;
                }
            }
            if (c == 1)
                break;
        }
        if (c == 1)
        {
            temp = arr[j][i];
            arr[j][i] = arr[j + 1][i];
            arr[j + 1][i] = temp;
            rebuild();
            c = 0;
            count++;
            printsteps();
        }
    }
    if (x == 2)
    {
        for (i = mode - 1; i > 0; i--)
        {
            for (j = 0; j < mode; j++)
            {
                if (arr[i][j] == 0)
                {
                    c = 2;
                    break;
                }
            }
            if (c == 2)
                break;
        }
        if (c == 2)
        {
            temp = arr[i - 1][j];
            arr[i - 1][j] = arr[i][j];
            arr[i][j] = temp;
            rebuild();
            c = 0;
            count++;
            printsteps();
        }
    }
    if (x == 3)
    {
        for (i = 0; i < mode - 1; i++)
        {
            for (j = 0; j < mode; j++)
            {
                if (arr[j][i] == 0)
                {
                    c = 3;
                    break;
                }
            }
            if (c == 3)
                break;
        }
        if (c == 3)
        {
            temp = arr[j][i];
            arr[j][i] = arr[j][i + 1];
            arr[j][i + 1] = temp;
            rebuild();
            c = 0;
            count++;
            printsteps();
        }
    }
    if (x == 4)
    {
        for (i = mode - 1; i > 0; i--)
        {
            for (j = 0; j < mode; j++)
            {
                if (arr[j][i] == 0)
                {
                    c = 4;
                    break;
                }
            }
            if (c == 4)
                break;
        }
        if (c == 4)
        {
            temp = arr[j][i];
            arr[j][i] = arr[j][i - 1];
            arr[j][i - 1] = temp;
            rebuild();
            c = 0;
            count++;
            printsteps();
        }
    }
}
void contralGame()
{
    while (1) {
        if (MouseHit()) {  // 检测鼠标事件
            MOUSEMSG msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN) {   // 左键按下
                isDragging = true;
                lastx = mousex = msg.x;
                lasty = mousey = msg.y;
            }
            else if (msg.uMsg == WM_LBUTTONUP) {  // 左键释放
                isDragging = false;
                int dx = mousex - lastx;
                int dy = mousey - lasty;
                if (abs(dx) > abs(dy)) {
                    if (dx > 0) direction(4);      // 向右移动
                    else direction(3);             // 向左移动
                }
                else {
                    if (dy > 0) direction(2);      // 向下移动
                    else direction(1);             // 向上移动
                }
                judge();                           // 判断游戏是否结束
            }
            else if (msg.uMsg == WM_MOUSEMOVE && isDragging) {   // 鼠标拖动中
                mousex = msg.x;
                mousey = msg.y;
            }
        }
        if (GetAsyncKeyState('W') & 0x8000) {
            direction(1); // 向上移动
            judge();      // 判断游戏是否结束
            Sleep(500);// 暂停一段时间，避免按键过快导致拼图块移动过快
        }
        else if (GetAsyncKeyState('S') & 0x8000) {
            direction(2); // 向下移动
            judge();
            Sleep(500);
        }
        else if (GetAsyncKeyState('A') & 0x8000) {
            direction(3); // 向左移动
            judge();
            Sleep(500);
        }
        else if (GetAsyncKeyState('D') & 0x8000) {
            direction(4); // 向右移动
            judge();
            Sleep(500);
        }
    }
}
void createCube()
{
    srand((unsigned)time(NULL));
    int m = 1;
    int t, t1, t2;
    int s = 0;
    for (i = 0; i < mode; i++)//先生成正确的状态
    {
        for (j = 0; j < mode; j++)
        {
            arr[i][j] = m;
            m++;
        }
    }
    arr[mode - 1][mode - 1] = 0;//空位的值设为0
    t1 = 1000;
    while (t1 != 1)//1000循环随机调换位置使之达到混乱状态
    {
        t1--;
        t = rand() % (mode - 1) + 1;
        for (i = 0; i < t; i++)//横向随机调换
        {
            for (j = 0; j < t + 1; j++)
            {
                if ((i + 1) != (mode - 1) || j != (mode - 1))
                {
                    temp = arr[i][j];
                    arr[i][j] = arr[i + 1][j];
                    arr[i + 1][j] = temp;
                    count++;
                }
            }
        }
        t2 = rand() % (mode - 1) + 1;
        for (i = 0; i < t2 + 1; i++)//纵向随机调换
        {
            for (j = 0; j < t2; j++)
            {
                if ((j + 1) != (mode - 1) || i != (mode - 1))
                {
                    temp = arr[i][j];
                    arr[i][j] = arr[i][j + 1];
                    arr[i][j + 1] = temp;
                    count++;
                }
            }
        }
    }
    if (count % 2 != 0)//判断调换位置次数的奇偶性
    {
        temp = arr[mode - 3][mode - 3];
        arr[mode - 3][mode - 3] = arr[mode - 2][mode - 2];
        arr[mode - 2][mode - 2] = temp;
    }
    for (i = 0; i < mode; i++)
    {
        for (j = 0; j < mode; j++)
        {
            for (int k = 0; k < (mode * mode); k++)        //遍历数组，把数组里面的数字实际打印出来
            {
                if (arr[i][j] == k)
                    printNumber(k);
            }
        }
    }
    count = 0;
}
void rebuild()
{
    for (i = 0; i < mode; i++)
    {
        for (j = 0; j < mode; j++)
        {
            for (int k = 0; k < (mode * mode); k++)
            {
                if (arr[i][j] == k)
                    printNumber(k);
            }
        }
    }
}
void judge()
{
    int m = 1;
    int n = 0;
    for (i = 0; i < mode; i++)
    {
        for (j = 0; j < mode; j++)
        {
            if (arr[i][j] == m)
            {
                n++;
            }
            m++;
        }
    }
    if (n == (mode * mode - 1))
    {
        RECT a{ 140,60,320,240 };
        RECT b{ 320,60,600,240 };
        settextstyle(25, 15, _T("楷体"));
        settextcolor(RGB(118, 110, 101));
        drawtext(_T("恭喜你!"), &a, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        drawtext(_T("成功拿到宝箱。"), &b, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        Sleep(3000);
        setbkcolor(BLACK);
        cleardevice();
        game();
    }
}
void printNumber(int x)
{
    int g = 0;
    int h = 0;
    if (mode == 3)
    {
        g = 9;
        h = 108;
    }
    if (mode == 4)
    {
        g = 8;
        h = 80;
    }
    if (mode == 5)
    {
        g = 5;
        h = 66;
    }
    if (x == 0)
    {
        setfillcolor(RGB(205, 193, 180));
        solidroundrect(140 + g * (j + 1) + h * j, 60 + g * (i + 1) + h * i, 140 + g * (j + 1) + h * (j + 1), 60 + h * (i + 1) + g * (i + 1), 15, 15);
        RECT a{ 140 + g * (j + 1) + h * j, 60 + g * (i + 1) + h * i, 140 + g * (j + 1) + h * (j + 1), 60 + h * (i + 1) + g * (i + 1) };
    }
    else
    {
        setfillcolor(RGB(237, 228, 218));
        solidroundrect(140 + g * (j + 1) + h * j, 60 + g * (i + 1) + h * i, 140 + g * (j + 1) + h * (j + 1), 60 + h * (i + 1) + g * (i + 1), 15, 15);
        RECT a{ 140 + g * (j + 1) + h * j, 60 + g * (i + 1) + h * i, 140 + g * (j + 1) + h * (j + 1), 60 + h * (i + 1) + g * (i + 1) };
        settextstyle(50, 30, _T("黑体"));
        settextcolor(RGB(118, 110, 101));
        setbkcolor(RGB(237, 228, 218));
        TCHAR s[10];
        _stprintf_s(s, _T("%d"), x);
        drawtext(_T(s), &a, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
}
void printsteps()
{
    settextstyle(30, 17, _T("楷体"));
    settextcolor(RGB(245, 249, 241));
    setbkcolor(RGB(187, 172, 160));
    setfillcolor(RGB(187, 172, 160));
    solidroundrect(514, 160, 629, 324, 15, 15);
    RECT w{ 514, 170, 629, 240 };
    RECT r{ 514, 220, 629, 300 };
    TCHAR s[10];
    _stprintf_s(s, _T("%d"), count);
    drawtext(_T("步数"), &w, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    drawtext(_T(s), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
void huarongdao()
{
    cleardevice();
    chooseMode();
    createMap();
    startGame();
    contralGame();
    _getch();
    closegraph();
}
void first()
{
    IMAGE first;
    loadimage(&first, ".\\卡牌素材\\gushi.png",640,480);
    putimage(0, 0, &first);
    Sleep(3000);
        while (1)
        {
            MOUSEMSG msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN)
            {
                cleardevice();
                huarongdao();
            }
        }
}
void shezhi()
{
    int x = 200; // 进度条初始位置
    int y = 250;
    int w = 200; // 进度条长度
    int h = 30;  // 进度条高度
    int pos = 0; // 进度条当前位置（百分比）

    while (1) // 等待键盘输入或关闭窗口
    {

        cleardevice(); // 清空画面
        settextcolor(WHITE);
        sprintf(text, "调整难度");
        outtextxy(200, 100, text);
        sprintf(text, "数值越大，难度越高");
        outtextxy(200, 150, text);
        RECT a{ 150,400, 300, 480 };
        drawtext(_T("开始探险"), &a, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        // 绘制进度条底部灰色矩形
        setfillcolor(LIGHTGRAY);
        solidrectangle(x, y, x + w, y + h);

        // 计算当前进度条的颜色
        COLORREF color = pos > 50 ? RED :GREEN;

        // 绘制进度条显示的矩形
        setfillcolor(color);
        solidrectangle(x, y, x + pos / 100.0 * w, y + h);

        // 绘制进度条边框黑色矩形
        setlinestyle(PS_SOLID, 2);
        rectangle(x, y, x + w, y + h);

        // 显示当前进度百分比
        setbkmode(TRANSPARENT);
        settextcolor(BLACK);
        char text[16];
        sprintf(text, "%d%%", pos);
        outtextxy(x + w / 2 - textwidth(text) / 2, y + h / 2 - textheight(text) / 2, text);

        // 获取鼠标消息
        MOUSEMSG m,m1;
        while (MouseHit())
        {
            m = GetMouseMsg();
            m1 = GetMouseMsg();
            if (m.uMsg == WM_MOUSEMOVE && m.mkLButton)
            {
                int px = m.x; // 鼠标当前位置
                if (px < x) px = x;
                if (px > x + w) px = x + w;
                pos = (px - x) * 100.0 / w; // 计算进度百分比
            }
            else if (m1.uMsg==WM_LBUTTONDOWN&&m1.x >= 150 && m1.x <= 300 && m1.y >= 400 && m1.y <= 480)
            {
                n = 40 * pos;
                huarongdao();
            }
        }

        Sleep(20); 
    }
}

// 显示输入提示，并等待用户输入文本
void getInputText(const char* prompt, char* text) {
    printf("%s", prompt);
    fgets(text, MAX_LEN + 1, stdin); // 读取用户输入的文本
    int len = strlen(text);
    if (len > 0 && text[len - 1] == '\n') { // 去除末尾的换行符
        text[len - 1] = '\0';
    }
}

// 加载账号和密码
void loadAccount(Account* account) {
    FILE* fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        account->username[0] = '\0';
        account->password[0] = '\0';
    }
    else {
        fscanf(fp, "%s %s", account->username, account->password);
        fclose(fp);
    }
}

// 保存账号和密码
void saveAccount(Account* account) {
    FILE* fp = fopen(FILE_NAME, "w");
    if (fp != NULL) {
        fprintf(fp, "%s %s", account->username, account->password);
        fclose(fp);
    }
}

int denglu() {

    // 加载账号和密码
    Account account;
    loadAccount(&account);

    // 如果账号和密码为空，则提示用户注册
    if (strlen(account.username) == 0 || strlen(account.password) == 0) {
        printf("您第一次登录，请注册\n");
        getInputText("账号: ", account.username);
        getInputText("密码: ", account.password);
        saveAccount(&account);
        system("cls");
    }

    // 循环等待用户输入账号和密码，并检查是否匹配
    int count = 0;
    while (true) {
        char username[MAX_LEN + 1] = { 0 };
        char password[MAX_LEN + 1] = { 0 };
        getInputText("请输入您的帐号:\n", username);
        getInputText("请输入您的密码 ", password);
        if (strcmp(username, account.username) == 0 && strcmp(password, account.password) == 0) {
            printf("登陆成功，开始游玩!\n");
            return 1;
        }
        else if(strcmp(username, "admin") == 0 && strcmp(password, "1111") == 0) {
            remove("account.txt");
            system("cls");
            main();  
        }

        else {
            count++;
            if (count >= 3) {
                MessageBox(NULL, "您输错密码次数太多了，请重新登录", "Error", MB_OK | MB_ICONERROR);
                return -1;
            }
            printf("账号密码错误\n");
            Sleep(2000);
            system("cls");
        }
    }

}

// 插入节点到链表中
void insertnode(struct Node** headRef, int data) {
    // 分配新节点
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;

    // 如果链表为空，则将新节点作为头结点
    if (*headRef == NULL) {
        *headRef = newNode;
    }
    // 否则在有序链表中找到合适的插入位置
    else {
        struct Node* p = *headRef;
        struct Node* prev = NULL;
        while (p != NULL && data < p->data) {
            prev = p;
            p = p->next;
        }
        // 在链表中插入新节点
        if (prev == NULL) {
            newNode->next = *headRef;
            *headRef = newNode;
        }
        else {
            newNode->next = prev->next;
            prev->next = newNode;
        }
    }
}


// 从文件中读取数据，并形成排行榜
struct Node* readDataAndCreateRanking(char* filename)
{
    FILE* fp = fopen(filename, "rb");  // 打开输入文件
    struct Node* head = NULL;  // 定义链表头指针

    // 从输入文件中读取整数并添加到有序链表中
    int x;
    while (fscanf(fp, "%d\n", &x) == 1) 
    {
        insertnode(&head, x);
    }

    fclose(fp);  // 关闭输入文件

    return head;
}

// 将排行榜画在屏幕上
void drawRanking(struct Node* head) {

    cleardevice();  // 清空绘图区域

    // 绘制标题
    settextcolor(YELLOW);
    settextstyle(30, 0, _T("Arial"));
    outtextxy(200, 50, _T("排 行 榜"));

    // 绘制表头
    settextcolor(WHITE);
    settextstyle(20, 0, _T("Arial"));
    outtextxy(120, 100, _T("名次"));
    outtextxy(360, 100, _T("分数"));

    // 绘制排行榜
    settextcolor(YELLOW);
    settextstyle(18, 0, _T("Arial"));
    int i = 1;
    struct Node* p = head;
    while (p != NULL) {
        TCHAR rank[10];
        _stprintf_s(rank, _T("%d"), i++);
        outtextxy(120, 100 + i * 30, rank);
        TCHAR score[10];
        _stprintf_s(score, _T("%d"), p->data);
        outtextxy(360, 100 + i * 30, score);
        p = p->next;
    }
    while (1)
    {
        MOUSEMSG msg = GetMouseMsg(); 
        if (msg.uMsg == WM_LBUTTONDOWN)
        {
            IMAGE start;
            loadimage(&start, ".\\卡牌素材\\start.jpg");
            settextstyle(30, 18, _T("楷体"));
            settextcolor(WHITE);
            putimage(0, 0, &start);
            setfillcolor(YELLOW);
            RECT a{ 480,0, 640, 80 };
            RECT b{ 480,100, 640, 180 };
            RECT c{ 480,200, 640, 280 };
            RECT d{ 480,300, 640, 380 };
            RECT e{ 480,400, 640, 480 };
            drawtext(_T("排行榜"), &e, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            drawtext(_T("开始探险"), &a, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            drawtext(_T("退出探险"), &b, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            drawtext(_T("背景故事"), &c, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            drawtext(_T("设置"), &d, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            return;
        }
    }
}

void paihang() {
    const char* filename = "score.bin";  // 数据文件名

    // 从文件中读取数据，并形成排行榜
    struct Node* head = readDataAndCreateRanking((char*)filename);

    // 将排行榜画在屏幕上
    drawRanking(head);

    return;
}


int main() 
{
    while (1)
    {
        if (denglu() == 1)
            break;
        else
        {
            Sleep(2000);
            exit(0);
        }
    }
    initGame();
    srand((unsigned int)time(NULL));
    mciSendString(_T("open .\\卡牌素材\\bkmusic.mp3 alias bkmusic"), NULL, 0, NULL);
    mciSendString(_T("play bkmusic repeat"), NULL, 0, NULL);
    // 声明按钮变量
    IMAGE start;
    loadimage(&start, ".\\卡牌素材\\start.jpg");
    settextstyle(30, 18, _T("楷体"));
    settextcolor(WHITE);
    putimage(0, 0, &start);
    setfillcolor(YELLOW);
    RECT a{ 480,0, 640, 80 };
    RECT b{ 480,100, 640, 180 };
    RECT c{ 480,200, 640, 280 };
    RECT d{ 480,300, 640, 380 };
    RECT e{ 480,400, 640, 480 };
    drawtext(_T("排行榜"), &e, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    drawtext(_T("开始探险"), &a, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    drawtext(_T("退出探险"), &b, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    drawtext(_T("背景故事"), &c, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    drawtext(_T("设置"), &d, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    // 循环检测鼠标点击事件
    while (1)
    {
        if (MouseHit()) {   // 如果有鼠标事件发生
            MOUSEMSG msg = GetMouseMsg();
            // 判断鼠标是否点击到了按钮区域
            if (msg.uMsg == WM_LBUTTONDOWN) {
                if (msg.x >= 480 && msg.x <= 580 && msg.y >= 0 && msg.y <= 80) {    // 点击了开始探险按钮
                    cleardevice();
                    huarongdao();
                    //game();
                }
                else if (msg.x >= 480 && msg.x <= 580 && msg.y >= 100 && msg.y <= 180) {  // 点击了退出按钮
                    cleardevice();
                    exit(0);
                }
                else if (msg.x >= 480 && msg.x <= 580 && msg.y >= 200 && msg.y <= 280) {  // 点击了背景故事按钮
                    cleardevice();
                    first();
                }
                else if (msg.x >= 480 && msg.x <= 580 && msg.y >= 300 && msg.y <= 380) {  // 点击了设置按钮
                    cleardevice();
                    shezhi();
                }
                else if (msg.x >= 480 && msg.x <= 580 && msg.y >= 400 && msg.y <= 480) {  // 点击了排行榜按钮
                    cleardevice();
                    paihang();
                }

            }
        }
    }
    // 关闭图形界面窗口
    closegraph();
    remove(FILENAME);
    return 0;
}
