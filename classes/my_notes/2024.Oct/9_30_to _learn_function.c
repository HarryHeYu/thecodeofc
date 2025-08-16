/*程序框架
1.File Inclision
2.Macro Definition
3.Type Definiition
4.全局变量Global Variables
5.函数声明Function Declarations
6.函数实现Function Implementation
7.主函数The Main Function*/
#include<stdio.h>
#include<math.h>/*这个是数学计算的库*/

#define PI 3.14
#define MAX(a,b) (((a)>(b))?(a):(b))/*每个变量都得括起来，最后的表达式也得加括号*/
// a>b?a:b
#define MUL(x,y) ((x)*(y))

/*函数又叫子程序*/
float Max(float a , float b);/*这是函数的声明，要带分号*/
/*The Max function returns the maximum value from a,b.*/
float Max (float a , float b){
//     float max;
//     max = a;
//     if(max < b){
//         max = b;
//     }
//     return max;/*定义函数*/
// /*最开头float的类型是最后return的类型*/
// /*子函数的的return返回到主函数*/
    return (a > b)?a:b;/*这是一个三元运算符*/
}
float HypotenuseOFRightTriangle(float a, float b);
/*这个函数返回的是直角三角形的斜边长*/
float HypotenuseOFRightTriangle(float a, float b){
    return sqrt(a*a + b*b);
}
void PrintMultiplicationTable(void);
/*九九乘法表*/
void PrintMultiplicationTable(void){
    int i,j;
    for(i = 1;i <= 9; i++){
        for(j = 1;j <= i; j++){
            printf("%d*%d = %2d",j,i,j*i);
        }
        printf("\n");
    }
    return;
}

int main(){
    float x1 = 3.14,x2 = 2.56;
    float y;
    y = Max(x1,x2);
    printf("Max = %f\n",y);
    float h;/*用一个变量前，先定义它*/
    h = HypotenuseOFRightTriangle(x1, x2);
    printf("h = %f\n",h);
    PrintMultiplicationTable();
    int z;
    z = MUL(1+2,3+4);
    printf("z = %d\n",z);
    return 0;
}/*主函数返回到操作系统*/