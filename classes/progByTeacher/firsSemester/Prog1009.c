// Skeleton of the C program.

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
// No global variables in this program.

// PART 5. Function Declarations
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
    float x1=3.14,x2=2.56;
    float y1,y2,h;
    int z;
    long factorial;
    factorial=Factorial(3);
    printf("factorial=%ld.\n",factorial);
    printf("The process of recursion:\n");
    ShowRecursion(3);
    putchar('\n');
    y1=MAX(x1,x2);
    y2=Max(x1,x2);
    printf("Max: y1=%f,y2=%f\n",y1,y2);
    z=MUL(1+2,3+4);
    // z=1+2*3+4;
    printf("z=MUL(1+2,3+4)=1+2*3+4=%d. BUG HERE.\n",z);
    h=HypotenuseOfRightTriangle(x1,x2);
    printf("Hypotenuse h=%f\n",h);
    PrintMultiplicationTable();
    return 0;
}
