#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#define INITIAL_STACK_SIZE 100
#define INCREMENT_SIZE 10
#define PI 3.141592653589793

typedef enum { ERROR = 0, OK = 1 } Status;
typedef enum {
    ERR_NONE,
    ERR_STACK_OVERFLOW,
    ERR_STACK_UNDERFLOW,
    ERR_INVALID_CHARACTER,
    ERR_INVALID_OPERATOR,
    ERR_DIVISION_BY_ZERO,
    ERR_MEMORY_ALLOCATION,
    ERR_INVALID_EXPRESSION,
    ERR_INVALID_FUNCTION
} ErrorCode;

// 定义栈结构
typedef struct {
    void** items;  // 泛型指针数组
    int top;       // 栈顶索引
    int size;      // 当前栈的大小
} Stack;

// 函数声明
ErrorCode initStack(Stack* s);
void destroyStack(Stack* s);
Status pushStack(Stack* s, void* item);
void* popStack(Stack* s);
int isStackEmpty(Stack* s);
int precedence(char op);
double applyOperator(double a, double b, char op);
double parseNumber(char** expr);
ErrorCode evaluate(char** expr, double* result);
double applyFunction(const char* funcName, double arg);
ErrorCode parseFunction(char** expr, char* funcName);

// 初始化栈
ErrorCode initStack(Stack* s) {
    s->items = (void**)malloc(INITIAL_STACK_SIZE * sizeof(void*));
    if (s->items == NULL) {
        return ERR_MEMORY_ALLOCATION;
    }
    s->top = -1;
    s->size = INITIAL_STACK_SIZE;
    return ERR_NONE;
}

// 销毁栈，释放分配的内存
void destroyStack(Stack* s) {
    while (!isStackEmpty(s)) {
        free(popStack(s));
    }
    free(s->items);
    s->items = NULL;
    s->top = -1;
    s->size = 0;
}

