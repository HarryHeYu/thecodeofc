//二维数组，可以看成矩阵.......动态内存
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define M 3
#define N 4
#define MAX 100
#define NUM 100

// void PrintArray(int a[], int n);
void PrintArray(float a[], int n);
void Randomise(int a[], int n,int max);
void Assign(float a[], int n);

// void PrintArray(int a[], int n){
//     int i;
//     for(i=0;i<n;i++){
//         printf("%d \t",a[i]);
//     }
//     return;
// }

void PrintArray(float a[], int n){
    int i;
    for(i=0;i<n;i++){
        printf("%f \t",a[i]);
    }
    return;
}

void Randomise(int *a, int n,int max){
    int i;
    srand((unsigned)time(NULL));
    for(i=0;i<n;i++){
        a[i] = rand()%max;
        //rand() 函数生成一个伪随机数，这个数的范围通常是从 0 到 RAND_MAX（在 stdlib.h 头文件中定义，通常 RAND_MAX 的值是 32767）。
        //% max 取 rand() 生成的随机数与 max 相除的余数。
        //因此，a[i] = rand() % max; 这行代码的作用是将数组 a 的第 i 个元素设置为一个随机数，这个随机数的范围是从 0 到 max-1。这是因为取模运算会得到小于 max 的所有可能的余数。
    }
    return;
}

void Assign(float a[], int n){
    int i;
    for(i=0;i<n;i++){
        a[i] = sqrt(i);
    }
    return;
}

int main(){
//     int a[NUM];
//     int b[M][N] = {{0,1,2,3},
//                    {10,11,12,13},
//                    {20,21,22,23}};//m行n列，第一个脚标看成行，第二个脚标看成列.外面的花括号表示赋初值，里面是行。(若后面要随机分配的话也是可以不用赋初值的)
//     int c[3][4][5];//编译器的方括号内放常量，定分配的内存空间。
//     int n;
//     printf("Please enter the length.\n");
//     scanf("%d",&n);//栈的内存是编译时分配的，堆的内存（具体运行时）是运行时分配的
//     int d[n];//变长数组，这个变量必须要有值
//     其中的数组a即为变长数组，程序在执行时可以准确计算出它的长度。长度
//     的计算必须放在数组定义之前。变长数组存储在栈区，变长数组的内存一
//     旦分配，它的长度就不能再变了。
//     变长数组可以根据程序运行时的需求来分配内存，因而不会出现浪费内
//     存的情况。变长数组的另一个好处是便于实现多维数组作函数参数时的松
//     耦合设计
//     Randomise(d,n,MAX);
//     PrintArray(d,n);
//     int i,j;
//     Randomise((int*)b,M*N,MAX);//b的类型为行指针，强制转换为整型指针才会一个一个的变换（就会是加4*4的字节，整形的就是4*1个字节）
//     //二维数组的数组名指针是行指针
//     // for(i=0;i<M;i++){
//     //     for(j=0;j<N;j++){
//     //         printf("%d\t",b[i][j]);
//     //     }
//     //     putchar('\n');
//     // }
//     Randomise(a,NUM,MAX);
//     // PrintArray(a,NUM);


//     int a[NUM];//a是占用栈的内存，NUM不能太大
    //申请内存要向操作系统申请
    //没用堆内存的程序都是简单程序
    //内存借完一定要还
    //堆内存（Heap Memory）是计算机科学中的一个术语，指的是在程序运行时动态分配的内存区域。
    //与栈内存（Stack Memory）不同，堆内存不是由编译器自动管理的，而是由程序员通过内存分配函数显式分配和释放的。
    float *p;
    //void* malloc(unsigned int size)  //返回型为通用指针。有些编译器会报错，需要强制转换，我们写的时候默认会报错，就要写。
    p = (float*)malloc(NUM*sizeof(float));//申请100个字节(括号内不要写常数)。内存申请函数。堆内存，动态内存。//一定要写XXX*sizeof(XX)
    //malloc 是 C 语言标准库中的一个函数，用于动态内存分配。
    //它定义在 <stdlib.h> 头文件中，其主要作用是在堆区（heap）申请指定大小的内存空间，并返回指向该内存块的指针。如果申请失败，通常会返回 NULL 指针。
    if(p==NULL){//如果为NILL则表明分配内存失败
        printf("Memory allocation failed.\n");
        return 1;
    }
    Assign(p,NUM);
    PrintArray(p,NUM);
    free(p);//把内存还给操作系统。释放内存，释放后p的值是随机的，不能再用，但指针还是指向那块内存。（指针悬空问题）
    p = NULL;//赋空值，防止悬空指针，C语言规定为空值的内存不能通过指针访问。
    return 0;
}