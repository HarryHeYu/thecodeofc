#include<stdio.h>
#include<stdbool.h>
bool isValid(int num, bool used[]);
// 判断一个数字是否有效
bool isValid(int num, bool used[]){
    // 循环遍历数字的每一位
    while(num > 0){
        // 获取当前位的数字
        int digit = num % 10;
        // 如果当前位为0或者已经使用过，则返回false
        if(digit == 0 || used[digit])return false;
        // 标记当前位已经使用过
        used[digit] = true;
        // 去掉当前位，继续判断下一位
        num /= 10;
    }
    // 如果所有位都有效，则返回true
    return true;
}

int main(){
    for(int first = 123; first <= 333; first++){
        //第一个数字的最大值应为 333，因为 333 × 3 = 999，仍然在三位数范围内
        int second = first * 2;
        int third = first *3;
        if(second > 999 || third > 999)continue;
        bool used[10] = {false};
        if(!isValid(first, used))continue;
        if(!isValid(second, used))continue;
        if(!isValid(third, used))continue;
        printf("%d %d %d\n",first, second, third);
    }
    return 0;
}
/*# [NOIP1998 普及组] 三连击
## 题目背景
本题为提交答案题，您可以写程序或手算在本机上算出答案后，直接提交答案文本，也可提交答案生成程序。
## 题目描述
将 $1, 2, \ldots , 9$ 共 $9$ 个数分成 $3$ 组，分别组成 $3$ 个三位数，且使这 $3$ 个三位数构成 $1 : 2 : 3$ 的比例，试求出所有满足条件的 $3$ 个三位数。
## 输入格式
无
## 输出格式
若干行，每行 $3$ 个数字。按照每行第 $1$ 个数字升序排列。
## 样例 #1
### 样例输入 #1
```
无
```
### 样例输出 #1
```
192 384 576
...
（剩余部分不予展示）
```*/