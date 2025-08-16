// Functions and Variables.

// PART 1. File Inclusions
#include<stdio.h>
#include<math.h>

// PART 2. Macro Definitions.
#define PI 3.14
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MUL(x,y)  x*y
// MUL(x,y) may possibly introduce bug.
#define MAX_DIVISOR 10000
#define MIN_DELTA 0.0001
// The above two macros are used in calculating integral.

// PART 3. Type Definitions
// No user types in this program.

// PART 4. Global Variables
// Global variables may have a bad effect on program structure.
    float gRadius=3;
    float gArea;

// PART 5. Function Declarations
int FindRealRoots(float a,float b,float c,float *px1,float *px2);
/*This function finds reals roots of the equation: ax^2 + bx + c = 0.
Return 2 if two reals,
       1 if i=one,
       0 if no real roots,
       -1 if not a quadratic equatioin.*/
void Increase1(int  *px);
void Increase2(int  x);//x是参数变量（局部）

float Max(float a,float b);
/* The Max function returns the maximum value
 * from {a,b}.
 */

float HypotenuseOfRightTriangle(float a,float b);
/* This function returns the hypotenuse of a right triangle.
 * The parameters a and b are the two legs of the triangle.
 * Note: #include <math.h>
 */

void PrintMultiplicationTable(void);
/* This function prints the multiplication table
 * on the screen.
 */

long Factorial(unsigned n);
// The Factorial function returns factorial n.

long ShowRecursion(unsigned n);
/* This function returns factorial n, and
 * shows the process of recursion.
 */

void UnderstandVariable(void);
// This function helps understand variables.

void AreaOfCircle_BadExample(void);
/* This is NOT a good function.
 * It finds the area of a circle.
 * The radius of the circle is gRadius.
 * The area is output into gArea.
 */

float AreaOfCircle(float r);
/* Given radius r,
 * this function returns the area of the circle.
 */

void StaticVariable(void);
// This function helps understand static variables.

void AutoVariable(void);
// This function helps understand auto variables.

void UnderstandPointerType(void);
// This function helps understand pointer types.

void AccessMemoryThroughPointers(void);
// Learn how to access memory through pointers.

void UnderstandPointerToPointer(void);
// This function helps understand pointers to pointers.

double CalculateIntegral(double (*pf)(double),
                         double a,double b);
/* The CalculateIntegral function returns the integral of
 * a function f(x) on the interval [a,b].
 * pf is the pointer to f(x).
 * The precision of the result is relevant to MAX_DIVISOR
 * and MIN_DELTA.
 * A larger MAX_DIVISOR and a smaller MIN_DELTA make the
 * result more accurate, at the price of more running time.
 */

double f1(double x);
// f1(x).

double f2(double x);
// f2(x).

// PART 6. Function Implementations
int FindRealRoots(float a,float b,float c,
                  float *px1,float *px2){
    // float delta;
    // delta=b*b-4*a*c;
    // if(delta<0){
    //     return 0;
    // }
    // else if(delta==0){
    //     *px1=-b/(2*a);
    //     return 1;
    // }
    // else if(a == 0) {
    //     return -1;
    // }
    // else{
    //     *px1=(-b+sqrt(delta))/(2*a);
    //     *px2=(-b-sqrt(delta))/(2*a);
    //     return 2;
    // }                    //这是我写的
    float delta;
    if (Fabs(a) <__FLT_EPSILON__) {//浮点数不能直接用等号即==，要使用Fabs函数
        return -1;
    }
    delta=b*b-4*a*c;
    if(delta<0){
        return 0;
    }
    else if(delta > 0){
        *px1=(-b+sqrt(delta))/(2*a);
        *px2=(-b-sqrt(delta))/(2*a);
        return 2;
    }
    else{
        *px1=-b/(2*a);
        return 1;
    }
  
}
void Increase1(int  *px){
    (*px)++;
    return;
}

void Increase2(int  x){
    x++;
    return;
}
float Max(float a,float b){
    if(a>b){
        return a;
    }
    else{
        return b;
    }
}

float HypotenuseOfRightTriangle(float a,float b){
   return sqrt(a*a+b*b);
}

void PrintMultiplicationTable(void){
   int i,j;
   for(i=1;i<=9;i++){
       for(j=1;j<=i;j++){
           printf("%d*%d=%2d\t",j,i,i*j);
       }
       printf("\n");
    }
    return;
}

long Factorial(unsigned n){
    if(n<=1){
        return 1;
    }
    else{
        return n*Factorial(n-1);
    }
}

long ShowRecursion(unsigned n){
    long int y;
    printf("f(%d) begins.\n",n);
    // f(n) denotes factorial(n).
    if(n<=1){
        y=1;
    }
    else{
        y=n*ShowRecursion(n-1);
    }
    printf("f(%d)=%ld ends.\n",n,y);
    return y;
}

void UnderstandVariable(){
    int i=-191;
    printf("Deep understanding of int i=-191;\n");
    printf("Address: %u\n",&i);
    printf("Length: %u\n",sizeof(i));
    printf("Data: %X(H)\n",i);
    // FFFF FF41
    printf("Look at i as a signed integer: %d\n",i);
    printf("Look at i as an unsigned integer:%u\n",i);
    printf("Look at i as an ASCII symbol: %c\n",i);
    printf("The symbol %c has an ASCII value: %d\n",i,(char)i);
    return;
}

void AreaOfCircle_BadExample(){
// This function is tightly coupled with global variables.
    gArea=PI*gRadius*gRadius;
    return;
}

float AreaOfCircle(float r){
    return PI*r*r;
}

void StaticVariable(){
// This function helps understand static variables.
    static int s=0;
    s++;
    printf("Static variable s=%d.\n",s);
    return;
}

