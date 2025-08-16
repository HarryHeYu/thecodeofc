// Functions and Variables.

// PART 1. File Inclusions
#include<stdio.h>
#include<math.h>

// PART 2. Macro Definitions.
#define PI 3.14
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MUL(x,y)  x*y
// MUL(x,y) may introduce bug.

// PART 3. Type Definitions
// No user types in this program.

// PART 4. Global Variables
// Global variables may have a bad effect on program structure.
    float gRadius=3;
    float gArea;

// PART 5. Function Declarations
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

// PART 6. Function Implementations
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
    //如果没把第一个i改为(char)i的话，但是前面%c会自动使编译器将其识别为符号格式
    return;
}

void AreaOfCircle_BadExample(){
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

float HypotenuseOfRightTriangle(float a,float b){
   return sqrt(a*a+b*b);
}

float Max(float a,float b){
    if(a>b){
        return a;
    }
    else{
        return b;
    }
}

// PART 7. The Main Function
int main(){
    int i;
    float x1=3.14,x2=2.56;
    float y1,y2,h;
    int z;
    long factorial;

    printf("\n2024-09-30:\n");
    printf("Macroes:\n");
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

    printf("\n2024-10-09:\n");
    factorial=Factorial(3);
    printf("factorial=%ld.\n",factorial);
    printf("The process of recursion:\n");
    ShowRecursion(3);

    printf("\n2024-10-12:\n");
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

    return 0;
}