// 向栈中压入一个元素
Status pushStack(Stack* s, void* item) {
    if (s->top >= s->size - 1) {
        void** newItems = (void**)realloc(s->items, (s->size + INCREMENT_SIZE) * sizeof(void*));
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
void* popStack(Stack* s) {
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
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return b != 0 ? a / b : NAN; // 避免除以零产生nan
        case '^': return pow(a, b);
        default:
            fprintf(stderr, "Invalid operator: %c\n", op);
            exit(1);
    }
}

// 解析字符串中的数值
double parseNumber(char** expr) {
    double result = 0.0;
    int decimalPlaces = 0;
    double fraction = 0.0;

    // 解析整数部分
    while (isdigit(**expr)) {
        result = result * 10 + (**expr - '0');
        (*expr)++;
    }

    // 解析小数部分
    if (**expr == '.') {
        (*expr)++;
        while (isdigit(**expr)) {
            fraction += (**expr - '0') * pow(10, -++decimalPlaces);
            (*expr)++;
        }
        result += fraction;
    }

    return result;
}

// 应用数学函数
double applyFunction(const char* funcName, double arg) {
    if (strcmp(funcName, "sin") == 0) return sin(arg);
    if (strcmp(funcName, "cos") == 0) return cos(arg);
    if (strcmp(funcName, "tan") == 0) return tan(arg);
    if (strcmp(funcName, "cot") == 0) return 1.0 / tan(arg);
    if (strcmp(funcName, "asin") == 0) return asin(arg);
    if (strcmp(funcName, "acos") == 0) return acos(arg);
    if (strcmp(funcName, "atan") == 0) return atan(arg);
    if (strcmp(funcName, "acot") == 0) return atan(1.0 / arg);
    if (strcmp(funcName, "sqrt") == 0) return sqrt(arg);
    fprintf(stderr, "Invalid function: %s\n", funcName);
    exit(1);
}

// 解析函数名
ErrorCode parseFunction(char** expr, char* funcName) {
    const char* funcStart = *expr;
    while (isalpha(**expr)) (*expr)++;  // 跳过字母字符

    int len = *expr - funcStart;
    if (len > 0 && len < 10) {
        strncpy(funcName, funcStart, len);
        funcName[len] = '\0';
        return ERR_NONE;
    }
    return ERR_INVALID_FUNCTION;
}

// 计算给定表达式的值
ErrorCode evaluate(char** expr, double* result) {
    Stack valueStack;
    Stack operatorStack;
    if (initStack(&valueStack) != ERR_NONE || initStack(&operatorStack) != ERR_NONE) {
        return ERR_MEMORY_ALLOCATION;
    }

    char* p = *expr;
    int expectOperand = 1;  // 初始为1，表示第一个字符应该是操作数

    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (isdigit(*p) || (*p == '-' && isdigit(p[1]) && expectOperand)) {
            double* value = (double*)malloc(sizeof(double));
            *value = parseNumber(&p);
            if (pushStack(&valueStack, value) != OK) {
                free(value);
                return ERR_STACK_OVERFLOW;
            }
            expectOperand = 0;
        } 
        // 函数处理
        else if (isalpha(*p)) {
            char funcName[10];
            if (parseFunction(&p, funcName) == ERR_NONE) {
                if (*p == '(') {
                    p++;  // 跳过 '('
                    double arg;
                    ErrorCode ec = evaluate(&p, &arg);  // 递归解析括号内的表达式
                    if (ec != ERR_NONE) return ec;
                    double res = applyFunction(funcName, arg);
                    double* resultValue = (double*)malloc(sizeof(double));
                    *resultValue = res;
                    if (pushStack(&valueStack, resultValue) != OK) {
                        free(resultValue);
                        return ERR_STACK_OVERFLOW;
                    }
                    expectOperand = 0;
                } else {
                    return ERR_INVALID_FUNCTION;
                }
            }
        }
        // 运算符处理
        else if (strchr("+-*/^", *p)) {
            while (!isStackEmpty(&operatorStack) && precedence(*((char*)operatorStack.items[operatorStack.top])) >= precedence(*p)) {
                if (valueStack.top < 1) {
                    return ERR_STACK_UNDERFLOW;
                }
                double* b = (double*)popStack(&valueStack);
                double* a = (double*)popStack(&valueStack);
                char op = *((char*)popStack(&operatorStack));
                double res = applyOperator(*a, *b, op);
                free(a);
                free(b);
                double* resultValue = (double*)malloc(sizeof(double));
                *resultValue = res;
                if (pushStack(&valueStack, resultValue) != OK) {
                    free(resultValue);
                    return ERR_STACK_OVERFLOW;
                }
            }
            char* opPtr = (char*)malloc(sizeof(char));
            *opPtr = *p;
            if (pushStack(&operatorStack, opPtr) != OK) {
                free(opPtr);
                return ERR_STACK_OVERFLOW;
            }
            p++;
            expectOperand = 1;
        }
        // 括号处理
        else if (*p == '(') {
            char* op = (char*)malloc(sizeof(char));
            *op = *p;
            if (pushStack(&operatorStack, op) != OK) {
                free(op);
                return ERR_STACK_OVERFLOW;
            }
            p++;
            expectOperand = 1;
        }
        // 其他情况
        else {
            return ERR_INVALID_CHARACTER;
        }
    }

    while (!isStackEmpty(&operatorStack)) {
        if (valueStack.top < 1) return ERR_STACK_UNDERFLOW;
        double* b = (double*)popStack(&valueStack);
        double* a = (double*)popStack(&valueStack);
        char op = *((char*)popStack(&operatorStack));
        double res = applyOperator(*a, *b, op);
        free(a);
        free(b);
        double* resultValue = (double*)malloc(sizeof(double));
        *resultValue = res;
        if (pushStack(&valueStack, resultValue) != OK) {
            free(resultValue);
            return ERR_STACK_OVERFLOW;
        }
    }

    if (isStackEmpty(&valueStack)) {
        return ERR_INVALID_EXPRESSION;
    }

    *result = *((double*)popStack(&valueStack));
    return ERR_NONE;
}

int main() {
    char expr[] = "tan(PI/4) + sin(PI/2)";
    double result;
    char* exprPtr = expr;
    if (evaluate(&exprPtr, &result) == ERR_NONE) {
        printf("Result: %.6f\n", result);
    } else {
        printf("Invalid expression\n");
    }
    return 0;
}
