#include<stdio.h>
#include<math.h>

#define STACK_SIZE 100
#define N 128
//程序等于数据加算法结构（加结构化设计）
/*typedef*/
typedef int Integer;//(在int前加一个typedef，Integer就变成一个类型了)
//给变量取名要用大驼峰命名法

//在程序设计中，有时需要这样的变量，它只能取少量的有意义的值。一般用enum关键字为这种变量定义枚举类型
//枚举类型只能取枚举常量中的某一个值，不能取其他值
//workday 和 weekend 被定义为枚举变量，花括号中的 sun，mon，… ，sat 称为枚举元素或枚举常量。它们是用户指定的名字。
// 枚举变量和其他数值型量不同，它们的值只限于花括号中指定的值之一。例如枚举变量 workday 和 weekend 的值只能是 sun 到 sat 之一。
typedef enum{FALSE=0,TRUE=1}Bool;//加了typedef后，Bool就变成了类型名
typedef enum{CLUB, DIAMOND,HEART,SPADE}Suit;//Suit通常用来表示一副牌中的花色
typedef enum{ERROR=0,OK=1}Status;

typedef struct PlanePoint{//这个表示结构名
    float x;
    float y;
}PlanePoint;//这个表示类型名,使用时就不需要写struct关键字了，直接使用类型名即可

/*在 C 语言中，结构体（struct）是一种用户定义的数据类型，它允许你将不同类型的数据组合在一起。结构体可以包含多个成员，
每个成员可以是不同的数据类型，如整数、浮点数、字符、数组、指针，甚至是其他结构体。
结构体的定义如下：
struct 结构体名称 {
    数据类型 成员1;
    数据类型 成员2;
    ...
};
可以使用点操作符（.）来访问结构体的成员：
student1.age = 20;  // 设置 student1 结构体的 age 成员
printf("%s\n", student1.name);  // 打印 student1 结构体的 name 成员

可以在声明结构体变量时进行初始化：
struct Student student2 = {"Alice", 22, 3.5};

可以创建指向结构体的指针，并使用箭头操作符（->）来访问成员：
struct Student *ptr = &student1;
ptr->age = 21;  // 等同于 (*ptr).age = 21;

可以创建结构体数组来存储多个结构体变量：
struct Student students[100];  // 声明一个包含 100 个 Student 结构体的数组

结构体可以作为函数的参数传递，既可以传递结构体变量，也可以传递结构体指针：
void printStudent(struct Student s) {
    printf("%s is %d years old with a GPA of %.2f\n", s.name, s.age, s.gpa);
}
void updateStudent(struct Student *s) {
    s->gpa = 4.0;
}
// 调用函数
printStudent(student1);
updateStudent(&student1);*/


/*面向对象的程序设计*/
/*类型是抽象的对象，（类定义的）变量是具体的对象*/
/*栈，数组*/
typedef unsigned int Element;
typedef struct{
    Element a[STACK_SIZE];//元素
    int n;//top
}Stack;
/*其中，Element是自定义的栈元素的数据类型，a是栈的开始地址，STACK
SIZE是预定义的栈容量，n 是栈元素的个数、同时也是栈顶的位置。是一个静态数组描述的栈*/
/*因此，当操作一个栈的时候，必须知道栈顶的位置。同时，也应该知道栈底的地址，即栈在内存中的开始地址。*/


/*在C语言中，数组名是数组的符号地址，然而结构名不是结构的符号地址，
结构变量可以像基本类型的变量一样作为一个整体来赋值。如果通过结构变量的名字访问成员变量，
那么可以使用点运算符(.)；如果通过结构指针访问成员变量，那么可以使用箭头运算符(− >)。*/



float Distance(PlanePoint *pa,PlanePoint *pb);//结构做参数时建议用指针


