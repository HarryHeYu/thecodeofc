#include<stdio.h>
/* 1/1 1/2 2/1 1/3 2/2 3/1 1/4 2/3 3/2 4/1 1/5   */
int main(){
    int n;
    scanf("%d",&n);
    int d = 0, sum = 0;
    while(sum < n){
        d++;
        sum += d;
    }
    int pos = n - sum + d;
    int i, j;
    if(d % 2 == 0){
        i = pos;
        j = (d + 1) - pos; 
    }else{
        i = d + 1 - pos;
        j = pos;
    }
    printf("%d/%d",i, j);
    return 0;
}
/*# [NOIP1999 普及组] Cantor 表
## 题目描述
现代数学的著名证明之一是 Georg Cantor 证明了有理数是可枚举的。他是用下面这一张表来证明这一命题的：
![](https://cdn.luogu.com.cn/upload/image_hosting/jdjdaf73.png)
我们以 Z 字形给上表的每一项编号。第一项是 $1/1$，然后是 $1/2$，$2/1$，$3/1$，$2/2$，…
## 输入格式
整数$N$（$1 \leq N \leq 10^7$）。
## 输出格式
表中的第 $N$ 项。
## 样例 #1
### 样例输入 #1
```
7
```
### 样例输出 #1
```
1/4
```
## 提示
- 2024-11-18 0:30 数据中加入了样例，放在不计分的子任务 2 中。*/