void AutoVariable(){
// This function helps understand auto variables.
    auto int a=0;  // auto can be omitted.
    a++;
    printf("Auto variable a=%d.\n",a);
    return;
}

void UnderstandPointerType(){
    int k;
    int *pk;
    char *pc;
    pk=&k;
    pc=(char*)pk;
    printf("k\'s address: %u, k\'s length: %u\n",&k,sizeof(k));
    printf("pc=%6u, pk=%6u\n",pc,pk);
    pc++;
    pk++;
    printf("After ++ : pc=%6u, pk=%6u\n",pc,pk);
    pc+=2;
    pk+=2;
    printf("After +2 : pc=%6u, pk=%6u\n",pc,pk);
    pc--;
    pk--;
    printf("After -- : pc=%6u, pk=%6u\n",pc,pk);
    return;
}

void AccessMemoryThroughPointers(){
    int i,k;
    int *pk;
    char *pc;
    pk=&k;
    *pk=0x33445566;
    printf("k\'s address: %u \n",pk);
    printf("k\'s size: %u \n",sizeof(k));
    printf("k\'s contents: %X(H) \n",*pk);
    pc=(char*)pk;
    printf("Address: Character (ASCII value) \n");
    for(i=0;i<sizeof(k);i++){
        printf("%6u:%4c (%XH=%d)\n",pc,*pc,*pc,*pc);
        pc++;
    }
    return;
}
/*k存在了（地址数字最小的）低位地址，存了连续四个地址（int*是整型，有四个字节），
33存在了最高位（即地址数字最大的），
66存在了最低位，与k在一起*/

void UnderstandPointerToPointer(){
    int i=123;
    int *pi;
    int **ppi;

    pi=&i;
    ppi=&pi;

    printf("Address of i: %u \n",&i);
    printf("Size of i: %u \n",sizeof(i));
    printf("Value of i: %d \n\n",i);

    printf("Address of pi: %u \n",&pi);
    printf("Size of pi: %u \n",sizeof(pi));
    printf("Value of pi: %u \n",pi);//这个打出来就是i的地址
    printf("Value of *pi: %d\n\n",*pi);//这个打出来就是i的值

    printf("Address of ppi: %u \n",&ppi);
    printf("Size of ppi: %u \n",sizeof(ppi));//应该和pi一样
    printf("Value of ppi: %u \n",ppi);//pi的地址
    printf("Value of *ppi: %u \n",*ppi);//这个是pi的值即i的地址
    printf("Value of **ppi: %d \n",**ppi);//打出来是i的值
    return;
}

double CalculateIntegral(double (*pf)(double),//前一个double是输出类型，后一个括号里的double是接收参数的类型
                         double a,double b){
    double delta,xi,sum;
    delta=(b-a)/MAX_DIVISOR;
    if(delta<MIN_DELTA){
        delta=MIN_DELTA;
    }
    xi=a;
    sum=0;
    while(xi<b){
        sum+=(*pf)(xi)*delta;
        xi+=delta;
    }
    return sum;
}

double f1(double x){
    return 2*x+3;
}

double f2(double x){
    return x*x+2*x+1;
}

// PART 7. The Main Function
int main(){
    int i;
    float x1=3.14,x2=2.56;
    float y1,y2,h;
    int z;
    long factorial;

    printf("2024-09-30: Macros and Functions\n");
    printf("Macros:\n");
    y1=MAX(x1,x2);
    y2=Max(x1,x2);
    printf("Max: y1=%f,y2=%f\n",y1,y2);
    z=MUL(1+2,3+4);
    // z=1+2*3+4;
    printf("z=MUL(1+2,3+4)=1+2*3+4=%d. BUG HERE.\n\n",z);
    printf("Functions:\n");
    h=HypotenuseOfRightTriangle(x1,x2);
    printf("Hypotenuse h=%f\n",h);
    PrintMultiplicationTable();

    printf("\n2024-10-09: Recursive Functions\n");
    factorial=Factorial(3);
    printf("factorial=%ld.\n",factorial);
    printf("The process of recursion:\n");
    ShowRecursion(3);

    printf("\n2024-10-12: Variables\n");
    UnderstandVariable();
    putchar('\n');

    printf("Global variables may have a bad effect.\n");
    AreaOfCircle_BadExample();
    printf("gRadius=%f,gArea=%f\n",gRadius,gArea);
    float gRadius=1;
    gArea=AreaOfCircle(gRadius);
    printf("gRadius=%f,gArea=%f\n",gRadius,gArea);
    AreaOfCircle_BadExample();
    printf("gRadius=%f,gArea=%f\n",gRadius,gArea);

    printf("\nStatic variable vs auto variable:\n");
    for(i=0;i<3;i++){
        StaticVariable();
        AutoVariable();
    }

    printf("\n2024-10-14: Pointer Variables\n");
    printf("Understand pointer types:\n");
    UnderstandPointerType();
    printf("\nAccess memory through pointers:\n");
    AccessMemoryThroughPointers();
    printf("\nUnderstand pointers to pointers:\n");
    UnderstandPointerToPointer();
    printf("\nCalculate the integral of a given function:\n");
    double s1,s2,a=1,b=2;
    s1=CalculateIntegral(f1,a,b);
    s2=CalculateIntegral(f2,a,b);
    printf("Integral of f1(x) on [%lf,%lf] is %lf\n",a,b,s1);
    printf("Integral of f2(x) on [%lf,%lf] is %lf\n",a,b,s2);

    int  x = 3;
    Increase1(&x);
    printf("x=%d\n",x);//而该着将主函数x的地址传给了函数，函数操作的是主函数的x
    Increase2(x);
    printf("x=%d\n",x);//主函数与函数中都有x，操作的是函数里的x而不是主函数的，两者无关

    return 0;
}
