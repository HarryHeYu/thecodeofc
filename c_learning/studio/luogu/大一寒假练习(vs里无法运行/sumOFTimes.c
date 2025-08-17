#include <stdio.h>
#include <string.h>
#define MAX 5000 // 定义一个足够大的数组来存储高精度数
// 函数用于将两个高精度数相加
void add(int a[], int b[]) {
    int carry = 0;//进位初始化为0，=
    for (int i = 0; i < MAX; i++) {
        a[i] = a[i] + b[i] + carry;
        carry = a[i] / 10;//若当前位的a+b+carry>=10，则将当前位进行进位（十进制）
        a[i] %= 10;//e.g:7+7+7=21->21%10=1,则当前位是1而carry就是2
    }
}
// 函数用于计算一个高精度数与一个整数的乘积
void multiply(int a[], int num) {
    int carry = 0;
    for (int i = 0; i < MAX; i++) {
        a[i] = a[i] * num + carry;
        carry = a[i] / 10;
        a[i] %= 10;
    }
}
// 函数用于打印高精度数
void print(int a[]) {
    int i = MAX - 1;
    while (i > 0 && a[i] == 0) i--; // 去掉前导0
    for (; i >= 0; i--) {
        printf("%d", a[i]);
    }
    printf("\n");
}
int main() {
    int n;
    scanf("%d", &n);

    int sum[MAX] = {0}; // 存储最终的结果
    int factorial[MAX] = {0}; // 存储当前的阶乘
    factorial[0] = 1; // 初始化1! = 1

    for (int i = 1; i <= n; i++) {
        multiply(factorial, i); // 计算i!
        add(sum, factorial); // 加到sum中
    }

    print(sum); // 打印结果
    return 0;
}
/*# [NOIP1998 普及组] 阶乘之和
## 题目描述
用高精度计算出 $S = 1! + 2! + 3! + \cdots + n!$（$n \le 50$）。
其中 `!` 表示阶乘，定义为 $n!=n\times (n-1)\times (n-2)\times \cdots \times 1$。例如，$5! = 5 \times 4 \times 3 \times 2 \times 1=120$。
## 输入格式
一个正整数 $n$。
## 输出格式
一个正整数 $S$，表示计算结果。
## 样例 #1
### 样例输入 #1
```
3
```
### 样例输出 #1
```
9
```
## 提示
**【数据范围】**
对于 $100 \%$ 的数据，$1 \le n \le 50$。
*/