//
// Created by server1 on 21-3-26.
//
//编译预处理指令＃开头

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "ResialbeArray.h"
#include "node.h"
/*宏定义见defineuse*/
#define PI 3.14159  //千万不能加分号，并不是Ｃ的语言。如果if后面使用宏多了分号，会使得出现;;　if/else会有问题
#define FORMAT "%d\n"
#define PI2 2*PI //可以运算
#define PRT printf("%f\t",PI); \
            printf("%f\n",PI2); \
            printf(FORMAT,15)   //　符号\表明一行未结束
#define _DEBUG //无值的宏用来进行条件编译的,由编译预处理的指令来检查这个宏
#define cube(x)  ((x)*(x)*(x))//带参数的宏
#define RADTODEG1(x) (x*57.29578)
#define RADTODEG2(x) (x)*57.29578  //不好，一切都需要括号，整个值需要括号，参数出现的每个地方都要有括号　（（x）*57.29578）
#define MIN(a,b) ((a)>(b)?(a):(b))
//const double PI = 3.14159;

typedef int Length;//类型定义，此时Length就是int 的别名 typedef 后第一个是原来的类型，第二个是新的别名  Length a,b,c
typedef long int64_t;

typedef union {//其中的成员占据同一份内存控件
    //同一时间只有一个成员是有效的，union的大小是其最大的成员　，初始化：对第一个成员做初始化
    int i;
    char ch[sizeof(int)];
}CHI;//elt1.i = 4,elt2.c = 'a'

/*typedef */struct data /*Data1*/{//结构　　　struct data today; today.month
    //初始化　struct data today＝｛3，１，2013｝/struct date this month={.month =7, .year = 2014},此时day = 0
    //结构成员可以是不同类型　结构可以运算，p3=(struct point){1,3} p1 = p2 即p1.x = p2.x p1.y = p2.y而数组无法这样做
    int year;
    int month;
    int day;
}Data2;//需要;
/**
 * 如果　没有typedef ,那么struct 后面的名字用来新建，最后括号后面的字段为变量。 用法：struct Data1 data_1,data_2;或者直接使用 Data2(变量)
 * 如果　有typedef，那么struct后面不需要名字，最后括号后面的即名字。　用法：Data2 data_3,data_4;
 * 对于union，差不多，最先有typedef，那么该行的名字即可以省略,否则不能省略union　关键字: union AnElt1 a;   AnElt2 b;
 */

//or
struct point{     //这种结构可以省略名字point，不太常见
    int x;
    int y;
}p1,p2;

struct rectangle{
    struct point p1;
    struct point p2;
};

void changeNumber(int a,int b);//声明  可以不加参数名　　void changeNuber(int ,int )　Ｃ语言无法传递变量进函数
//只能传递值，所以a=1,b=2,不存在swap(a,b)的函数交换两个变量的值，只是１，２传进去做了交换，不会影响外面实际a,b的值
void timeDuration();//时间间隔计算
void guessNumber(void);  //声明时添加void,规定函数无输入参数
void numOrderChange(int x);//交换数字顺序
int isPrime(int x);//素数判断
void moneyDivider(int total);//零钱凑整
void aboveAvrange();//平均数之上的数字多少个
void numCount();//* 计算输入的0-9以内的数字有多少个　
int search(int key,int a[],int length);//搜索数组内的数字
int isPrime2(int x);//素数的初级优化方案
void chessGame();//3*3棋盘游戏
void address(); //指针！
void pmethod(int* p);//指针作为函数参数
void minmax(int a[],int lengh, int *min,int *max);//解释数组传入函数时为什么不能在函数内部使用sizeof(数组a)的方式算数组大小
void zhizhenandconst(const int * qwe);//const int* / int * const
void swap_new(int *a , int *b);//可以使用的swap函数
int divide(int a, int b, int *result);//指针的一个作用，使函数返回多个结果。函数本身可以有返回值，结果也可以从指针带回，返回０表明无法作除法。
void mostErrorAboutZhizhen();//指针使用的最常见错误
void zhengshufanwei();//一些范围和unsigned
void zhengshudeInandOut();//整数的输入输出
void floatfanwei();//Float,double范围
void bool();//逻辑类型，Ｃ不存在
void Pjisuan();//指针的计算
void dynomicStorage();//动态内存分配
void stringshuzu();//二维数组的指针表示
void structP(struct data *p);//将结构的指针传入函数, (*p).month = 12  即 指针所指向的结构体中的成员month = 12，即简写成　p->month
void output(const struct data *p);
void structInstruct();//结构中嵌套结构
void uniontest();//union场合
void staticValue();//静态本地变量，static 初始化只在第一次进入函数时做，当函数离开时，静态本地变量会继续存在并保持其值，以后进入函数时保持上次离开的值
//静态本地变量实际上是特殊的全局变量，生存期是全局的，在内存中的位置紧贴在一起，但是只有局部的作用域

