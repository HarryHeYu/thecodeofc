#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<ctype.h>

#define STACK_INIT_SIZE 100
#define STACK_INCREMENT 10

typedef double StackElem; // 使用double类型以支持浮点数
typedef char OpStackElem; // 用于运算符的栈元素类型
typedef enum{FALSE = 0, TRUE = 1}Bool;
typedef enum{ERROR = 0, OK = 1}Status;

typedef struct{
    StackElem *pbase;
    StackElem *ptop;
    int size;
}Stack;

typedef struct{
    OpStackElem *pBase;
    OpStackElem *pTop;
    int opSize;
}OpStack;

typedef struct {
    double num; // 用于存储 double 类型的参数
    char op;   // 用于存储 char 类型的参数
    int type;  // 用于标识参数类型，0 表示 double，1 表示 char
} CalcParam;

Stack* StackConstruct(); // 构建栈
void StackDestruct(Stack* pStack); // 销毁栈
Status initStack(Stack* pStack, int elemSize); // 初始化栈
Status stackFree(Stack* pStack); // 释放栈
void stackClear(Stack* pStack); // 清空栈
int StackLength(Stack* pStack); // 获取栈的长度
Bool StackIsEmpty(Stack* pStack); // 判断栈是否为空
Status StackPush(Stack* pStack, StackElem elem); // 入栈
Status StackPop(Stack* pStack, StackElem* pElem); // 出栈
Status StackGetTop(Stack* pStack, StackElem* pElem); // 获取栈顶元素

OpStack* OpStackConstruct(); // 构建运算符栈
void OpStackDestruct(OpStack* pStack); // 销毁运算符栈
Status OpStackInit(OpStack* pStack, int elemSize); // 初始化运算符栈
Status OpStackPush(OpStack* pStack, OpStackElem elem); // 入栈
Status OpStackPop(OpStack* pStack, OpStackElem* pElem); // 出栈
Status OpStackGetTop(OpStack* pStack, OpStackElem* pElem); // 获取栈顶元素
Bool OpStackIsEmpty(OpStack* pStack);

double calculator(CalcParam* params, int paramCount);

Stack* StackConstruct(){
    Stack* pStack = (Stack*)malloc(sizeof(Stack));
    if(!pStack){
        printf("内存分配失败！\n");
        exit(1);
    }
    pStack->pbase = (StackElem*)malloc(STACK_INIT_SIZE * sizeof(StackElem));
    if(!pStack->pbase){
        printf("内存分配失败！\n");
}