#include <stdio.h>
#include <string.h>

// 将整数转换为给定进制的字符串
void intToBase(int num, int base, char result[]) {
    int index = 0;
    while (num > 0) {
        int digit = num % base;
        result[index++] = (digit < 10) ? (digit + '0') : (digit - 10 + 'A');
        num /= base;
    }
    result[index] = '\0';
    reverse(result, index);
}

// 检查是否为回文数
int isPalindrome(char num[], int len) {
    for (int i = 0; i < len / 2; i++) {
        if (num[i] != num[len - i - 1]) {
            return 0;
        }
    }
    return 1;
}

// 将字符串反转
void reverse(char num[], int len) {
    for (int i = 0; i < len / 2; i++) {
        char temp = num[i];
        num[i] = num[len - i - 1];
        num[len - i - 1] = temp;
    }
}

// 将两个数字字符串相加
int add(char num1[], char num2[], char result[], int base) {
    int carry = 0;
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    int maxLen = len1 > len2 ? len1 : len2;
    int i = 0;
    
    for (i = 0; i < maxLen || carry; i++) {
        int digit1 = i < len1 ? (num1[len1 - i - 1] >= 'A' ? num1[len1 - i - 1] - 'A' + 10 : num1[len1 - i - 1] - '0') : 0;
        int digit2 = i < len2 ? (num2[len2 - i - 1] >= 'A' ? num2[len2 - i - 1] - 'A' + 10 : num2[len2 - i - 1] - '0') : 0;
        int sum = digit1 + digit2 + carry;
        carry = sum / base;
        sum %= base;
        result[i] = sum < 10 ? sum + '0' : sum - 10 + 'A';
    }
    result[i] = '\0';
    reverse(result, i);
    return i;
}

int main() {
    int n;
    char m[101];
    scanf("%d", &n);
    scanf("%s", m);
    
    for (int step = 0; step <= 30; step++) {
        int len = strlen(m);
        if (isPalindrome(m, len)) {
            printf("STEP=%d\n", step);
            return 0;
        }
        
        char reversed[101];
        strcpy(reversed, m);
        reverse(reversed, len);
        
        char result[102];
        int resultLen = add(m, reversed, result, n);
        strcpy(m, result);
    }
    
    printf("Impossible!\n");
    return 0;
}
/*# [NOIP 1999 普及组] 回文数
## 题目描述
若一个数（首位不为零）从左向右读与从右向左读都一样，我们就将其称之为回文数。
例如：给定一个十进制数 $56$，将 $56$ 加 $65$（即把 $56$ 从右向左读），得到 $121$ 是一个回文数。
又如：对于十进制数 $87$：
STEP1：$87+78=165$  
STEP2：$165+561=726$  
STEP3：$726+627=1353$   
STEP4：$1353+3531=4884$  
在这里的一步是指进行了一次 $N$ 进制的加法，上例最少用了 $4$ 步得到回文数 $4884$。
写一个程序，给定一个 $N$（$2 \le N \le 10$ 或 $N=16$）进制数 $M$（$100$ 位之内），求最少经过几步可以得到回文数。
如果在 $30$ 步以内（包含 $30$ 步）不可能得到回文数，则输出 `Impossible!`。
## 输入格式
两行，分别是 $N$，$M$。
## 输出格式
如果能在 $30$ 步以内得到回文数，输出格式形如 `STEP=ans`，其中 $\text{ans}$ 为最少得到回文数的步数。
否则输出 `Impossible!`。
## 样例 #1
### 样例输入 #1
```
10
87
```
### 样例输出 #1
```
STEP=4
```*/