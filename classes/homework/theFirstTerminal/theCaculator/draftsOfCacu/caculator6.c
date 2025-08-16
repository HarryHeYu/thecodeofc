#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

// 定义初始栈大小和增量大小
#define INITIAL_STACK_SIZE 100
#define INCREMENT_SIZE 10
// 定义圆周率的值
#define M_PI 3.14159265358979323846  

// 定义栈中项的类型
typedef enum { NUMBER, OPERATOR, PAREN, FUNCTION } ItemType;

// 定义栈中的项
typedef struct {
    union { double num; char op; } data; // 可以存储数字或操作符
    ItemType type; // 项的类型
} StackItem;

// 定义栈结构
typedef struct {
    StackItem* items; // 栈中的项数组
    int top; // 栈顶位置
    int size; // 栈的当前大小
} Stack;

// 定义状态码
typedef enum { ERROR = 0, OK = 1 } Status;

// 定义错误码
typedef enum {
    ERR_NONE, ERR_STACK_OVERFLOW, ERR_STACK_UNDERFLOW, ERR_INVALID_CHARACTER,
    ERR_INVALID_OPERATOR, ERR_DIVISION_BY_ZERO, ERR_MEMORY_ALLOCATION,
    ERR_INVALID_EXPRESSION, ERR_MULTIPLE_DECIMAL_POINTS, ERR_INVALID_FUNCTION,
    ERR_INVALID_ARGUMENT
} ErrorCode;

// 定义数学函数类型
typedef double (*MathFunction)(double);

// 定义函数结构
typedef struct { const char* name; MathFunction func; int takesDegrees; } Function;

// 定义数学函数数组
Function functions[] = {
    {"sin", sin, 1}, {"cos", cos, 1}, {"tan", tan, 1}, {"asin", asin, 0},
    {"acos", acos, 0}, {"atan", atan, 0}, {"arcsin", asin, 0},
    {"arctan", atan, 0}, {"arccos", acos, 0}, {"sqrt", sqrt, 0},
    {"log", log10, 0}, {"ln", log, 0}, {NULL, NULL, 0}
};

// 函数声明
ErrorCode initStack(Stack* s); // 初始化栈
void destroyStack(Stack* s); // 销毁栈
Status pushStack(Stack* s, StackItem item); // 将项压入栈
StackItem popStack(Stack* s); // 从栈弹出项
int isStackEmpty(Stack* s); // 检查栈是否为空
int precedence(char op); // 操作符优先级
double applyOperator(double a, double b, char op); // 应用操作符
double parseNumber(const char** expr); // 解析数字
double applyFunction(double arg, MathFunction func, int takesDegrees); // 应用函数
ErrorCode evaluate(const char* expr, double* result); // 评估表达式
int main(int argc, char* argv[]); // 主函数

// 初始化栈
ErrorCode initStack(Stack* s) {
    // 分配初始内存空间给栈
    s->items = (StackItem*)malloc(INITIAL_STACK_SIZE * sizeof(StackItem));
    // 如果内存分配失败，返回错误码
    if (!s->items) return ERR_MEMORY_ALLOCATION;
    s->top = -1; s->size = INITIAL_STACK_SIZE;
    return ERR_NONE;
}

// 销毁栈
void destroyStack(Stack* s) {
    // 释放栈内存空间
    free(s->items);
    s->items = NULL;
    s->top = -1;
    s->size = 0;
}

// 将项压入栈
Status pushStack(Stack* s, StackItem item) {
    // 如果栈满，扩展栈空间
    if (s->top >= s->size - 1) {
        StackItem* newItems = (StackItem*)realloc(s->items, (s->size + INCREMENT_SIZE) * sizeof(StackItem));
        // 如果内存重新分配失败，返回错误码
        if (!newItems) return ERROR;
        s->items = newItems;
        s->size += INCREMENT_SIZE;
    }
    s->items[++(s->top)] = item;
    return OK;
}

// 从栈弹出项
StackItem popStack(Stack* s) {
    // 如果栈为空，打印错误信息并退出
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

// 操作符优先级
int precedence(char op) {
    switch (op) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '^': return 3;
        case 'U': return 4; // 一元减号
        default: return 0;
    }
}

// 应用操作符
double applyOperator(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return b != 0 ? a / b : NAN;
        case '^': return (a < 0 && floor(b) != b) ? NAN : pow(a, b);
        case 'U': return -a; // 一元减号
        default:
            fprintf(stderr, "Invalid operator: %c\n", op);
            exit(1);
    }
}

// 解析数字
double parseNumber(const char** expr) {
    double result = 0.0, fraction = 0.0;
    int decimalPlaces = 0, hasDecimal = 0, negative = 0;

    if (**expr == '-') {
        negative = 1;
        (*expr)++;
    }

    while (isdigit(**expr) || **expr == '.') {
        if (**expr == '.') {
            if (hasDecimal) return NAN;
            hasDecimal = 1;
        } else {
            if (hasDecimal) {
                fraction += (**expr - '0') * pow(10, -++decimalPlaces);
            } else {
                result = result * 10 + (**expr - '0');
            }
        }
        (*expr)++;
    }
    result += fraction;
    return negative ? -result : result;
}