Status Push(Stack *pStack,Element e);//存入栈中
Status Pop(Stack *pStack,Element *pe);//出栈，出栈值出来后要赋值给参数变量指向的地方，不然在参数变量里出来就没了
Bool IsEmpty(Stack *pStack);
void InitStack(Stack* pStack);

void InitStack(Stack* pStack){
    pStack->n=0;//n=0即是将栈顶指针指向栈底，也就是说栈（这个数组的元素个数为0）是空的
 }

 /*当为一个对象(变量)赋初值时，这个对象必须存在，因此下面的用法是错误的：
1//这是初学者常犯的错误 
2 SimpleStack *pStack; 
// 定义了一个指针
3 InitStack(pStack); // 为pStack指向的对象赋初值

其中第2行只是定义了一个指针，并没有定义栈；第3行试图为pStack指向的对象赋初值，然而这个对象根本就不存在，
所以是错的。这是初学者常犯的错误，请务必理解出错的原因。正确的用法是：
1 SimpleStack stack; // 定义了一个对象
2 InitStack(&stack); // 为这个对象初始化
其中第1行定义了一个stack对象，编译器需要为它分配内存；然后第2行对它初始化，这时需要传stack的地址，这个地址就是指向stack的指针。
*/

Status ConvertNumber(unsigned decimal,unsigned base,char digitString[],int n);//2（最小是二进制）<=base<=36(10个阿拉伯数字+26个英文字母)

Bool IsEmpty(Stack *pStack){
    if(pStack->n==0){
        return TRUE;
    }
    else{
        return FALSE;
    }
}

Status ConvertNumber(unsigned decimal,unsigned base,char digitString[],int n){
    Stack stack;
    Element e;
    stack.n=0;
    if(base<2||base>36){
        return ERROR;
    }
    if(decimal==0){
        digitString[0]='0';
        digitString[1]='\0';
        return OK;
    }
    while(decimal!=0){
        e = decimal%base;
        Push(&stack,e);
        decimal/=base;
    }
    int i=0;
    while(IsEmpty(&stack)!=TRUE){
        Pop(&stack,&e);
        if(e>=0 && e<=9){
            digitString[i]=e+'0';
        }
        else if(e>=10 && e<=36){
            digitString[i]=e-10+'A';
        }
        else{
            return ERROR;
        }
        i++;
    }
}

Status Push(Stack *pStack, Element e) {
    if (pStack->n >= STACK_SIZE) {
        return ERROR; // 栈满，无法再入栈，返回错误状态
    }
    pStack->a[pStack->n] = e; // 将元素 e 存储在栈顶位置
    pStack->n++; // 栈顶指针 n 增加，表示栈中元素数量增加
    return OK; // 入栈成功，返回成功状态
}

Status Pop(Stack *pStack,Element *pe){
    if(pStack->n<=0){
        return ERROR; // 栈空，无法出栈，返回错误状态
    }
    pStack->n--;//先使其指向栈顶指针。先减少栈顶指针 n，使其指向栈顶元素。
    *pe=pStack->a[pStack->n];// 将栈顶元素的值赋给 *pe
    return OK;
}

float Distance(PlanePoint *pa,PlanePoint *pb){
    float dx,dy;
    dx =pb->x - pa->x;
    dy =pb->y - pa->y;
    return sqrt(dx*dx+dy*dy);
}

int main(){
    char s[N];
    unsigned x =137;
    ConvertNumber(x,2,s,N);
    printf("x=%d(10)=%s(2)\n",x,s);
    return 0;
    Integer i =8;
    Bool b;
    Suit suit;
    PlanePoint p1,p2,*p;
    p1.x=0;
    p1.y=0;
    p=&p2;//p2赋值给p
    p->x=3;
    p->y=4;//p2通过指针来赋值
/*指针用箭头，不是指针的话用.*/
    suit = HEART;
    b = TRUE;
    float distance;
    distance = Distance(&p1,&p2);
    printf("distance=%f\n",distance);
    printf("i=%d\n",i);
    printf("Hello\n");
    return 0;
}