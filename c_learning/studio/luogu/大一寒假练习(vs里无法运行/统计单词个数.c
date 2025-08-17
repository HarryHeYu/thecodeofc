#include<stdio.h>
#include<string.h>
#define MAX_LEN 201//字母串最大长度
#define MAX_WORDS 7//字典中单词最大数量
#define MAX_PARTS 41//最大划分数
char str[MAX_LEN];//存储输入的字母串
char words[MAX_WORDS][MAX_LEN];//存储字典中的单词
int wordCount[MAX_LEN][MAX_LEN];//wordCount[i][j]表示第i个字符到第j个字符组成的子串中包含的单词个数;用于记录每个子串包含的单词个数
int dp[MAX_LEN][MAX_PARTS];//dp[i][j]表示将前i个字符分成j份时包含的最大单词个数;是动态规划过程中记录状态的关键数组。
//检查从start开始的子串是否包含某个单词
int containsWord(int start, int end);
void preprocess(int len);
int containsWord(int start, int end){
    int len = end - start + 1;
    for(int i = 0; words[i][0] != '\0'; i++){
        int wordLen = strlen(words[i]);
        if(len >= wordLen && strncmp(str + start, words[i], wordLen) == 0)return 1;
//使用 strncmp 函数比较子串和单词是否匹配，比较的长度为 wordLen。如果子串长度大于等于单词长度且匹配成功，则返回 1，表示包含该单词；否则继续检查下一个单词。
    }
    return 0;
}
//预处理每个子串中包含的单词个数
/*使用三层嵌套的 for 循环来遍历所有可能的子串。外层循环 i 表示子串的起始位置，中层循环 j 表示子串的结束位置，内层循环 k 用于在子串内移动检查。
对于每个子串 [i, j]，调用 containsWord 函数检查从 k 到 j 的子串是否包含字典中的单词。如果包含，则 wordCount[i][j] 的值加 1。
通过这种方式，我们可以得到所有子串包含的单词个数，为后续的动态规划计算提供基础。*/
void preprocess(int len){
    for(int i = 0; i < len; i++){
        for(int j = i; j < len; j++){
            for(int k = i; k <= j; k++){
                if(containsWord(k, j))wordCount[i][j]++;
            }
        }
    }
}
int main(){
    int p, k;//p表示字母串的行数,k表示划分的份数
    scanf("%d %d",&p, &k);
    //读取字母串
    int len = 0;
    for(int i = 0; i < p; i++){
        char line[21];
        scanf("%s",line);
        strcpy(str + len, line);//复制字符串
        len += 20;
    }
    str[len] = '\0';
    //读取字典中的单词
    int s;
    scanf("%d",&s);
    for(int i = 0; i < s; i++){
        scanf("%s",words[i]);
    }
    words[s][0] = '\0';//方便后续遍历字典
//这相当于在存储了实际单词的最后一行后面添加了一个空字符串作为结束标志。
    preprocess(len);//预处理每个子串中包含的单词个数
    //动态规划初始化
    for(int i = 0; i < len; i++){
        dp[i][1] = wordCount[0][i];
//初始化 dp 数组，将前 i 个字符分成 1 份的最大单词个数就是从第 0 个到第 i 个字符组成的子串包含的单词个数。
    }
    //动态规划计算
//状态转移方程：dp[i][j] = max(dp[m][j - 1] + wordCount[m + 1][i])，其中 j - 2 <= m < i。
    for(int j = 2; j <= k; j++){
        for(int i =j - 1; i < len; i++){
            dp[i][j] = 0;//初始化当前状态
            for(int m = j - 2; m < i; m++){//要将前 i 个字符分成 j 份，我们可以枚举所有可能的分割点 m（j - 2 <= m < i）。
                int temp = dp[m][j - 1] + wordCount[m + 1][i];
/*将前 i 个字符以 m 为分割点分成两部分后：
前 m 个字符分成 j - 1 份的最大单词个数是 dp[m][j - 1]。
从第 m + 1 个字符到第 i 个字符这部分子串包含的单词个数是 wordCount[m + 1][i]。
那么，将前 i 个字符分成 j 份且以 m 为分割点时，总的单词个数就是这两部分的单词个数之和，即 dp[m][j - 1] + wordCount[m + 1][i]。*/
                if(temp > dp[i][j])dp[i][j] = temp;
            }
        }
    }
    printf("%d\n",dp[len - 1][k]);
    return 0;
}
/*# P1026 [NOIP 2001 提高组] 统计单词个数
## 题目描述
给出一个长度不超过 $200$ 的由小写英文字母组成的字母串（该字串以每行 $20$ 个字母的方式输入，且保证每行一定为 $20$ 个）。要求将此字母串分成
 $k$ 份，且每份中包含的单词个数加起来总数最大。  
每份中包含的单词可以部分重叠。当选用一个单词之后，其第一个字母不能再用。例如字符串 `this` 中可包含 `this` 和 `is`，选用 `this` 之后就不能包含
 `th`。
单词在给出的一个不超过 $6$ 个单词的字典中。
要求输出最大的个数。
## 输入格式
每组的第一行有两个正整数 $p,k$。
$p$ 表示字串的行数，$k$ 表示分为 $k$ 个部分。
接下来的 $p$ 行，每行均有 $20$ 个字符。
再接下来有一个正整数 $s$，表示字典中单词个数。
接下来的 $s$ 行，每行均有一个单词。
## 输出格式
$1$个整数，分别对应每组测试数据的相应结果。
## 输入输出样例 #1
### 输入 #1
```
1 3
thisisabookyouareaoh
4
is
a
ok
sab
```
### 输出 #1
```
7
```
## 说明/提示
【数据范围】  
对于 $100\%$ 的数据，$2 \le k \le 40$，$1 \le s \le 6$。
【样例解释】
划分方案为 this / isabookyoua / reaoh
**【题目来源】**
NOIP 2001 提高组第三题*/