/* 使用全局变量和静态本地变量的函数是线程不安全的*/
void defineuse();//宏定义

void printscanf();//%d的解释

void fileOpen();//文件的标准打开方式

void Pmethod2();//指向函数的指针
void f();
void cal(int (*f)(int a,int b));
int plus(int a,int b);
int minus(int a,int b);
enum COLOR{ //枚举类型,比const int更为方便一些　COLOR这个名字通常不用，通常只用大括号里的名字，只能是int类型，值为从0到n
    //void f(enum COLOR c)
    //实际上Ｃ语言枚举很少用，做的不太好
    RED,//0　，ＲＥＤ　＝１　枚举可以指定固定值，指定后的下一个元素没有指定的话，就是指定的值 + 1
    YELLOW,//1
    GREEN,//2
    NUMBEROFCOLORS//3,小套路,最后一个元素值是前面的计数
};
/**
 * typedef enum{
 *  BUTTON_NONE;
 *  BUTTON_LEFT;    //这样定义也很常见，0,1,2
 * }AclMouseEvent;
 */

/**
 *
   void f(enum COLOR c){//Ｃ语言必须带上enum
    enum COLOR t = RED;
    enum COLOR A =YELLOW;
    enum COLOR B = GREEN;
    int color[NUMBEROFCOLORS] = {};
}
 */

/**
 * 一个返回指针的函数
 * 返回一个函数的本地变量的指针是非常不安全的
 * 返回全局变量或静态本地变量的地址是安全的
 * 最好的做法是返回传入的指针
 *
 */


/*
 * char world[] = {'H','E','L','L','O','!'}   字符数组
 * char world[] = {'H','E','L','L','O','!'，'\0'} 既是字符数组也是字符串　　0或'\0'是一样的但是不可以是'0'
 * 对Ｃ语言来说，以０或'\0'结尾的一串字符即是字符串，0标志字符串的结束，但他不是字符串的一部分
 * 计算字符串长度的时候不包括这个0
 * 字符串以数组的形式存在，以数组或指针的形式访问string.h有很多处理字符串的函数
 * char c[] = "hello"  长度为6.编译器会自动生成末尾的０
 * Ｃ语言的字符串是以字符数组的形式存在的，不能用运算符对字符串运算，通过数组的方式可以遍历字符串
 */
void string();
void stringhmethod();

int Global;//没有做初始化的全局变量会自动或的0值，指针会自动得到NULL
int* globalP;


