/*动态栈，单栈，健壮性够了（有括号平衡检查，动态内存管理，错误处理（动态分配的），栈操作检查，运算符优先级处理，输入处理
表达式验证，内存释放，异常字符处理）
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>  // 需要包含这个头文件来使用 strchr 函数
#define STACK_INIT_SIZE 100
#define STACK_INCREMENT 10

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

// 函数声明
void initStack(Stack* s); // 初始化栈
void destroyStack(Stack* s); // 销毁栈，释放分配的内存
Status pushStack(Stack* s, StackItem item); // 向栈中压入一个元素
StackItem popStack(Stack* s); // 从栈中弹出一个元素
int isStackEmpty(Stack* s); // 检查栈是否为空
int precedence(char op); // 获取操作符的优先级
double applyOperator(double a, double b, char op); // 应用操作符对两个数值进行运算
double parseNumber(const char** expr); // 解析字符串中的数值
Status checkBalancedParentheses(const char* expr); // 检查括号是否平衡
Status validateExpression(const char* expr); // 验证表达式的有效性
double evaluate(const char* expr); // 计算给定表达式的值

// 初始化栈
void initStack(Stack* s) {
    s->items = (StackItem*)malloc(STACK_INIT_SIZE * sizeof(StackItem));
    if (s->items == NULL) {
        printf("Failed to initialize stack.\n");
        exit(1);
    }
    s->top = -1;
    s->size = STACK_INIT_SIZE;
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
    if (s->top >= s->size - 1) { // 检查栈是否已满
        StackItem* newItems = (StackItem*)realloc(s->items, (s->size + STACK_INCREMENT) * sizeof(StackItem));
        if (newItems == NULL) {
            printf("Stack overflow!\n");
            return ERROR;
        }
        s->items = newItems;
        s->size += STACK_INCREMENT;
    }
    s->items[++(s->top)] = item;
    return OK;
}

// 从栈中弹出一个元素
StackItem popStack(Stack* s) {
    if (isStackEmpty(s)) {
        printf("Stack is empty!\n");
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
        case '/': return b != 0 ? a / b : 0; // 避免除以零
        case '^': return pow(a, b);
        default:
            printf("Invalid operator: %c\n", op);
            exit(1);
    }
}

// 解析字符串中的数值
double parseNumber(const char** expr) {
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

// 检查括号是否平衡
Status checkBalancedParentheses(const char* expr) {
    int balance = 0;
    const char* p = expr;
    while (*p) {
        if (*p == '(') {
            balance++;
        } else if (*p == ')') {
            balance--;
            if (balance < 0) {
                printf("Unbalanced parentheses!\n");
                return ERROR;
            }
        }
        p++;
    }
    if (balance != 0) {
        printf("Unbalanced parentheses!\n");
        return ERROR;
    }
    return OK;
}

// 验证表达式的有效性
Status validateExpression(const char* expr) {
    if (expr == NULL || *expr == '\0') {
        printf("Invalid expression: empty or NULL.\n");
        return ERROR;
    }
    return OK;
}

// 计算给定表达式的值
double evaluate(const char* expr) {
    if (validateExpression(expr) == ERROR) {
        return 0;
    }
    if (checkBalancedParentheses(expr) == ERROR) {
        return 0;
    }

    Stack valueStack;
    Stack operatorStack;
    initStack(&valueStack);
    initStack(&operatorStack);

    const char* p = expr;
    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }
        if (isdigit(*p) || *p == '.') {
            double value = parseNumber(&p);
            StackItem item = {.type = NUMBER, .data.num = value};
            pushStack(&valueStack, item);
        } else if (*p == '(') {
            StackItem item = {.type = OPERATOR, .data.op = *p};
            pushStack(&operatorStack, item);
            p++;
        } else if (*p == ')') {
            while (!isStackEmpty(&operatorStack) && operatorStack.items[operatorStack.top].data.op != '(') {
                double b = popStack(&valueStack).data.num;
                double a = popStack(&valueStack).data.num;
                char op = popStack(&operatorStack).data.op;
                double result = applyOperator(a, b, op);
                StackItem item = {.type = NUMBER, .data.num = result};
                pushStack(&valueStack, item);
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
                double result = applyOperator(a, b, op);
                StackItem item = {.type = NUMBER, .data.num = result};
                pushStack(&valueStack, item);
            }
            StackItem item = {.type = OPERATOR, .data.op = *p};
            pushStack(&operatorStack, item);
            p++;
        } else {
            printf("Invalid character: %c\n", *p);
            exit(1);
        }
    }

    while (!isStackEmpty(&operatorStack)) {
        double b = popStack(&valueStack).data.num;
        double a = popStack(&valueStack).data.num;
        char op = popStack(&operatorStack).data.op;
        double result = applyOperator(a, b, op);
        StackItem item = {.type = NUMBER, .data.num = result};
        pushStack(&valueStack, item);
    }

    if (isStackEmpty(&valueStack)) {
        printf("Invalid expression!\n");
        destroyStack(&valueStack);
        destroyStack(&operatorStack);
        return 0;
    }
    double result = popStack(&valueStack).data.num;
    destroyStack(&valueStack);
    destroyStack(&operatorStack);
    return result;
}
int main() {
    char expr[256];
    printf("请输入表达式: ");
    if (fgets(expr, sizeof(expr), stdin) == NULL) {
        printf("Error reading input.\n");
    return 1;
    }
    // 如果存在换行符就移除
    size_t len = strlen(expr);
    if (len > 0 && expr[len - 1] == '\n') {
        expr[len - 1] = '\0';
    }
    double result = evaluate(expr);
    printf("结果: %.2f\n", result);
    return 0;
}