// 应用函数
double applyFunction(double arg, MathFunction func, int takesDegrees) {
    return func(takesDegrees ? arg * (M_PI / 180.0) : arg);
}

// 评估表达式
ErrorCode evaluate(const char* expr, double* result) {
    Stack valueStack, operatorStack;
    // 初始化两个栈，用于存储数值和操作符
    if (initStack(&valueStack) != ERR_NONE || initStack(&operatorStack) != ERR_NONE) return ERR_MEMORY_ALLOCATION;

    const char* p = expr;
    ItemType prevTokenType = OPERATOR;

    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }
        if (isdigit(*p) || *p == '.') {
            double value = parseNumber(&p);
            // 如果解析数字失败，销毁栈并返回错误码
            if (isnan(value)) {
                destroyStack(&valueStack);
                destroyStack(&operatorStack);
                return ERR_MULTIPLE_DECIMAL_POINTS;
            }
            // 将解析出的数字压入数值栈
            if (pushStack(&valueStack, (StackItem){.type = NUMBER, .data.num = value}) != OK) {
                destroyStack(&valueStack);
                destroyStack(&operatorStack);
                return ERR_STACK_OVERFLOW;
            }
            prevTokenType = NUMBER;
        } else if (*p == '(') {
            // 处理左括号，将其压入操作符栈
            if (pushStack(&operatorStack, (StackItem){.type = PAREN, .data.op = *p}) != OK) {
                destroyStack(&valueStack);
                destroyStack(&operatorStack);
                return ERR_STACK_OVERFLOW;
            }
            p++;
            prevTokenType = PAREN;
        } else if (*p == ')') {
            // 处理右括号，弹出操作符栈直到遇到左括号
            while (!isStackEmpty(&operatorStack) && operatorStack.items[operatorStack.top].data.op != '(') {
                if (isStackEmpty(&valueStack)) {
                    destroyStack(&valueStack);
                    destroyStack(&operatorStack);
                    return ERR_STACK_UNDERFLOW;
                }
                double b = popStack(&valueStack).data.num;
                if (isStackEmpty(&valueStack)) {
                    destroyStack(&valueStack);
                    destroyStack(&operatorStack);
                    return ERR_STACK_UNDERFLOW;
                }
                double a = popStack(&valueStack).data.num;
                char op = popStack(&operatorStack).data.op;
                double res = applyOperator(a, b, op);
                if (isnan(res)) {
                    destroyStack(&valueStack);
                    destroyStack(&operatorStack);
                    return ERR_DIVISION_BY_ZERO;
                }
                if (pushStack(&valueStack, (StackItem){.type = NUMBER, .data.num = res}) != OK) {
                    destroyStack(&valueStack);
                    destroyStack(&operatorStack);
                    return ERR_STACK_OVERFLOW;
                }
            }
            // 弹出左括号
            if (!isStackEmpty(&operatorStack)) popStack(&operatorStack);
            p++;
            prevTokenType = PAREN;
        } else if (strchr("+-*/^", *p)) {
            // 处理操作符
            char op = *p;
            if (op == '-' && (prevTokenType == OPERATOR || prevTokenType == PAREN)) op = 'U';
            while (!isStackEmpty(&operatorStack) && operatorStack.items[operatorStack.top].type == OPERATOR &&
                   precedence(operatorStack.items[operatorStack.top].data.op) >= precedence(op)) {
                if (isStackEmpty(&valueStack)) {
                    destroyStack(&valueStack);
                    destroyStack(&operatorStack);
                    return ERR_STACK_UNDERFLOW;
                }
                double b = popStack(&valueStack).data.num;
                if (isStackEmpty(&valueStack)) {
                    destroyStack(&valueStack);
                    destroyStack(&operatorStack);
                    return ERR_STACK_UNDERFLOW;
                }
                double a = popStack(&valueStack).data.num;
                char popOp = popStack(&operatorStack).data.op;
                double res = applyOperator(a, b, popOp);
                if (isnan(res)) {
                    destroyStack(&valueStack);
                    destroyStack(&operatorStack);
                    return ERR_DIVISION_BY_ZERO;
                }
                if (pushStack(&valueStack, (StackItem){.type = NUMBER, .data.num = res}) != OK) {
                    destroyStack(&valueStack);
                    destroyStack(&operatorStack);
                    return ERR_STACK_OVERFLOW;
                }
            }
            if (pushStack(&operatorStack, (StackItem){.type = OPERATOR, .data.op = op}) != OK) {
                destroyStack(&valueStack);
                destroyStack(&operatorStack);
                return ERR_STACK_OVERFLOW;
            }
            p++;
            prevTokenType = OPERATOR;
        } else if (isalpha(*p)) {
            // 处理函数
            const char* funcStart = p;
            while (isalpha(*p)) p++;
            char funcName[p - funcStart + 1];
            strncpy(funcName, funcStart, p - funcStart);
            funcName[p - funcStart] = '\0';

            int funcIndex = -1;
            for (int i = 0; functions[i].name != NULL; i++) {
                if (strcmp(funcName, functions[i].name) == 0) {
                    funcIndex = i;
                    break;
                }
            }

            if (funcIndex != -1) {
                if (*p != '(') {
                    destroyStack(&valueStack);
                    destroyStack(&operatorStack);
                    return ERR_INVALID_EXPRESSION;
                }
                p++;
                double arg;
                ErrorCode ec = evaluate(p, &arg);
                if (ec != ERR_NONE) {
                    destroyStack(&valueStack);
                    destroyStack(&operatorStack);
                    return ec;
                }
                double funcRes = applyFunction(arg, functions[funcIndex].func, functions[funcIndex].takesDegrees);
                if (pushStack(&valueStack, (StackItem){.type = NUMBER, .data.num = funcRes}) != OK) {
                    destroyStack(&valueStack);
                    destroyStack(&operatorStack);
                    return ERR_STACK_OVERFLOW;
                }
                int parenthesisLevel = 1;
                while (*p && parenthesisLevel > 0) {
                    if (*p == '(') parenthesisLevel++;
                    else if (*p == ')') parenthesisLevel--;
                    p++;
                }
                if (parenthesisLevel != 0) {
                    destroyStack(&valueStack);
                    destroyStack(&operatorStack);
                    return ERR_INVALID_EXPRESSION;
                }
                prevTokenType = NUMBER;
            } else {
                destroyStack(&valueStack);
                destroyStack(&operatorStack);
                return ERR_INVALID_FUNCTION;
            }
        } else {
            destroyStack(&valueStack);
            destroyStack(&operatorStack);
            return ERR_INVALID_CHARACTER;
        }
    }

    while (!isStackEmpty(&operatorStack)) {
        if (operatorStack.items[operatorStack.top].type == PAREN) {
            destroyStack(&valueStack);
            destroyStack(&operatorStack);
            return ERR_INVALID_EXPRESSION;
        }
        if (isStackEmpty(&valueStack)) {
            destroyStack(&valueStack);
            destroyStack(&operatorStack);
            return ERR_STACK_UNDERFLOW;
        }
        double b = popStack(&valueStack).data.num;
        if (isStackEmpty(&valueStack)) {
            destroyStack(&valueStack);
            destroyStack(&operatorStack);
            return ERR_STACK_UNDERFLOW;
        }
        double a = popStack(&valueStack).data.num;
        char op = popStack(&operatorStack).data.op;
        double res = applyOperator(a, b, op);
        if (isnan(res)) {
            destroyStack(&valueStack);
            destroyStack(&operatorStack);
            return ERR_DIVISION_BY_ZERO;
        }
        if (pushStack(&valueStack, (StackItem){.type = NUMBER, .data.num = res}) != OK) {
            destroyStack(&valueStack);
            destroyStack(&operatorStack);
            return ERR_STACK_OVERFLOW;
        }
    }

    if (isStackEmpty(&valueStack)) {
        destroyStack(&valueStack);
        destroyStack(&operatorStack);
        return ERR_INVALID_EXPRESSION;
    }

    *result = popStack(&valueStack).data.num;
    destroyStack(&valueStack);
    destroyStack(&operatorStack);
    return ERR_NONE;
}

