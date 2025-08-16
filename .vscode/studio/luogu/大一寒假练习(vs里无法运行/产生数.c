#include<stdio.h>
#include<string.h>
#define MAX_LEN 35
#define MAX_RESULT_LEN 1000
int graph[10][10];//邻接矩阵，记录可达性
/*在代码里，使用一个二维数组 graph[10][10] 作为邻接矩阵来记录数字间的可达性。
其中 graph[i][j] 是一个布尔类型的值（在代码中用 0 和 1 表示），当 graph[i][j] = 1 时，
表示数字 i 可以变换成数字 j，即从数字 i 到数字 j 是可达的；当 graph[i][j] = 0 时，表示不可达。*/
int count[10];//每个数字的可达数目

//Floyd算法处理所有间接可达性
/*可达性描述了数字之间的一种关联关系。在本题中，我们有一系列的数字变换规则，
这些规则规定了某些一位数可以变换成另一些一位数。例如，如果有规则 “2 -> 5” 和 “5 -> 8”，
那么从数字 2 出发，通过先变换到 5，再从 5 变换到 8，就可以说数字 2 到数字 8 是可达的。*/
void floyd(){
    for(int k = 0; k < 10; k++){
        for(int i = 0; i < 10; i++){
            for(int j = 0; j < 10; j++){
                // 如果 i 能到 k 且 k 能到 j，则 i 能到 j
                if(graph[i][k] && graph[k][j])graph[i][j] = 1;
            }
        }
    }
}

//高精度乘法函数
/*res[]：这是一个整数数组，用于存储大整数。数组中的每个元素代表大整数的一位，并且低位数字存储在数组的低索引位置，
即采用低位在前的存储方式。例如，大整数 123 会被存储为 res[0] = 3，res[1] = 2，res[2] = 1。
*len：这是一个指向整数的指针，它指向存储大整数的数组 res 当前的长度。因为在乘法过程中，结果的位数可能会增加，所以需要通过指针来动态更新数组的长度。*/
void multiply(int res[], int *len, int multiplier){
    int carry = 0;
    for(int i = 0; i < *len; i++){
        int temp = res[i] * multiplier + carry;
        res[i] = temp % 10;
        carry = temp / 10;
    }
    //处理剩余的进位
    /*当 for 循环结束后，如果 carry 不为 0，说明还有剩余的进位需要处理。通过 while 循环，将进位逐位存入 res 数组的后面位置。
res[(*len)++] = carry % 10;：将 carry 的个位数存入 res 数组的当前末尾位置，然后将数组长度 *len 加 1。*/
    while(carry){
        res[(*len)++] = carry % 10;
        carry /= 10;
    }
}
int main(){
    //存储输入的整数n(最多是30位)
    char n[MAX_LEN];
    int k;
    //读取输入的整数n和规则数量k
    scanf("%s %d",n,&k);
    //初始化邻接矩阵，每个数字初始可达自身
    for(int i = 0; i < 10; i++){
        graph[i][i] = 1;
    }
    //处理变化规则
    for(int i = 0; i < k; i++){
        int x, y;
        //读取每条变换规则
        scanf("%d %d",&x,&y);
        //记录直接可达
        graph[x][y] = 1;
    }
    //计算所有可达路径
    floyd();
    //记录每个数字的可达数目
    for(int i = 0; i < 10; i++){
        count[i] = 0;
        for(int j = 0; j < 10; j++){
            if(graph[i][j])count[i]++;
        }
    }
    //高精度乘法初始化(结果数组，低位在前)
    int res[MAX_RESULT_LEN] = {1};
    int len = 1;
    //遍历每一位数字
    for(int i = 0; n[i] != '\0'; i++){
        int digit = n[i] - '0';
        int multiplier = count[digit];
        //执行高精度乘法
        multiply(res, &len, multiplier);
    }
    //输出结果
    for(int i = len - 1; i >= 0; i--){
        printf("%d", res[i]);
    }
    printf("\n");
    return 0;
}
/*# P1037 [NOIP 2002 普及组] 产生数
## 题目描述
给出一个整数 $n$ 和 $k$ 个变换规则。
规则：
- 一位数可变换成另一个一位数。
- 规则的右部不能为零。
例如：$n=234,k=2$。有以下两个规则：
- $2\longrightarrow 5$。  
- $3\longrightarrow 6$。 
上面的整数 $234$ 经过变换后可能产生出的整数为（包括原数）:
- $234$。
- $534$。
- $264$。
- $564$。
共 $4$ 种不同的产生数。
现在给出一个整数 $n$ 和 $k$ 个规则。求出经过任意次的变换（$0$ 次或多次），能产生出多少个不同整数。
仅要求输出个数。
## 输入格式
第一行两个整数 $n,k$，含义如题面所示。
接下来 $k$ 行，每行两个整数 $x_i,y_i$，表示每条规则。
## 输出格式
共一行，输出能生成的数字个数。
## 输入输出样例 #1
### 输入 #1
```
234 2
2 5
3 6
```
### 输出 #1
```
4
```
## 说明/提示
对于 $100\%$ 数据，满足 $n \lt 10^{30}$，$k \le 15$。*/