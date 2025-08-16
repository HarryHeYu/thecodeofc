#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define STACK_INIT_SIZE 100
#define STACK_INCREMENT 10
#define CALCULATE_MODE_SIZE 200

typedef double StackElem; // 使用double类型以支持浮点数
typedef char OpStackElem; // 用于运算符的栈元素类型
typedef enum { ERROR = 0, OK = 1 } Status;
typedef enum { FALSE = 0, TRUE = 1 } Bool;

typedef struct {
    StackElem* pBase;
    StackElem* pTop;
    int size;
    int elemSize;
} Stack;

typedef struct {
    OpStackElem* pBase;
    OpStackElem* pTop;
    int size;
    int elemSize;
} OpStack;

typedef struct {
    double num; // 用于存储 double 类型的参数
    char op;   // 用于存储 char 类型的参数
    int type;  // 用于标识参数类型，0 表示 double，1 表示 char
} CalcParam;

// 函数声明
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

Status StackPush(Stack* pStack, StackElem elem) {
    if (StackLength(pStack) >= pStack->size) {
        StackElem* newBase = (StackElem*)realloc(pStack->pBase, (pStack->size + STACK_INCREMENT) * pStack->elemSize);
        if (newBase == NULL) {
            return ERROR;
        }
        pStack->pBase = newBase;
        pStack->pTop = pStack->pBase + StackLength(pStack);
        pStack->size += STACK_INCREMENT;
    }
    *pStack->pTop++ = elem;
    return OK;
}

Status StackPop(Stack* pStack, StackElem* pElem) {
    if (StackIsEmpty(pStack)) {
        return ERROR;
    }
    pStack->pTop--;
    *pElem = *pStack->pTop;
    return OK;
}

Status StackGetTop(Stack* pStack, StackElem* pElem) {
    if (StackIsEmpty(pStack)) {
        return ERROR;
    }
    *pElem = *(pStack->pTop - 1);
    return OK;
}

// 获取运算符的优先级
int getPriority(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;
        default:
            return 0;
    }
}

// 运算符计算函数
double operate(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return b != 0 ? a / b : 0; // 防止除以0
        case '^': return pow(a, b);
        default: return 0;
    }
}

Bool OpStackIsEmpty(OpStack* pStack) {
    return pStack->pTop == pStack->pBase;
}

double calculator(CalcParam* params, int paramCount) {
    Stack *numStack = StackConstruct();
    OpStack *opStack = OpStackConstruct(); // 用于运算符的栈

    for (int i = 0; i < paramCount; ++i) {
        if (params[i].type == 0) { // double 类型
            StackPush(numStack, params[i].num);
        } else if (params[i].type == 1) { // char 类型
            OpStackElem op = params[i].op; // 声明并初始化 op 变量
            if (op == '(') {
                OpStackPush(opStack, op);
            } else if (op == ')') {
                while (!OpStackIsEmpty(opStack) && OpStackGetTop(opStack, &op) == OK && op != '(') {
                    double a, b;
                    if (StackPop(numStack, &b) == ERROR || StackPop(numStack, &a) == ERROR) {
                        break; // 或者处理错误
                    }
                    OpStackPop(opStack, &op);
                    double result = operate(a, b, op);
                    StackPush(numStack, result);
                }
                if (!OpStackIsEmpty(opStack) && OpStackGetTop(opStack, &op) == OK && op == '(') {
                    OpStackPop(opStack, &op); // 弹出左括号
                }
            } else {
                while (!OpStackIsEmpty(opStack) && getPriority(OpStackGetTop(opStack, &op)) >= getPriority(op)) {
                    double a, b;
                    if (StackPop(numStack, &b) == ERROR || StackPop(numStack, &a) == ERROR) {
                        break; // 或者处理错误
                    }
                    OpStackPop(opStack, &op);
                    double result = operate(a, b, op);
                    StackPush(numStack, result);
                }
                OpStackPush(opStack, op);
            }
        }
    }

    while (!OpStackIsEmpty(opStack)) {
        OpStackElem op;
        if (OpStackPop(opStack, &op) == ERROR) {
            break; // 或者处理错误
        }
        double a, b;
        if (StackPop(numStack, &b) == ERROR || StackPop(numStack, &a) == ERROR) {
            break; // 或者处理错误
        }
        double result = operate(a, b, op);
        StackPush(numStack, result);
    }

    double result = 0.0;
    if (StackPop(numStack, &result) == ERROR) {
        // 处理错误
    }
    StackDestruct(numStack);
    OpStackDestruct(opStack); // 释放运算符栈
    return result;
}


int main() {
    char calculateMode[CALCULATE_MODE_SIZE];
    CalcParam params[CALCULATE_MODE_SIZE];
    int paramCount = 0;

    printf("请输入计算式：");
    if (fgets(calculateMode, CALCULATE_MODE_SIZE, stdin) != NULL) {
        calculateMode[strcspn(calculateMode, "\n")] = '\0';
        // 解析计算式
        char *token = strtok(calculateMode, " ");
        while (token != NULL) {
            params[paramCount++] = atof(token);
            token = strtok(NULL, " ");
        }
    }

    double result = calculate(params, paramCount);
    printf("计算结果为：%.2f\n", result);
    return 0;
}