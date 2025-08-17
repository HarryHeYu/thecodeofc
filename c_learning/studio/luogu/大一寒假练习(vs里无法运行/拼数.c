#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int compare(const void *a, const void *b);
int compare(const void *a, const void *b){
    const char *s1 = *(const char **)a;
    const char *s2 = *(const char **)b;
    char temp1[24], temp2[24];
    strcpy(temp1, s1);//复值
    strcat(temp1, s2);//拼接字符串
    strcpy(temp2, s2);
    strcat(temp2, s1);
    return strcmp(temp2, temp1);//比较大小
}
int main(){
    int n;
    scanf("%d",&n);
    char nums[20][12];/*12 字节的列宽设计是为了满足题目中 a_i ≤ 1e9 的最大长度需求：
最大数字 1,000,000,000 转换为字符串需要 11字节（10位数字 + 终止符 \0）
12 字节提供安全余量，避免溢出*/
    char *ptrs[20];
    for(int i = 0; i < n; ++i){
        scanf("%s",nums[i]);
        ptrs[i] = nums[i];
    }
    qsort(ptrs, n, sizeof(char*), compare);
    /*void qsort(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void *));
    base：指向待排序数组首地址的指针
    nitems：数组中元素的数量
    size：单个元素所占字节数
    compar：指向比较函数的指针，该函数用于定义排序规则
    比较函数的原型：int compar(const void* a, const void *b);返回值<0：a<b;=0a=b;>0:a>b
    int compare_int(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}*/
    char result[400] = {0};
    for(int i = 0; i < n; i++){
        strcat(result, ptrs[i]);
    }
    if(result[0] == '0'){
        printf("0\n");
    }else{
        printf("%s\n",result);
    }
    return 0;
}  
/*# [NOIP1998 提高组] 拼数
## 题目描述
设有 $n$ 个正整数 $a_1 \dots a_n$，将它们联接成一排，相邻数字首尾相接，组成一个最大的整数。
## 输入格式
第一行有一个整数，表示数字个数 $n$。
第二行有 $n$ 个整数，表示给出的 $n$ 个整数 $a_i$。
## 输出格式
一个正整数，表示最大的整数
## 样例 #1
### 样例输入 #1
```
3
13 312 343
```
### 样例输出 #1
```
34331213
```
## 样例 #2
### 样例输入 #2
```
4
7 13 4 246
```
### 样例输出 #2
```
7424613
```
## 提示
对于全部的测试点，保证 $1 \leq n \leq 20$，$1 \leq a_i \leq 10^9$。*/