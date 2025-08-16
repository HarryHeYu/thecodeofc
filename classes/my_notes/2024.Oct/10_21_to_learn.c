//计算机中数据远多于程序
#include <stdio.h>
#include <stdlib.h>//标准库，random在这里面。
/*种子值决定了随机数序列的起点，从而影响生成的随机数序列。当你使用相同的种子值多次初始化随机数生成器时，它会生成相同的随机数序列。
 rand()：生成伪随机数；srand()：设置随机数生成器的种子*/
#include <time.h>//时间库

#define N 10
#define MAX 100
// int a[N];//定义一个数组，数组名a，元素个数N
// /*a[0],a[1],a[2],...a[N-1],a[N],a[N+1],...([]中的叫下标)，下标从0开始,a[N]开始的都越界了，不存在
// 超出了申请的范围（很多对程序攻击就是利用越界，因为有些没有数组检查）
// a is the address of the array
// a是一个地址常量
// int *p;
// p = a;//p是指针变量，a是指针常量   *p....(指向）a[0]    意思是p中储存的地址可以更改，而a中的不可以   改变p本身的值不会改变a的值
// p++;//p指向a[1]
// a++;//a[1]的地址，a++没有意义,常量不能改变;p = a+1相当于a = a+1
// p = &a[2];//p指向a[2]
/*在C和C++中，数组中的每个元素本身并不是一个指针，但它的地址可以通过数组名和索引来访问，这在某种程度上与指针的行为相似。具体来说：
数组元素的地址：当你有一个数组时，你可以通过数组名加上索引来访问数组中的元素。
例如，如果有一个数组 int arr[10];，那么 arr[0]、arr[1] 等都是数组元素。在内存中，arr[0] 的地址就是数组的起始地址，
arr[1] 的地址就是数组起始地址加上一个 int 类型大小的偏移量，以此类推。数组名作为指针：在C和C++中，数组名可以被看作是一个指向数组第一个元素的指针。
这意味着 arr（不带有索引）实际上是一个指向 arr[0] 的指针。因此，&arr[0]（数组第一个元素的地址）和 arr（数组名）在大多数情况下是等价的，
除了在某些表达式中可能会有细微的差别。指针和数组的转换：在函数参数中，如果你有一个指向数组元素类型的指针（例如 int*），
你可以将它赋值给一个数组（例如 int arr[10]），但你需要确保数组有足够的空间来存储你将要复制的数据。然而，这种转换通常不是必要的，因为你可以直接传递数组名作为指针。
指针算术：在数组中，你可以使用指针算术来遍历数组。例如，arr + 1 会给你 arr[1] 的地址，这与 &arr[1] 是相同的。
尽管数组名在很多情况下表现得像一个指针，但它们在语法上并不完全相同。例如，你不能对数组名进行指针算术操作，除非它被用作函数参数或者在特定的上下文中（如循环中）。
此外，数组的大小信息在数组名中是可用的，而在指针中则没有这样的信息。
在C++中，标准库提供了一些模板和函数，如 std::begin 和 std::end，它们可以返回指向数组第一个和最后一个元素之后位置的指针，这使得数组可以被用作某些算法和容器的输入。
总结来说，数组中的每个元素不是一个指针，但数组名可以被看作是指向数组第一个元素的指针，并且你可以使用指针的概念来访问数组中的元素。*/


// int q[];//[]不表示数组，表示int *q;
// q = a;
// q[i] = a[i];//q[i] = *(q+i) = *(a+i) = a[i]


// int a[N] = {0,1,3};
// int b[4] = {2,5,8,5}；//定一个数组，虽然但是不写【】中的数编译器也会自动补充，但是还是最好写上


void ClearArray(int a[],int n);//函数声明，函数定义在后面  把一块内存清零
void PrintArray(int *a,int n);//函数声明，函数定义在后面  把一块内存打印出来
void RandomizeArray(int a[],int n,int max);//函数声明，函数定义在后面  把一块内存随机化
void ScalarMult(int a[],int n,int k);//函数声明，函数定义在后面  把一块内存乘以一个数
void AddVectors(int a[],int b[],int n);//函数声明，函数定义在后面  把两个数组加起来
int DotProduct(int a[],int b[],int n);//函数声明，函数定义在后面  计算两个向量的点积

void ScalarMult(int a[],int n,int k){
    int i;
    for(i=0;i<n;i++){
        a[i] = a[i]*k;
    }
    return;
}
void AddVectors(int a[],int b[],int n){
    int i;
    for(i=0;i<n;i++){
        a[i] = a[i]+b[i];
    }
    return;
}
int DotProduct(int a[],int b[],int n){
    int i,sum;
    for(i=0,sum=0;i<n;i++){
        sum = sum+a[i]*b[i];
    }
    return sum;
}
void RandomizeArray(int a[],int n,int max){
    int i;
    // srand(1);//伪随机数，种子恒定
    srand(time(NULL));//真随机数
/*在 C 语言中，time(NULL) 是一个调用 time 函数的表达式，其中 NULL 是一个宏，代表空指针。
这个调用的意思是获取当前时间，并将其作为 time_t 类型的值返回。time_t 通常是一个能够存储自 1970 年 1 月 1 日 00:00:00 UTC 以来的秒数的整数类型。
这里的 NULL 作为参数传递给 time 函数，表示你不关心函数返回的时间值，或者你不需要将这个值存储在任何地方。尽管如此，time 函数仍然会返回当前时间的 time_t 表示，
即使你没有提供一个变量来接收它。*/
    //void srand(unsigned int seed);
    //time_t(类型) time(time_t *timer)（函数）;可以返回系统的时间
    //time_t is a long integer type.
    /*time_t t;
    t = time(NULL);or time(&t);
    返回值是从1970开始计数的秒数*/
    for(i=0;i<n;i++){
        a[i] = rand()%max;
        /*int rand(void);
        the function returns a random number*/
    }
    return;
}
void ClearArray(int a[],int n){
    int i;
    for(i=0;i<n;i++){
        a[i] = 0;
    }
    return;
}
void PrintArray(int *a,int n){
    int i;
    for(i=0;i<n;i++){
        printf("%d ",a[i]);
        printf("\n");
    }
}

int main(){
    int a[N];
    int b[N];
    RandomizeArray(a,N,MAX);
    PrintArray(a,N);
    ClearArray(a,N);
    PrintArray(a,N);
    ScalarMult(a,N,9);
    PrintArray(a,N);
    AddVectors(a,b,N);
    PrintArray(a,N);
    printf("%d\n",DotProduct(a,b,N));
    return 0;
}