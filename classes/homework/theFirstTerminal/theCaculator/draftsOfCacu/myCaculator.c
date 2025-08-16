#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define STACK_INIT_SIZE 100
#define STACK_INCREMENT 10
#define caculateModeSize 200

typedef double StackElem; // 使用double类型以支持浮点数
typedef enum { ERROR = 0, OK = 1 } Status;
typedef enum { FALSE = 0, TRUE = 1 } Bool;

typedef struct {
    StackElem* pBase;
    StackElem* pTop;
    int size;
    int elemSize;
} Stack;

// 函数声明
Stack* StackConstruct(); // 构建栈
void StackDestruct(Stack* pStack); // 销毁栈
Status initStack(Stack* pStack, int elemSize); // 初始化栈
Status stackFree(Stack* pStack); // 释放栈
void stackClear(Stack* pStack); // 清空栈
int StackLength(Stack* pStack); // 获取栈的长度
Bool StackIsEmpty(Stack* pStack); // 判断栈是否为空
Status StackPush(Stack* pStack, StackElem pElem); // 入栈
Status StackPop(Stack *pStack, StackElem *pElem);// 出栈
Status StackGetTop(Stack* pStack,StackElem* pElem);// 获取栈顶元素
Status StackTraverse(Stack* pStack,Status(*visit)());//遍历栈\

double caculator(const char *cacuMode);

// 函数实现
Stack* StackConstruct() {
    Stack* pStack = (Stack*)malloc(sizeof(Stack));
    if (pStack == NULL) {
        return NULL;
    }
    if (initStack(pStack, sizeof(StackElem)) == ERROR) {
        free(pStack);
        return NULL;
    }
    return pStack;
}

void StackDestruct(Stack* pStack) {
    stackFree(pStack);
    free(pStack);
}

Status initStack(Stack* pStack, int elemSize) {
    pStack->pBase = malloc(STACK_INIT_SIZE * elemSize);
    if (pStack->pBase == NULL) {
        return ERROR;
    }
    pStack->pTop = pStack->pBase;
    pStack->size = STACK_INIT_SIZE;
    pStack->elemSize = elemSize;
    return OK;
}

Status stackFree(Stack* pStack) {
    free(pStack->pBase);
    pStack->pBase = NULL;
    pStack->pTop = NULL;
    pStack->size = 0;
    return OK;
}

void stackClear(Stack* pStack) {
    pStack->pTop = pStack->pBase;
}

int StackLength(Stack* pStack) {
    return (pStack->pTop - pStack->pBase) / (pStack->elemSize);
}

Bool StackIsEmpty(Stack* pStack) {
    return pStack->pTop == pStack->pBase;
}

Status StackPush(Stack* pStack, StackElem pElem) {
    if (StackLength(pStack) >= pStack->size) {
        StackElem* newBase = (StackElem*)realloc(pStack->pBase, (pStack->size + STACK_INCREMENT) * pStack->elemSize);
        if (newBase == NULL) {
            return ERROR;
        }
        pStack->pBase = newBase;
        pStack->pTop = pStack->pBase + StackLength(pStack);
        pStack->size += STACK_INCREMENT;
    }
    *pStack->pTop++ = pElem;
    return OK;
}

Status StackPop(Stack *pStack, StackElem *pElem){
    if(StackIsEmpty(pStack)){
        return ERROR;
    }
    pStack->pTop--;
    *pElem = *(--pStack->pTop);
}

Status StackTraverse(Stack* pStack,Status(*visit)()){
    int i,n;
    n=StackLength(pStack);
    for(i=0;i<n;i++){
        if(visit(&pStack->pBase[i])==ERROR){////不加&的话就只能读不能写
            return ERROR;
        }
    }
    return OK;
}

Status StackGetTop(Stack* pStack,StackElem* pElem){
    if(StackIsEmpty(pStack)){
        return ERROR;
    }
    *pElem=*(pStack->pTop-1);
    return OK;
}

int precede(char op1, char op2) {
    int priority[][7] = {
        {'+', '-', '*', '/', '(', ')', '#'},
        {1, 1, -1, -1, -1, 1, 1},
        {1, 1, -1, -1, -1, 1, 1},
        {1, 1, 1, 1, -1, 1, 1},
        {1, 1, 1, 1, -1, 1, 1},
        {-1, -1, -1, -1, -1, 0, 2},
        {1, 1, 1, 1, -1, 1, 0}
    };
    int row = 0, col = 0;
    switch (op1) {
        case '+': row = 0; break;
        case '-': row = 1; break;
        case '*': row = 2; break;
        case '/': row = 3; break;
        case '(': row = 4; break;
        case ')': row = 5; break;
        case '#': row = 6; break;
    }
    switch (op2) {
        case '+': col = 0; break;
        case '-': col = 1; break;
        case '*': col = 2; break;
        case '/': col = 3; break;
        case '(': col = 4; break;
        case ')': col = 5; break;
        case '#': col = 6; break;
    }
    return priority[row][col];
}

double caculator(const char *caculateMode) {
    Stack *numStack = StackConstruct();
    Stack *opStack = StackConstruct();
    char *token;
    const char *delimiter = " ";
    char *copyMode = strdup(caculateMode);
    char opStackTop;
    
    StackPush(opStack, '#'); // 初始化操作符栈

    token = strtok(copyMode, delimiter);
    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            double num = atof(token);
            StackPush(numStack, num);
        } else {
            StackGetTop(opStack, &opStackTop);
            while (precede(opStackTop, token[0]) >= 0 && opStackTop != '(') {
                StackPop(opStack, &opStackTop);
                double op2 = 0.0, op1 = 0.0;
                StackPop(numStack, &op2);
                StackPop(numStack, &op1);
                switch (opStackTop) {
                    case '+': StackPush(numStack, op1 + op2); break;
                    case '-': StackPush(numStack, op1 - op2); break;
                    case '*': StackPush(numStack, op1 * op2); break;
                    case '/': StackPush(numStack, op1 / op2); break;
                    case '^': StackPush(numStack, pow(op1, op2)); break;
                }
                StackGetTop(opStack, &opStackTop);
            }
            if (token[0] != ')') {
                StackPush(opStack, token[0]);
            } else {
                StackPop(opStack, &opStackTop); // 忽略右括号
            }
        }
        token = strtok(NULL, delimiter);
    }

    // 清理剩余的操作符
    while (opStackTop != '#') {
        StackPop(opStack, &opStackTop);
        double op2 = 0.0, op1 = 0.0;
        StackPop(numStack, &op2);
        StackPop(numStack, &op1);
        switch (opStackTop) {
            case '+': StackPush(numStack, op1 + op2); break;
            case '-': StackPush(numStack, op1 - op2); break;
            case '*': StackPush(numStack, op1 * op2); break;
            case '/': StackPush(numStack, op1 / op2); break;
            case '^': StackPush(numStack, pow(op1, op2)); break;
        }
        StackGetTop(opStack, &opStackTop);
    }

    double result = 0.0;
    StackPop(numStack, &result);
    StackDestruct(numStack);
    StackDestruct(opStack);
    free(copyMode);
    return result;
}

int main(){
    char caculateMode[caculateModeSize];
    printf("请输入计算式：");
    if(fgets(caculateMode, caculateModeSize, stdin) != NULL){
        caculateMode[strcspn(caculateMode, "\n")] = '\0';
        double result = caculator(caculateMode);
        printf("计算结果为：%f\n", result);
    }
    else{
        printf("输入错误\n");
    }
    return 0;
}