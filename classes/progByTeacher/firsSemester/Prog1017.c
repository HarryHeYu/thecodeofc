/* This program helps you learn programing with
 * the C language.
 * If you want to learn something, please
 * #define LEARN_something 1
 * in the part of macro definitions.
 */

// PART 1. File Inclusions
#include<stdio.h>
#include<math.h>
#include<float.h>

// PART 2. Macro Definitions.
#define LEARN_FUNCTIONS_AND_MACROS 0
#define LEARN_RECURSIVE_FUNCTIONS  0
#define LEARN_VARIABLES 0
#define LEARN_POINTER_VARIABLES 0
#define LEARN_PARAMETER_VARIABLES 1

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
void LearnFunctionsAndMacros(void);
// This function helps learn functions and macros.

void LearnRecursiveFunctions(void);
// This function helps learn recursive functions.

void LearnVarialbes(void);
// This function helps learn variables.

void LearnPointerVariables(void);
// This function helps learn pointer variables.

void LearnParameterVariables(void);
// This function helps learn parameter variables.

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

void Increase(int *px);
// This function increases *px by 1.

void FailToIncrease(int x);
// This function tries to increase x but fails.

void Swap(int *px1,int *px2);
// This function swaps *px1 and *px2.

int RootsOfQuadraticEquation(double a,double b,double c,
                             double *px1,double *px2);
/* This function finds the real roots of
 * ax^2+bx+c=0.
 * Output: two real roots in *px1 and *px2.
 * Return: 2, if two real roots;
 *         1, if one real root;
 *         0, if no real roots;
 *         -1, if not a quadratic equation.
 * Note: #include<math.h> and #include<float.h>
 */

// PART 6. Function Implementations
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
        printf("%6X:%4c (%XH=%d)\n",pc,*pc,*pc,*pc);
        pc++;
    }
    return;
}

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
    printf("Value of pi: %u \n",pi);
    printf("Value of *pi: %d\n\n",*pi);

    printf("Address of ppi: %u \n",&ppi);
    printf("Size of ppi: %u \n",sizeof(ppi));
    printf("Value of ppi: %u \n",ppi);
    printf("Value of *ppi: %u \n",*ppi);
    printf("Value of **ppi: %d \n",**ppi);
    return;
}

double CalculateIntegral(double (*pf)(double),
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

void Increase(int *px){
    (*px)++;
    return;
}

void FailToIncrease(int x){
    x++;
    return;
}

void Swap(int *px1,int *px2){
    int temp;
    temp=*px1;
    *px1=*px2;
    *px2=temp;
    return;
}

int RootsOfQuadraticEquation(double a,double b,double c,
                             double *px1,double *px2){
    double delta;
    if(fabs(a)<DBL_EPSILON){
        return -1;
    }
    delta=b*b-4*a*c;
    if(delta<0){
        return 0;
    }
    else if(delta>0){
        *px1=(-b+sqrt(delta))/(2*a);
        *px2=(-b-sqrt(delta))/(2*a);
        return 2;
    }
    else{
        *px1=-b/(2*a);
        *px2=-b/(2*a);
        return 1;
    }
}

void LearnFunctionsAndMacros(){
    float x1=3.14,x2=2.56;
    float y1,y2,h;
    int z;

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
    return;
}

void LearnRecursiveFunctions(){
    long factorial;
    printf("\n2024-10-09: Recursive Functions\n");
    factorial=Factorial(3);
    printf("factorial=%ld.\n",factorial);
    printf("The process of recursion:\n");
    ShowRecursion(3);
    return;
}

void LearnVarialbes(){
    int i;
    float gRadius=1;
    printf("\n2024-10-12: Variables\n");
    UnderstandVariable();
    putchar('\n');
    printf("Global variables may have a bad effect.\n");
    AreaOfCircle_BadExample();
    printf("gRadius=%f,gArea=%f\n",gRadius,gArea);
    gArea=AreaOfCircle(gRadius);
    printf("gRadius=%f,gArea=%f\n",gRadius,gArea);
    AreaOfCircle_BadExample();
    printf("gRadius=%f,gArea=%f\n",gRadius,gArea);

    printf("\nStatic variable vs auto variable:\n");
    for(i=0;i<3;i++){
        StaticVariable();
        AutoVariable();
    }
    return;
}

void LearnPointerVariables(){
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
    return;
}

void LearnParameterVariables(){
    int i=0,j=100;
    printf("i=%d,j=%d\n",i,j);
    Increase(&i);
    FailToIncrease(j);
    printf("After Increase: i=%d,j=%d\n",i,j);
    Swap(&i,&j);
    printf("After Swap:i=%d,j=%d\n",i,j);
    putchar('\n');

    int r;
    double a=1,b=2,c=1;
    double x1,x2;
    printf("Find roots of %.2fx^2+%.2fx+%.2f=0:\n",a,b,c);
    r=RootsOfQuadraticEquation(a,b,c,&x1,&x2);
    if(r<0){
        printf("Error. Illegal input.\n");
    }
    else if(r==0){
        printf("No real roots.\n");
    }
    else if(r==1){
        printf("One root: x1=x2=%lf \n",x1);
    }
    else if(r==2){
        printf("Two roots: x1=%lf,x2=%lf \n",x1,x2);
    }
    else{
        printf("Error.\n");
    }
    return;
}

// PART 7. The Main Function
int main(){
    #if LEARN_FUNCTIONS_AND_MACROS
    LearnFunctionsAndMacros();
    #endif // LEARN_FUNCTIONS_AND_MACROS

    #if LEARN_RECURSIVE_FUNCTIONS
    LearnRecursiveFunctions();
    #endif // LEARN_RECURSIVE_FUNCTIONS

    #if LEARN_VARIABLES
    LearnVarialbes();
    #endif // LEARN_VARIABLES

    #if LEARN_POINTER_VARIABLES
    LearnPointerVariables();
    #endif // LEARN_POINTER_VARIABLES

    #if LEARN_PARAMETER_VARIABLES
    LearnParameterVariables();
    #endif // LEARN_PARAMETER_VARIABLES

    return 0;
}