int main(/*void*/int argc,char const *argv[]) {//main函数本身也是一个函数，其他地方有定义
//    changeNumber(5,4);
//    timeDuration();
//    guessNumber();
//    numOrderChange(12345);
//    isPrime(23);
//    moneyDivider(3);
//    aboveAvrange();
//    numCount();
//    {
//        /**
//         * 数组的集成初始化还可以　a[10] = {2},即a[0] = 2,其他为0,  a[] = {a[1]=2,a[5]=7}即大小为６的数组，a[1],a[5]确定，其他为０
//         * sizeof(a)　数组a的字节占据大小  所以一个数组的元素格式可以用sizeof(a)/sizeof(a[0])，永远适用！！ sizeof的类型返回用%lu
//         * %d 十进制有符号整型，%u十进制无符号整型　%i相同
//         * ％ld 长整型输出
//         * %lu long unsigned 无符号长整型或无符号长浮点数
//         * ％f　十进制浮点数
//         * %o 八进制数　%x　十六进制数（不输出0x）
//         * %p　输出地址　，printf("%p",&a);　
//         * %s　输出字符串　%c 输出单个字符　　%%输出符号％
//         *
//         *
//         * 有a[]被初始化，不能int b[] = a,无法直接赋值！！必须采用遍历赋值，唯一方法
//         */
//        int a[] = {11,23,9,6,15,13,8,32,7,14,19};
//        int count = sizeof(a)/ sizeof(a[0]);
//        printf("数组a的长度%d,a的总大小%lu,a[0]的大小%lu\n",count, sizeof(a), sizeof(a[0]));
//        int x;
//        int loc;
//        printf("请输入一个数字：");
//        scanf("%d",&x);
//        loc = search(x,a, sizeof(a)/sizeof(a[0])); //当数组作为函数参数时，往往需要另一个参数来传入数组的大小，如果a[]传进去，无法在函数中使用sizeof(a)
//        //跟指针有关，不然无法计算！！！！！！！！！！！！！！！
//        if(loc != -1){
//            printf("%d在第%d个位置上\n",x,loc);
//        }else{
//            printf("%d不存在\n",x);
//        }
//    }
//    chessGame();
//    address();
//    {
//        int i = 6;
//        printf("&i=%p\n",&i);
//        printf("before:%d\n",i);
//        pmethod(&i);
//        printf("after:%d\n",i);
//    }
//    {
//        int a[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,21,23,55,};
//        printf("main sizeof(a)=%lu\n", sizeof(a));
//        int min,max;
//        minmax(a, sizeof(a)/ sizeof(a[0]),&min,&max);
//        printf("min=%d,max=%d,a[0]=%d\n",min,max,a[0]);
//        int *p = &min;
//        printf("*p = %d\n",*p);
//        printf("p[0]=%d\n",p[0]); //数组本身是特殊的指针，int a[10];int *p =a; 但是数组的单元表达的是变量，需要用＆取地址　a==&a[0] p[0] <==>a[0]
//        printf("*a = %d\n",*a);
////        int a[] <==> int *const a 数组变量是const的指针，所以不能被赋值，所以int b[] = a 不可以
//
//    }
//    string();
//    stringhmethod();
//    Pjisuan();
//    dynomicStorage();
//    {
//        struct data today = {};
//        structP(&today);
//        output(&today);
//    }
//    structInstruct();
//    uniontest();
//    {
//    staticValue();
//    staticValue();
//    staticValue();
//}
//    defineuse();
//    printscanf();
//    fileOpen();
    Pmethod2();
    return 0; //return -1 结果255,return 2 结果　２,0代表程序运行正确
}

/**
 * 交换顺序
 * @param a
 * @param b
 */
void changeNumber(int a,int b) {
    int t;
    t = a;
    a = b;
    b = t;
    printf("a = %d, b = % d\n", a, b);//函数内部做了值的交换，传进该函数的没有变量只有实际的数值
}
/**
 * 时间间隔
 */
void timeDuration() {
    printf("请输入两个时间:\n");
    int hour1, minute1;
    int hour2, minute2;
    scanf("%d %d", &hour1, &minute1);
    scanf("%d %d", &hour2, &minute2);
    int h1 = hour2 - hour1;
    int m1 = minute2 - minute1;
    if (m1 < 0) {
        m1 = 60 + m1;
        h1--;
    }
    printf("时间差%d小时%d分钟\n", h1, m1);
}
//    printf("5 ==3 ? %d\n", 5==3);

/**
 * 猜数游戏
 */
void guessNumber() {
    srand(time(0));
    int srand_num = rand() % 100 + 1;  //随机100以内的数字
    int num;
    int count = 0;
    do {
        printf("请输入一个数字:\n");
        count++;
        scanf("%d", &num);
        if (num < srand_num) {
            printf("你输入的数小了\n");
        } else if (num > srand_num) {
            printf("你输入的数大了\n");
        }
    } while (num != srand_num);
    printf("你用了%d次找到了该数:%d\n", count, srand_num);
}

/**
    整数逆序
 */
void numOrderChange(int x) {
    int digit, ret;
    while (x > 0) {
        digit = x % 10;
        printf("digit = %d, ", digit);
        ret = ret * 10 + digit;
        x /= 10;
        printf("x = %d\n", x);
    }
    printf("ret = %d\n", ret);
}

/**
 * 素数
 */
