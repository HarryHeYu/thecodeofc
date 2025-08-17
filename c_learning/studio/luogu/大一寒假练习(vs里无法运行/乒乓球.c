#include<stdio.h>
#include<string.h>
void calculateAndPrintResults(char* record, int gamePoints){
    int scoreA = 0, scoreB = 0;
    for(int i = 0; record[i] != '\0'; i++){
        if(record[i] == 'W'){
            scoreA++;
        }else if(record[i] == 'L'){
            scoreB++;
        }
        //判断依据是否结束
        if((scoreA >= gamePoints || scoreB >= gamePoints) && (scoreA - scoreB >= 2 || scoreB - scoreA >= 2)){
            printf("%d:%d\n",scoreA, scoreB);
            scoreA = 0;
            scoreB = 0;
        }
    }
    //输出最后一局的比分
    printf("%d:%d\n",scoreA, scoreB);
}
int main(){
    char record[62501] = "";
    char line[26];
    //定义一个长度为 26 的字符数组 line，用于临时存储每次输入的一行字符串，因为每行至多 25 个字母，再加上字符串结束符 '\0'，所以长度为 26。
    //读取输入直到遇到'E'
    while(1){
        scanf("%s",line);
        if(strchr(line, 'E') != NULL){
            strncat(record, line, strchr(line, 'E') - line);
//strncat是比strcat更安全的字符串拼接函数，第三个参数是拼接的长度
// char *strncat(char *dest, const char *src, size_t n);
            break;
        }
        strcat(record, line);
    }
    //计算并输出11分制的结果
    calculateAndPrintResults(record, 11);
    //输出空行
    printf("\n");
    //计算并输出21分制的结果
    calculateAndPrintResults(record, 21);
    return 0;
}
/*# P1042 [NOIP 2003 普及组] 乒乓球
## 题目背景
国际乒联现在主席沙拉拉自从上任以来就立志于推行一系列改革，以推动乒乓球运动在全球的普及。其中 $11$ 分制改革引起了很大的争议，
有一部分球员因为无法适应新规则只能选择退役。华华就是其中一位，
他退役之后走上了乒乓球研究工作，意图弄明白 $11$ 分制和 $21$ 分制对选手的不同影响。
在开展他的研究之前，他首先需要对他多年比赛的统计数据进行一些分析，所以需要你的帮忙。
## 题目描述
华华通过以下方式进行分析，首先将比赛每个球的胜负列成一张表，然后分别计算在 $11$ 分制和 $21$ 分制下，双方的比赛结果（截至记录末尾）。
比如现在有这么一份记录，（其中 $\texttt W$ 表示华华获得一分，$\texttt L$ 表示华华对手获得一分）：
$\texttt{WWWWWWWWWWWWWWWWWWWWWWLW}$
在 $11$ 分制下，此时比赛的结果是华华第一局 $11$ 比 $0$ 获胜，第二局 $11$ 比 $0$ 获胜，正在进行第三局，当前比分 $1$ 比 $1$。
而在 $21$ 分制下，此时比赛结果是华华第一局 $21$ 比 $0$ 获胜，
正在进行第二局，比分 $2$ 比 $1$。如果一局比赛刚开始，则此时比分为 $0$ 比 $0$。直到分差大于或者等于 $2$，才一局结束。
**注意：当一局比赛结束后，下一局立刻开始**。
你的程序就是要对于一系列比赛信息的输入（$\texttt{WL}$ 形式），输出正确的结果。
## 输入格式
每个输入文件包含若干行字符串，字符串由大写的 $\texttt W$ 、 $\texttt L$ 和 $\texttt E$ 组成。
其中 $\texttt E$ 表示比赛信息结束，程序应该忽略 $\texttt E$ 之后的所有内容。
## 输出格式
输出由两部分组成，每部分有若干行，每一行对应一局比赛的比分（按比赛信息输入顺序）。其中第一部分是 $11$ 分制下的结果，第二部分是 $21$ 分制下的结果，两部分之间由一个空行分隔。
## 输入输出样例 #1
### 输入 #1
```
WWWWWWWWWWWWWWWWWWWW
WWLWE
```
### 输出 #1
```
11:0
11:0
1:1

21:0
2:1
```
## 说明/提示
每行至多 $25$ 个字母，最多有 $2500$ 行。
（注：事实上有一个测试点有 $2501$ 行数据。）*/