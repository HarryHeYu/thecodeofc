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
double parseNumber(const char** expr);
ErrorCode evaluate(const char* expr, double* result);
double applyFunction(const char* funcName, double arg);

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
double parseNumber(const char** expr) {
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

// 计算给定表达式的值
ErrorCode evaluate(const char* expr, double* result) {
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
    int expectOperand = 1; // 用于判断是否期待操作数，初始为1表示第一个字符应为操作数
    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }
        if (isdigit(*p) || *p == '.' || (*p == '-' && isdigit(p[1]) && expectOperand)) {
            double* value = (double*)malloc(sizeof(double));
            if (*p == '-') {
                p++; // 移动到下一个字符
                (*value) = -parseNumber(&p); // 处理负数
            } else {
                *value = parseNumber(&p);
            }
            if (pushStack(&valueStack, value) != OK) {
                free(value);
                destroyStack(&valueStack);
                destroyStack(&operatorStack);
                return ERR_STACK_OVERFLOW;
            }
            expectOperand = 0;
        } else if (strncmp(p, "pi", 2) == 0 || strncmp(p, "PI", 2) == 0) {
            double* value = (double*)malloc(sizeof(double));
            *value = PI;
            if (pushStack(&valueStack, value) != OK) {
                free(value);
                destroyStack(&valueStack);
                destroyStack(&operatorStack);
                return ERR_STACK_OVERFLOW;
            }
            p += 2;
            expectOperand = 0;
        } else if (*p == '(') {
            char* op = (char*)malloc(sizeof(char));
            *op = *p;
            if (pushStack(&operatorStack, op) != OK) {
                free(op);
                destroyStack(&valueStack);
                destroyStack(&operatorStack);
                return ERR_STACK_OVERFLOW;
            }
            p++;
            expectOperand = 1;
        } else if (*p == ')') {
            while (!isStackEmpty(&operatorStack) && *((char*)operatorStack.items[operatorStack.top]) != '(') {
                if (valueStack.top < 1) {
                    destroyStack(&valueStack);
                    destroyStack(&operatorStack);
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
                    destroyStack(&valueStack);
                    destroyStack(&operatorStack);
                    return ERR_STACK_OVERFLOW;
                }
            }
            if (!isStackEmpty(&operatorStack) && *((char*)operatorStack.items[operatorStack.top]) == '(') {
                free(popStack(&operatorStack));  // 弹出 '('
            }
            p++;
            expectOperand = 0;
        } else if (*p == '-' && expectOperand) {
            double* value = (double*)malloc(sizeof(double));
            *value = -1.0;
            if (pushStack(&valueStack, value) != OK) {
                free(value);
                destroyStack(&valueStack);
                destroyStack(&operatorStack);
                return ERR_STACK_OVERFLOW;
            }
            char* op = (char*)malloc(sizeof(char));
            *op = '*';
            if (pushStack(&operatorStack, op) != OK) {
                free(op);
                free(value);
                destroyStack(&valueStack);
                destroyStack(&operatorStack);
                return ERR_STACK_OVERFLOW;
            }
            p++;
            expectOperand = 1;
            continue;
        } else if (strchr("+-*/^", *p)) {
            while (!isStackEmpty(&operatorStack) && precedence(*((char*)operatorStack.items[operatorStack.top])) >= precedence(*p)) {
                if (valueStack.top < 1) {
                    destroyStack(&valueStack);
                    destroyStack(&operatorStack);
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
                    destroyStack(&valueStack);
                    destroyStack(&operatorStack);
                    return ERR_STACK_OVERFLOW;
                }
            }
            char* opPtr = (char*)malloc(sizeof(char));
            *opPtr = *p;
            if (pushStack(&operatorStack, opPtr) != OK) {
                free(opPtr);
                destroyStack(&valueStack);
                destroyStack(&operatorStack);
                return ERR_STACK_OVERFLOW;
            }
            p++;
            expectOperand = 1;
        } else if (isalpha(*p)) {
            const char* funcStart = p;
            while (isalpha(*p)) p++;
            char funcName[10];
            strncpy(funcName, funcStart, p - funcStart);
            funcName[p - funcStart] = '\0';

            if (*p == '(') {
                p++;
                double arg;
                ErrorCode ec = evaluate(p, &arg);
                if (ec != ERR_NONE) {
                    destroyStack(&valueStack);
                    destroyStack(&operatorStack);
                    return ec;
                }
                double res = applyFunction(funcName, arg);
                double* resultValue = (double*)malloc(sizeof(double));
                *resultValue = res;
                if (pushStack(&valueStack, resultValue) != OK) {
                    free(resultValue);
                    destroyStack(&valueStack);
                    destroyStack(&operatorStack);
                    return ERR_STACK_OVERFLOW;
                }
                while (*p && *p != ')') p++;
                if (*p == ')') p++;
                expectOperand = 0;
            } else {
                fprintf(stderr, "Invalid function call: %s\n", funcName);
                destroyStack(&valueStack);
                destroyStack(&operatorStack);
                return ERR_INVALID_FUNCTION;
            }
        } else {
            fprintf(stderr, "Invalid character: %c\n", *p);
            destroyStack(&valueStack);
            destroyStack(&operatorStack);
            return ERR_INVALID_CHARACTER;
        }
    }

    while (!isStackEmpty(&operatorStack)) {
        if (valueStack.top < 1) {
            destroyStack(&valueStack);
            destroyStack(&operatorStack);
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

    *result = *((double*)popStack(&valueStack));
    destroyStack(&valueStack);
    destroyStack(&operatorStack);
    return ERR_NONE;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }
    
    FILE* inputFile = fopen(argv[1], "r");
    FILE* outputFile = fopen(argv[2], "w");

    if (inputFile == NULL || outputFile == NULL) {
        fprintf(stderr, "Error opening files.\n");
        return 1;
    }

    char expr[256];
    double result;
    ErrorCode errorCode;
    while (fgets(expr, sizeof(expr), inputFile)) {
        size_t len = strlen(expr);
        if (len > 0 && expr[len - 1] == '\n') {
            expr[len - 1] = '\0';
        }

        // 忽略空行
        if (len == 0 || expr[0] == '\0') {
            continue;
        }

        errorCode = evaluate(expr, &result);
        if (errorCode != ERR_NONE) {
            fprintf(outputFile, "Error evaluating expression: %d\n", errorCode);
        } else {
            fprintf(outputFile, "结果: %.6f\n", result);
        }
    }

    fclose(inputFile);
    fclose(outputFile);
    return 0;
}