int isPrime(int x) {
    int isPrime = 1;
    for (int i = 2; i < x; i++) {
        if (x % i == 0) {
            isPrime = 0;
            break;
            // continue;  跳过这一轮的循环
        }
    }
    if (isPrime == 1) {
        return 1;
    } else {
        return 0;
    }
}

int isPrime2(int x){  // man sqrt　　　　　　　素数的优化方案，　　
    int ret = 1;
    int i;
    if(x ==1 || x % 2 == 0 && x != 2){
        ret = 0;
    }
//    for(i = 3 ; i < sqrt(x) ; i +=2){ //sqrt 平方根
//        if(x % i == 0){
//            ret = 0;
//            break;
//        }
//    }
    return ret;
}

////     char c = 'c';
////     if(c >='A'&&c <='Z')

//        a == 6 && b == 1  对于&&，左边为false，则不会做右边的运算
//        a == 6 && b += 1  对于||,左边为true，则不会做右边的运算
//         不要把表达式写进去
/**
 * 凑硬币
 */
void moneyDivider(int total) {
    int one, two, five;
    int exit = 0;
    for (one = 1; one <= total * 10; one++) {
        for (two = 1; two <= total * 10 / 2; two++) {
            for (five = 1; five <= total * 10 / 5; five++) {
                if (one * 1 + two * 2 + five * 5 == total * 10) {
                    printf("可以用%d张一毛，%d张两毛和%d张五毛的凑成２块钱\n", one, two, five);
                    exit = 1;
                    break;
//                     goto out;  //尽量只在跳出多重循环时使用goto.  out为自定义的标号
                }
            }
            if (exit) break;
        }
        if (exit)break;
    }
//    out:return 0;
}

void aboveAvrange(){//输出所有大于平均数的数．存在安全隐患，有没有可能大于数组下标100
    int number[100];//不可以int[100] a 这样定义
    //数组大小确定后不可改变
    //数组的单元在内存内部是依次排列的
    int x;
    int cnt = 0;
    double sum = 0;
    printf("shuzu 请输入数(直到-1停止):\n");
    scanf("%d",&x);
    while(x != -1){
        number[cnt] = x;
        sum += x;
        cnt ++;
        scanf("%d",&x);
    }
    if(cnt > 0){
        double averange = sum/cnt;
        printf("平均数：%f\n",averange);
        int i;
        for(i = 0; i < cnt; i++){
//            printf("number[%d] = %d \n",i,number[i]);
            if(number[i] > averange) {
                printf("%d\n", number[i]);
            }
        }
    }
}


/**
 * 计算输入的0-9以内的数字有多少个　
 */
void numCount(){
    const int NUMBER = 10;
    int i,x;
    int count[NUMBER];
    for(i = 0 ; i < NUMBER ; i ++){
        count[i] = 0;
    }
    printf("请输入０－９以内的数字，以－１结束，该程序统计个数字个数:\n");
    scanf("%d",&x);
    while( x != -1){
        if(x <= 9 && x >= 0){
            count[x] ++;
        }
        scanf("%d",&x);
    }
    for(i = 0 ; i < NUMBER; i ++){
        printf("数字%d出现的次数为%d次\n",i,count[i]);
    }
}

int search(int key,int a[],int length){
    int ret = -1;
    int i;
    for(i = 0 ; i < length ; i ++){
        if(a[i] == key){
            ret = i;
            break;
        }
    }
    return  ret;
}