// 主函数
int main(int argc, char* argv[]) {
    // 检查命令行参数数量
    if (argc != 3) {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }
    
    // 打开输入输出文件
    FILE* inputFile = fopen(argv[1], "r");
    FILE* outputFile = fopen(argv[2], "w");
    if (!inputFile || !outputFile) {
        fprintf(stderr, "Error opening files.\n");
        return 1;
    }
    
    char expr[256];
    double result;
    ErrorCode errorCode;
    
    // 读取输入文件中的每一行表达式
    while (fgets(expr, sizeof(expr), inputFile)) {
        // 去除表达式末尾的换行符
        if (expr[strlen(expr) - 1] == '\n') expr[strlen(expr) - 1] = '\0';
        
        // 评估表达式
        errorCode = evaluate(expr, &result);
        
        // 将结果或错误信息写入输出文件
        if (errorCode != ERR_NONE) {
            switch (errorCode) {
                case ERR_MULTIPLE_DECIMAL_POINTS:
                    fprintf(outputFile, "Error: Multiple decimal points.\n");
                    break;
                case ERR_INVALID_CHARACTER:
                    fprintf(outputFile, "Error: Invalid character.\n");
                    break;
                case ERR_DIVISION_BY_ZERO:
                    fprintf(outputFile, "Error: Division by zero.\n");
                    break;
                case ERR_INVALID_EXPRESSION:
                    fprintf(outputFile, "Error: Invalid expression.\n");
                    break;
                case ERR_INVALID_FUNCTION:
                    fprintf(outputFile, "Error: Invalid function name.\n");
                    break;
                case ERR_INVALID_ARGUMENT:
                    fprintf(outputFile, "Error: Invalid function argument.\n");
                    break;
                default:
                    fprintf(outputFile, "Error: Unknown error.\n");
                    break;
            }
        } else {
            fprintf(outputFile, "Result: %.2f\n", result);
        }
    }
    
    // 关闭输入输出文件
    fclose(inputFile);
    fclose(outputFile);
    return 0;

    
}