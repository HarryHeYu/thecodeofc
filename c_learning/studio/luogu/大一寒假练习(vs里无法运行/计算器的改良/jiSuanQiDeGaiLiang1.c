#include<stdio.h>
#include<math.h>
#include<string.h>
#include<ctype.h>
#define f(i, a, b) for(register int i = a; i <= b; ++i)
//这是一个宏定义，将 f(i, a, b) 替换为 for(register int i = a; i <= b; ++i)，方便后续使用 for 循环。
/*register 是一个存储类关键字，用于建议编译器将变量存储在寄存器中。寄存器是CPU内部的高速存储单元，访问寄存器的速度通常比访问内存快得多
。因此，使用 register 关键字可以提高程序的性能，尤其是在循环或频繁访问某个变量的情况下。*/
void paraseEquation(const char* s, int* son, int *mother, char* ansx);
/*s：输入的方程字符串。 son：指向存储常数项的变量的指针。son：指向存储常数项的变量的指针。 
mother：指向存储未知数系数的变量的指针。ansx：指向存储未知数的字符变量的指针。 */
void parseEquation(const char *s, int *son, int *mother, char *ansx) {
    int len = strlen(s + 1);
    char temp[100010];
    strcpy(temp + 1, s + 1);//将输入的字符串复制到temp中
    temp[0] = '+';  // 为了方便处理，在开头添加一个符号
    temp[len + 1] = '+';  // 随便一个符号都可以
    int flag, havenum;//flag用于标记当前处理的项是常数项
    int pre, lor = 1, equal;
    //pre记录上一个运算符的位置，lor用于区分方程等式左右两边，equal用于记录等号的位置
    pre = 0;
    f(i, 1, len + 1) {
        if (temp[i] >= 'a' && temp[i] <= 'z')
            *ansx = temp[i];//如果遇到字母就将其存在ansx中
        if (temp[i] == '=' || temp[i] == '-' || temp[i] == '+') {
            // 当遇到 '='、'-' 或者 '+' 这些运算符时，开始处理当前的数字部分
            havenum = 0;//用于标记是否存在数字，初始化为 0 表示还未发现数字
            if (i == 1 || i - 1 == equal) {
                pre = i;
                continue;
                /*如果 i == 1，表示当前是字符串的第一个字符，直接更新 pre 为当前字符的索引，然后跳过本次循环后续操作，进入下一次循环。
                如果 i - 1 == equal，说明前一个字符是等号，同样更新 pre 并跳过后续操作。*/
            }
            int j, x = 0;//定义循环变量 j 和整数变量 x，x 用于存储从上个运算符到当前运算符之间提取出来的数字，初始化为 0。
            if (isdigit(temp[i - 1]))
                flag = 0;//是数字就标记为0，为常数项
            else
                flag = 1;//不是数字就标记为1，是未知数项
            for (j = pre + 1; j <= i - 1 - flag; ++j) {
                //从上个运算符的下一个位置（pre + 1）开始，到当前运算符前一个位置（i - 1）减去 flag 结束进行遍历。
                x = (x << 1) + (x << 3) + (temp[j] - '0');
                //相当于x*2+x*8+...   本质是x*10，只是用二进制来转换计算更快
                havenum = 1;
            }
            if (havenum == 0)
                x = 1;//将x设为1，例如方程中的a可以看做1*a
            if (flag == 0)
                *son += x * lor * -1 * (temp[pre] - 44) * -1;
            else
                *mother += x * lor * (temp[pre] - 44) * -1;//temp[pre] 表示上一个运算符，+ 的 ASCII 码值是 43，- 的 ASCII 码值是 45。
                //*-1这是为了调整符号，与前面的 (temp[pre] - 44) 配合，正确处理各项的正负号。
            pre = i;//更新索引
            if (temp[i] == '=') {
                lor = -1;
                temp[i] = '+';
                equal = i;
            }
        }
    }
}
double solveEquation(int son, int mother) {
    double ansy = (double)son / (double)mother;
    if (fabs(ansy) - 0.0 < 0.000001)//判断是否为0
        ansy = 0.0;
    return ansy;
}
int main() {
    char s[100010];
    int son = 0, mother = 0;
    char ansx;
    scanf("%s", s + 1);
    // 解析方程
    parseEquation(s, &son, &mother, &ansx);
    // 求解方程
    double ansy = solveEquation(son, mother);
    // 输出结果
    printf("%c=%.3lf", ansx, ansy);
    return 0;
}
/*# [NOIP 2000 普及组] 计算器的改良
## 题目背景
NCL 是一家专门从事计算器改良与升级的实验室，最近该实验室收到了某公司所委托的一个任务：
需要在该公司某型号的计算器上加上解一元一次方程的功能。实验室将这个任务交给了一个刚进入的新手 ZL 先生。
## 题目描述
为了很好的完成这个任务，ZL 先生首先研究了一些一元一次方程的实例：
- $4+3x=8$。
- $6a-5+1=2-2a$。
- $-5+12y=0$。
ZL 先生被主管告之，在计算器上键入的一个一元一次方程中，只包含整数、小写字母及 `+`、`-`、`=` 
这三个数学符号（当然，符号“`-`”既可作减号，也可作负号）。方程中并没有括号，也没有除号，方程中的字母表示未知数。
你可假设对键入的方程的正确性的判断是由另一个程序员在做，或者说可认为键入的一元一次方程均为合法的，且有唯一实数解。
## 输入格式
一个一元一次方程。
## 输出格式
解方程的结果（精确至小数点后三位）。
## 样例 #1
### 样例输入 #1
```
6a-5+1=2-2a
```
### 样例输出 #1
```
a=0.750
```*/