void chessGame(){//0 为 O 1为　Ｘ
    printf("矩阵棋盘游戏3*3，请输入棋盘情况:\n");
    const int SIZE = 3;
    int result,numofO,numofX;
    int board[SIZE][SIZE];
    for(int i = 0; i < SIZE; i++){//输入棋盘情况
        for(int j = 0; j < SIZE; j++){
            scanf("%d",&board[i][j]);
        }
    }
    {
        printf("您输入的棋盘为:\n");
        for(int i = 0; i < SIZE; i++){//输入棋盘情况
            for(int j = 0; j < SIZE; j++){
                printf("%d ",board[i][j]);
                if(j == SIZE -1)
                    printf("\n");
            }
        }
    }

    //检查获胜　step1 检查行
    for(int i = 0; i < SIZE && result == -1; i++){
        numofO = numofX = 0;//初始化
        for(int j = 0 ; j < SIZE; j++){
            if(board[i][j] == 1){
                numofX ++;
            }else{
                numofO ++;
            }
        }if(numofO == SIZE){
            result = 0;
        }else if(numofX == SIZE){
            result = 1;
        }
    }
    //step 2检查列
    for(int j = 0 ; j < SIZE ; j++){
        numofO = numofX = 0;
        for(int i = 0 ; i <SIZE ; i++){
            if(board[i][j] == 1){
                numofX ++;
            }else{
                numofO ++;
            }
        }
    }if(numofO == SIZE){
        result = 0;
    }else if(numofX == SIZE){
        result = 1;
    }

    //step 3检查斜线
    numofO = numofX = 0;
    for(int i = 0; i < SIZE ; i++){
        if(board[i][i] == 1){
            numofX ++;
        }else{
            numofO ++;
        }
    }
    if(numofO == SIZE){
        result = 0;
    }else if(numofX == SIZE){
        result = 1;
    }
    numofO = numofX = 0;
    for(int i = 0; i < SIZE ; i ++){
        if(board[i][SIZE-i-1] == 1){
            numofX ++;
        }else{
            numofO ++;
        }
    }
    if(numofO == SIZE){
        result = 0;
    }else if(numofX == SIZE){
        result = 1;
    }
    if(result == 1){
        printf("１获胜\n");
    }else
        printf("0 获胜\n");
}

void address(){
    int a;
    a = 6;
    printf("sizeof(int)=%ld\n", sizeof(int));//sizeof是静态的，比如sizeof(a++)  a++不会进行运算
    printf("sizeof(double)=%ld\n",sizeof(double));//double的大小是int的两倍
    printf("sizeof(a)=%ld\n", sizeof(a));
//    scanf("%d",&a);//&取得变量的地址，＆的操作数必须是一个变量　　＆（a+b）　＆（a++） &(a--)   X  不可以!
    int i = 0;
    int m;
    int n;
//    printf("0x%x\n",p);//64为下不一样
    printf("%p\n",&i);
    printf("%lu\n", sizeof(int));
    printf("%lu\n", sizeof(&i));//64位下为８个字节　，３２为４个

    printf("%p\n",&m);
    printf("%p\n",&n);//变量的定义在地址中是紧挨着的，自顶向下定义,m n地址相差一个int的大小
    printf("----------------------------------------------------\n");
    int b[10];
    printf("%p\n",&b);
    printf("%p\n",b);
    printf("%p\n",&b[0]);//数组的地址为第一个下标的元素的地址．每个元素相差４
    printf("%p\n",&b[1]);
    printf("----------------------------------------------------\n");
    //指针类型的变量，保存地址的变量
//    {
//        int c;
//        int* v = &c ;  //v保存了变量c的地址，这里叫做　v指向了c
//        int* t,y;//这两句的意思完全一样，t为指针，y为int
//        int *t,y;
//
//    }
}

void pmethod(int* p){
    printf(" p=%p\n",p);
    int k = *p;  //对指针做＊运算，可以访问存储在该地值上的变量的值
    printf("*p=%d,k=%d\n",*p,k);
    *p = 26;//对外部变量做了值的修改
}

void minmax(int a[],int length,int* min,int* max){ //参数表中的int a[] 也可以写成 int *a
    //数组本身是特殊的指针，int a[10];int *p =a; 但是数组的单元表达的是变量，需要用＆取地址　a==&a[0] p[0] <==>a[0]
    int i;
    a[0] = 1000;
//    printf("minmax sizeof(a)=%lu\n", sizeof(a));//函数中的传入的数组实际上是指向a[0]的一个指针
    *min = *max = a[0];
    for(i = 1; i < length ; i++){
        if(a[i] < *min){
            *min = a[i];
        }
        if(a[i] > *max){
            *max = a[i];
        }
    }
}

void string(){
    char word[8];
    scanf("%7s",word);
    printf("%7s##\n",word);// %s 读入一个单词（到空格tab或回车为止） scanf不安全，没有定义输入长度 %s中间可以加数字限制输入长度　８－１＝７最多为7，因为空格
    char buffer[100] = "";
    //空字符串　buffer[0] == '\0'
//    char buffer[] = "";//这个数组的长度只有１！！！！！！！！！！！！！！！！！
}

