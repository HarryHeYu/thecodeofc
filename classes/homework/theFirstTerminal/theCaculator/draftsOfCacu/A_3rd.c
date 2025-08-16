/*单栈，动态栈，健壮性，调试*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>  // 需要包含这个头文件来使用 strchr 函数
#define INITIAL_STACK_SIZE 100
#define INCREMENT_SIZE 10
#define DEBUG 0  // 定义DEBUG宏来开启调试模式

// 定义栈中元素的数据类型
typedef enum {
    NUMBER,
    OPERATOR
} ItemType;

// 定义栈中的元素
typedef struct {
    union {
        double num;  // 用于存储数值
        char op;     // 用于存储运算符
    } data;
    ItemType type;  // 标识数据类型
} StackItem;

// 定义栈结构
typedef struct {
    StackItem* items;  // 指向栈元素的指针
    int top;          // 栈顶索引
    int size;         // 当前栈的大小
} Stack;

typedef enum { ERROR = 0, OK = 1 } Status;
typedef enum {
    ERR_NONE,
    ERR_STACK_OVERFLOW,
    ERR_STACK_UNDERFLOW,
    ERR_INVALID_CHARACTER,
    ERR_INVALID_OPERATOR,
    ERR_DIVISION_BY_ZERO,
    ERR_MEMORY_ALLOCATION,
    ERR_INVALID_EXPRESSION
} ErrorCode;

// 函数声明
ErrorCode initStack(Stack* s); // 初始化栈
void destroyStack(Stack* s); // 销毁栈，释放分配的内存
Status pushStack(Stack* s, StackItem item); // 向栈中压入一个元素
StackItem popStack(Stack* s); // 从栈中弹出一个元素
int isStackEmpty(Stack* s); // 检查栈是否为空
int precedence(char op); // 获取操作符的优先级
double applyOperator(double a, double b, char op); // 应用操作符对两个数值进行运算
double parseNumber(const char** expr); // 解析字符串中的数值
ErrorCode evaluate(const char* expr, double* result); // 计算给定表达式的值

// 初始化栈
ErrorCode initStack(Stack* s) {
    s->items = (StackItem*)malloc(INITIAL_STACK_SIZE * sizeof(StackItem));
    if (s->items == NULL) {
        return ERR_MEMORY_ALLOCATION;
    }
    s->top = -1;
    s->size = INITIAL_STACK_SIZE;
    return ERR_NONE;
}

// 销毁栈，释放分配的内存
void destroyStack(Stack* s) {
    free(s->items);
    s->items = NULL;
    s->top = -1;
    s->size = 0;
}

// 向栈中压入一个元素
Status pushStack(Stack* s, StackItem item) {
#if DEBUG
    printf("Pushing item to stack.\n");
#endif
    if (s->top >= s->size - 1) { // 检查栈是否已满
        StackItem* newItems = (StackItem*)realloc(s->items, (s->size + INCREMENT_SIZE) * sizeof(StackItem));
        if (newItems == NULL) {
            return ERROR;
        }
        s->items = newItems;
        s->size += INCREMENT_SIZE;
    }
    s->items[++(s->top)] = item;
    return OK;
}

// 从栈中弹出一个元素
StackItem popStack(Stack* s) {
#if DEBUG
    printf("Popping item from stack.\n");
#endif
    if (isStackEmpty(s)) {
        fprintf(stderr, "Stack underflow!\n");
        exit(1);
    }
    return s->items[(s->top)--];
}

// 检查栈是否为空
int isStackEmpty(Stack* s) {
    return s->top == -1;
}

// 获取操作符的优先级
int precedence(char op) {
#if DEBUG
    printf("Getting precedence of operator.\n");
#endif
    if (op == '+' || op == '-') {
        return 1;
    } else if (op == '*' || op == '/') {
        return 2;
    } else if (op == '^') {
        return 3;
    }
    return 0;
}

// 应用操作符对两个数值进行运算
double applyOperator(double a, double b, char op) {
#if DEBUG
    printf("Applying operator %c to numbers %f and %f.\n", op, a, b);
#endif
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return b != 0 ? a / b : 0; // 避免除以零
        case '^': return pow(a, b);
        default:
            fprintf(stderr, "Invalid operator: %c\n", op);
            exit(1);
    }
}

// 解析字符串中的数值
double parseNumber(const char** expr) {
#if DEBUG
    printf("Parsing number from expression.\n");
#endif
    double result = 0.0;
    int decimalPlaces = 0;
    double fraction = 0.0;
    int hasDecimal = 0;

    // 解析整数部分
    while (isdigit(**expr)) {
        result = result * 10 + (**expr - '0');
        (*expr)++;
    }

    // 解析小数部分
    if (**expr == '.') {
        hasDecimal = 1;
        (*expr)++;
        while (isdigit(**expr)) {
            fraction += (**expr - '0') * pow(10, -++decimalPlaces);
            (*expr)++;
        }
        result += fraction;
    }

    return result;
}

// 计算给定表达式的值
ErrorCode evaluate(const char* expr, double* result) {
#if DEBUG
    printf("Evaluating expression.\n");
#endif
    if (expr == NULL || *expr == '\0') {
        return ERR_INVALID_EXPRESSION;
    }

    Stack valueStack;
    Stack operatorStack;
    if (initStack(&valueStack) != ERR_NONE || initStack(&operatorStack) != ERR_NONE) {
        destroyStack(&valueStack);
        destroyStack(&operatorStack);
        return ERR_MEMORY_ALLOCATION;
    }

    const char* p = expr;
    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }
        if (isdigit(*p) || *p == '.') {
            double value = parseNumber(&p);
            StackItem item = {.type = NUMBER, .data.num = value};
            if (pushStack(&valueStack, item) != OK) {
                destroyStack(&valueStack);
                destroyStack(&operatorStack);
                return ERR_STACK_OVERFLOW;
            }
        } else if (*p == '(') {
            StackItem item = {.type = OPERATOR, .data.op = *p};
            if (pushStack(&operatorStack, item) != OK) {
                destroyStack(&valueStack);
                destroyStack(&operatorStack);
                return ERR_STACK_OVERFLOW;
            }
            p++;
        } else if (*p == ')') {
            while (!isStackEmpty(&operatorStack) && operatorStack.items[operatorStack.top].data.op != '(') {
                double b = popStack(&valueStack).data.num;
                double a = popStack(&valueStack).data.num;
                char op = popStack(&operatorStack).data.op;
                double res = applyOperator(a, b, op);
                StackItem item = {.type = NUMBER, .data.num = res};
                if (pushStack(&valueStack, item) != OK) {
                    destroyStack(&valueStack);
                    destroyStack(&operatorStack);
                    return ERR_STACK_OVERFLOW;
                }
            }
            if (!isStackEmpty(&operatorStack) && operatorStack.items[operatorStack.top].data.op == '(') {
                popStack(&operatorStack);  // 弹出 '('
            }
            p++;
        } else if (strchr("+-*/^", *p)) {
            while (!isStackEmpty(&operatorStack) && precedence(operatorStack.items[operatorStack.top].data.op) >= precedence(*p)) {
                double b = popStack(&valueStack).data.num;
                double a = popStack(&valueStack).data.num;
                char op = popStack(&operatorStack).data.op;
                double res = applyOperator(a, b, op);
                StackItem item = {.type = NUMBER, .data.num = res};
                if (pushStack(&valueStack, item) != OK) {
                    destroyStack(&valueStack);
                    destroyStack(&operatorStack);
                    return ERR_STACK_OVERFLOW;
                }
            }
            StackItem item = {.type = OPERATOR, .data.op = *p};
            if (pushStack(&operatorStack, item) != OK) {
                destroyStack(&valueStack);
                destroyStack(&operatorStack);
                return ERR_STACK_OVERFLOW;
            }
            p++;
            } 
            else {
                fprintf(stderr,"Invalid character: %c\n", *p);
                destroyStack(&valueStack);
                destroyStack(&operatorStack);
                return ERR_INVALID_CHARACTER;
                }
        }
        while (!isStackEmpty(&operatorStack)) {
        double b = popStack(&valueStack).data.num;
        double a = popStack(&valueStack).data.num;
        char op = popStack(&operatorStack).data.op;
        double res = applyOperator(a, b, op);
        StackItem item = {.type = NUMBER, .data.num = res};
        if (pushStack(&valueStack, item) != OK) {
            destroyStack(&valueStack);
            destroyStack(&operatorStack);
            return ERR_STACK_OVERFLOW;
        }
        }

        if (isStackEmpty(&valueStack)) {
            fprintf(stderr, "Invalid expression!\n");
            destroyStack(&valueStack);
            destroyStack(&operatorStack);
            return ERR_INVALID_EXPRESSION;
        }

        *result = popStack(&valueStack).data.num;
        destroyStack(&valueStack);
        destroyStack(&operatorStack);
        return ERR_NONE;
        }

int main() {
    char expr[256];
    printf("请输入表达式: ");
    if (fgets(expr, sizeof(expr), stdin) == NULL) {
    fprintf(stderr, "Error reading input.\n");
    return 1;
    }
    // 如果存在换行符就移除
    size_t len = strlen(expr);
    if (len > 0 && expr[len - 1] == '\n') {
        expr[len - 1] = '\0';
    }
    double result;
    ErrorCode errorCode = evaluate(expr, &result);
    if (errorCode != ERR_NONE) {
        fprintf(stderr, "Error evaluating expression: %d\n", errorCode);
        return 1;
    }

    printf("结果: %.2f\n", result);
    return 0;
}