void stringhmethod(){
    char buffer[]="hello";
    //strlen
    printf("strlen = %lu\n",strlen(buffer)); //strlen 计算字符串长度，不包含末尾的0
    printf("sizeof(buffer)=%lu\n", sizeof(buffer));

//    strcmp(s1,s2) 比较两个字符串　返回0 相等，１ s1>s2, -1 s1<s2
    char s1[] = "abc";
    char s2[] = "abc";
    printf("%d\n",strcmp(s1,s2));
    char s3[] = "bbc";
    printf("%d\n",strcmp(s1,s3));
    //stcpy(s1,s2) 把s2的字符串内容，拷贝到s1的空间去
    //strcat(s1,s2)　字符串连接，把s2拷贝到s1后面，返回s1,s1必须有足够的空间
    //这两个函数不安全，以下版本较为安全
//    strcpy(s1,s2,n)能够拷贝最多n个字符
//    strcat(s1,s2,n)能够脸上最多n个字符
//    strcmp(s1,s2,n)前n个字符比较
}


void zhizhenandconst(const int *qwe){//判断哪个被const的标志是看const在*的前面还是后面 ,参数表中的const int可以传入非const指针，但是该函数可以保证不变动该指针
    //const在*后面，表示指针不能修改，const在前面则表示非指针的整体不能修改
    int i = 0;
    int j = 1;
    int * const q = &i;//q为const,q的值即i的地址不能改变
    *q = 26;//ok 没问题
//    q++;//no　不能做

    const int *p = &i;
//    *p = 26//no 不能变因为int 　　（*p）是　const
    i =26;p=&j;//ok没问题
//    void f(const int* x){
//        int a = 15;
//        f(&a);//ok
//        const int b = a;
//        f(&b);//ok
//        b = a + 1;//error
//    }
    const int a[] = {1,2,3,4,5,6,};//全部都不能改变
}

void swap_new(int *a , int *b){
    int t = *a;
    *a = *b;
    *b = t;
}//此swap函数就可以使用，值通过指针带回

int divide(int a, int b, int *result){//指针的一个作用，使函数返回多个结果。函数本身可以有返回值，结果也可以从指针带回，返回０表明无法作除法。
    int ret = 1;
    if(b == 0) ret = 0;
    else{
        *result = a/b;
        return ret;
    }
}

void mostErrorAboutZhizhen(){
    int *p;
    int k;
    k = 12;
    *p = 12;//error p还没有指向任何地方，无法被赋值。
}

void zhengshufanwei(){
    /**
     * -2^n-1 ~ 2^n-1 -1
     * char 1个字节　-128~127 00000000
     * short 2个字节　-32768~32767 00000000 00000000
     * int  取决于编译器(CPU),通常的意义是　一个字　４个或８个　-2^32-1 ~ 2^32-1 -1
     * long　４字节
     * long long　８字节
     */
     unsigned char c = 255;//unsigned　让正数部分扩大一倍，但是无法表达负数　　若一个字面量想表达自己是unsigned　可以加u或Ｕ ：　255u/255U
//     char c = 127, c = c + 1 , c = -128
}

void zhengshudeInandOut(){
//    %u unsigned char = -1
    char c = -1;
    printf("%u",c);//c = max
}

void floatfanwei(){
//    float 32字长　７个有效数字　scanf %f printf %f,%e   %e为科学计数法输出　　　在%和f之间加上.n表达输出小数点后n位，是四舍五入的
//    double 64字长　１５个有效数字 scanf %lf printf %f,%e
//inf表示无穷大　-inf 表示负无穷大
  float a,b,c;
  a = 1.235f;
  b = 2.123f;
    //　f1 == f2 可能失败
//   fabs(f1-f2) < 1e-12; fabs求绝对值，小于一个很小的值
}

void bool(){
//#include <stdbool.h>才可以用bool类型，Ｃ中没有bool，true即为１
//逻辑短路
// a ==b && c == d 对于&& 只要左边为0,右边不做计算
//a == b || a+1 == 2 对于　||　只要左边为１，右边不做计算
}

void Pjisuan(){
    char ac[]={1,2,3,4,5,6,7,8,9,-1};
    char *p = ac;
    char *p1 = &ac[5];
    printf("%p\n",p);
    printf("p+1=%p\n",p+1);
    printf("*(p+1)=%d\n",*(p+1));
    printf("p1-p=%d\n",p1-p);
//    *(p+n) <--> a[n] , p++移动到下一个位置上去
    int ai[]={1,2,3,4,5,6,7,8,9};
    int *q = ai;
    int *q1 = &ai[6];
    printf("%p\n",q);
    printf("q+1=%p\n",q+1);//指针的加减每次移动一个sizeof(类型)的距离，所以int是４，char是１
    printf("*(q+1)=%d\n",*(q+1));
    printf("q1-q=%d\n",q1-q);//指针减法，得到的值是这两个地址之间的差除以sizeof(类型)
//    *p++, ++优先级高
//    取出p所指的数据来，完事之后顺便把p移动到下一个位置去,常用于数组之类的连续空间的操作，像遍历
    for(int i = 0 ; i < sizeof(ac)/ sizeof(ac[0]);i++){
        printf("%d\n",ac[i]);
    }
//    for(p = ac ; *p != -1 ; p ++){
//        printf("%d\n",*p);
//    }与下等价
    while(*p != -1){
        printf("%d\n",*p++);
    }
    //<,<=...都可以对指针做，比较的是地址大小，数组中的地址永远是线性的
    // 每个程序都有0地址，通常不能随便碰的地址，指针不应该具有0值，可用0地址做特殊的事件:返回的指针是无效的／指针没有被初始化（先初始化为0）NULL表示0地址
    //无论指向什么类型，指针的大小都是一样的，都是指向地址，指向不同类型的指针不能相互赋值　p = q.(char * int *)不行
    //void*　表示不知道指向什么东西的指针
    int x;
    int *c = &x;
    void *d = (void*)c;//可以，没有改变，指示把c看成void
}

void dynomicStorage(){//malloc free    void* malloc(size_t size); 向malloc申请的大小是以字节为单位的，返回的结果需要强制类型转换
    //动态内存分配
    //如果空间不够，申请失败，则返回0,叫做NULL
    int* a = 0;
    int number ;
    int i;
    printf("请输入数量　:");
    scanf("%d",&number);
    a =(int*) malloc(number* sizeof(int));//需要n个int的大小的内存,申请之后，之后完全可以把a当成一个数组来确定
    for(i = 0; i < number; i++){
        scanf("%d",&a[i]);
    }
    for(i = number-1;i>=0;i--){
        printf("%d\t",a[i]);
    }
    free(a);//释放内存空间
    free(NULL);//没问题

    //常见的free错误！！！！！！！！！！！！！
    //忘记free,找不到合适的时机free,free过了，地址变化了直接free


    //申请的都要还
    //只能还申请来的空间的首地址
   /**
        *int *p = malloc(100*1024*1024);
        p ++;
        free(p);// X!!!!!!!!!!,p的值已经改变

    */

//    void *p;
//    int cnt = 0;
//    while( (p = malloc(100 * 1024 * 1024)) ){ //p为０时，申请失败
//        cnt++;
//    }
//    printf("分配了%d00MB的空间\n",cnt);
}

void stringshuzu(){
    //char **a; a是一个指针，指向另一个指针，那个指针指向一个字符（串）
    //char a[][n]　,第二维一定要有一个具体的数
    char a[][10] = {"Hello"};
    char *b[]={"Hello" };//可以表达二维数组，不同于a[][10]　b中每一个单元都是指针，指向其他的字符串
//    b[0] <--> char *
//    int main(int argc,char const *argv[])
}

void structP(struct data *p){  //(*p).month = 12  即 指针所指向的结构体中的成员month = 12，即简写成　p->month
    scanf("%d",&p->year);
    scanf("%d",&p->month);
    scanf("%d",&p->day);
}
void output(const struct data *p){
    printf("%d-",p->year);
    printf("%d-",p->month);
    printf("%d\n",p->day);
}

void structInstruct() {//结构中嵌套结构
    struct rectangle rec1,*rp;
    rp = &rec1;
    rec1.p1.x = 1;
    rec1.p1.y = 2;
    rec1.p2.x = 3;
    rec1.p2.y = 4;
    //那么一下四个写法全都正确
//    rec1.p1.x
//    rp->p1.x
//    (rec1.p1).x
//    (rp->p1).x

//但是没有rp->p1->x ,因为p1不是指针！
}

void uniontest(){
    CHI chi;
    int i ;
    chi.i = 1234;
    for(int i = 0; i < sizeof(int);i++){
        printf("%02hhX\n",chi.ch[i]);
    }//输出D2040000 本身int 1234　为　000004D2  因为在内存中低位在前
}

void staticValue(){
    static int all = 1;
    printf("all = %d\n",all);
    all += 2;
    printf("agn all = %d\n",all);
}

void defineuse(){
    PRT;
    printf("%s:%d\n",__FILE__,__LINE__);//预定义的宏，文件名，行号，编译时的日期，时间
    printf("%s,%s\n",__DATE__,__TIME__);
    printf("%d\n",cube(5));
    printf("%f\n",RADTODEG1(5+2));
    printf("%f\n",180/RADTODEG2(1));//实际上运算的是　180/1 * 57.29578
}

void printscanf(){//%[flags][width][.prec][hlL]type
    //flag == (space) 正数留空
    //flag == 0，０填充
    printf("%+9d\n",123);//  占据９个字符的控件　，默认右对其　　输出：　　+123
    printf("%-9d\n",123);   //左对齐-　　。＋就是+号 输出123
    printf("%09d\n",123);
    printf("%-09d\n",123);
    printf("%9.2f\n",123.0);//[width] or [.prec]　　m.n表示占据m位的宽度。小数点后n位
    printf("%*d\n",6,123);//*的意思是下一个参数是字符数，占据6个位置，格式灵活调节
    int num;
    printf("%hhd%n\n",12345,&num);//[hlL] hh 单个字节(12345:57)   h short l long  ll longlong L longDouble
    printf("num = %d\n",num);//这个num以上用法，用来记录已经输出的位数
    scanf("%*d%d",&num);//*跳过这个整数的输入，存入第二次输入的整数
    printf("num = %d\n",num);
    //scanf如果用%i来读取，那么比%d更加灵活，会自动识别0x12,012,等8进制和１６进制的数并转换成Int来保存
    //[^.][^,]到, .为止
    //printf和scanf的返回值是输入输出的个数！！printf多一个回车换行符
    // < > 文件的重定向，< 12.in 表示从１２这个文件夹读取作为函数的输入，   ./a.out　> 12.out　表示结果输出到12.out之中

}

void fileOpen(){//打开文件的标准代码
    FILE *fp = fopen("12.in","r");//文件名，r 读 r+读写,从文件头开始  w 写,不存在则创建，存在则清空　
    // w+打开读写　a　追加，不存在新建，存在从文键尾开始　追加..x如rx,wx。只新建，若存在则不能打开，为的是不破环源文件
    int num;
    if(fp){
        fscanf(fp,"%d",&num);
        printf("%d\n",num);
        fclose(fp);
    }else{
        printf("文件无法打开！");
    }
}

void Pmethod2(){//C语言中每个函数都会得到一个地址
    /*
     * 比如回调函数，acllib.h中有一个注册函数registerMouseEvent(mouseListener);它需要的就是一个指向函数的指针，会在鼠标移动时调用MouseListener函数
     * 只需要在程序中实现mouseListener函数即可
     * void mouserListener(int x,int y,int button,int event){  ...}
     *
     */
    int i;
    int* p = &i;
    printf("f() = %p\n",f);
    void(*pf)(void) = f;//pf是　一个指向函数的指针，直接交付函数名字！
    //具体使用场景，比如根据用户的输入来确定调用的函数，我们可以使用void(*fa[])(int ) = {f,g,h}来把需要调用的函数放进一个指向函数指针的数组中
    //scanf("%i",&i)）;
    //if(i >=0 && i <sizeof(fa)/sizeof(fa[0])){     (*fa[i])[0];    }，[0]为参数，调用不同的方法
    printf("pf = %p\n",pf);
    f();
    (*pf)();
    cal(plus);//使用指向函数的指针作为参数，根据输入的参数来确定具体调用的函数是哪一个
    cal(minus);
}

void f(){
    printf("in f()\n");
}

void cal(int (*h)(int,int)) {//使用指向函数的指针作为参数，根据输入的参数来确定具体调用的函数是哪一个
    printf("%d",(*h)(2,3));
    printf("\n");
}
int  plus(int a,int b){
    return a+b;
}
int minus(int a,int b){
    return a-b;
}
