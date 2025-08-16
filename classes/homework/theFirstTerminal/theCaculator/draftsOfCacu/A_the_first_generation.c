#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#define STACK_INIT_SIZE 100
#define STACK_INCREMENT 10

typedef struct {
    double* base;
    double* top;
    int size;
} NumStack;

typedef struct {
    char* base;
    char* top;
    int size;
} OpStack;

typedef enum { ERROR = 0, OK = 1 } Status;

// 函数声明
void initNumStack(NumStack* s);// 初始化数值栈
void initOpStack(OpStack* s);// 初始化操作符栈

Status pushNumStack(NumStack* s, double num);
// 向数值栈中压入一个数值.. 参数 s 是指向数值栈的指针，num 是要压入的数值.. 返回值：如果操作成功返回 OK，如果失败返回 ERROR

Status pushOpStack(OpStack* s, char op);
// 向操作符栈中压入一个操作符.. 参数 s 是指向操作符栈的指针，op 是要压入的操作符..返回值：如果操作成功返回 OK，如果失败返回 ERROR

double popNumStack(NumStack* s);
// 从数值栈中弹出一个数值.. 参数 s 是指向数值栈的指针.. 返回值：栈顶的数值

char popOpStack(OpStack* s);
// 从操作符栈中弹出一个操作符.. 参数 s 是指向操作符栈的指针..返回值：栈顶的操作符

int isNumStackEmpty(NumStack* s);
// 检查数值栈是否为空.. 参数 s 是指向数值栈的指针..返回值：如果栈为空返回 1，否则返回 0

int isOpStackEmpty(OpStack* s);
// 检查操作符栈是否为空.. 参数 s 是指向操作符栈的指针.. 返回值：如果栈为空返回 1，否则返回 0

void destroyNumStack(NumStack* s);// 销毁数值栈，释放分配的内存
void destroyOpStack(OpStack* s);// 销毁操作符栈，释放分配的内存

int precedence(char op);
// 获取操作符的优先级.. 参数 op 是要获取优先级的操作符..返回值：操作符的优先级，数值越大优先级越高

double applyOperator(double a, double b, char op);
// 应用操作符对两个数值进行运算.. 参数 a 和 b 是操作数，op 是操作符..返回值：运算结果

double parseNumber(const char** expr);
// 解析字符串中的数值.. 参数 expr 是指向字符串的指针，该字符串包含一个或多个数字，可能包含小数点

void evaluateExpression(const char* expr, NumStack* values, OpStack* ops);// 计算给定表达式的值

//初始化
void initNumStack(NumStack* s) {
    s->base = (double*)malloc(sizeof(double) * STACK_INIT_SIZE);
    if (s->base == NULL) {
        printf("Failed to init NumStack.\n");
        exit(1);
    }
    s->top = s->base;
    s->size = STACK_INIT_SIZE;
}

void initOpStack(OpStack* s) {
    s->base = (char*)malloc(sizeof(char) * STACK_INIT_SIZE);
    if (s->base == NULL) {
        printf("Failed to init OpStack.\n");
        exit(1);
    }
    s->top = s->base;
    s->size = STACK_INIT_SIZE;
}

//入栈
Status pushNumStack(NumStack* s, double num) {
    if (s->top - s->base >= s->size) {
        double* newBase = (double*)realloc(s->base, sizeof(double) * (s->size + STACK_INCREMENT));
        if (newBase == NULL) {
            printf("Failed to resize NumStack.\n");
            return ERROR;
        }
        s->base = newBase;
        s->top = s->base + (s->top - s->base);
        s->size += STACK_INCREMENT;
    }
    *s->top++ = num;
    return OK;
}

Status pushOpStack(OpStack* s, char op) {
    if (s->top - s->base >= s->size) {
        char* newBase = (char*)realloc(s->base, sizeof(char) * (s->size + STACK_INCREMENT));
        if (newBase == NULL) {
            printf("Failed to resize OpStack.\n");
            return ERROR;
        }
        s->base = newBase;
        s->top = s->base + (s->top - s->base);
        s->size += STACK_INCREMENT;
    }
    *s->top++ = op;
    return OK;
}

//出栈
double popNumStack(NumStack* s) {
    if (s->top == s->base) {
        printf("NumStack is empty.\n");
        exit(1);
    }
    return *(--s->top);
}

char popOpStack(OpStack* s) {
    if (s->top == s->base) {
        printf("OpStack is empty.\n");
        exit(1);
    }
    return *(--s->top);
}

//判断栈是否为空
int isNumStackEmpty(NumStack* s) {
    return s->top == s->base;
}

int isOpStackEmpty(OpStack* s) {
    return s->top == s->base;
}

//销毁栈
void destroyNumStack(NumStack* s) {
    free(s->base);
    s->base = NULL;
    s->top = NULL;
    s->size = 0;
}

void destroyOpStack(OpStack* s) {
    free(s->base);
    s->base = NULL;
    s->top = NULL;
    s->size = 0;
}

//优先级
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

//进行一次运算
double applyOperator(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return b != 0 ? a / b : 0; // Avoid division by zero
        case '^': return pow(a, b);
        default: 
            printf("Invalid operation: %c\n", op);
            exit(1);
    }
}

//处理数字
double parseNumber(const char** expr) {
    double result = 0.0;
    double factor = 1.0;
    int hasDecimal = 0;

    while (isdigit(**expr) || **expr == '.') {
        if (**expr == '.') {
            hasDecimal = 1;
        } else {
            if (hasDecimal) {
                factor *= 0.1;
                result = result + (**expr - '0') * factor;
            } else {
                result = result * 10 + (**expr - '0');
            }
        }
        (*expr)++;
    }
    return result;
}

//计算表达式
void evaluateExpression(const char* expr, NumStack* values, OpStack* ops) {
    const char* p = expr;
    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }
        if (isdigit(*p) || *p == '.') {
            double value = parseNumber(&p);
            if (pushNumStack(values, value) == ERROR) {
                printf("Failed to push to NumStack.\n");
                exit(1);
            }
        } else if (*p == '(') {
            if (pushOpStack(ops, *p) == ERROR) {
                printf("Failed to push to OpStack.\n");
                exit(1);
            }
            p++;
        } else if (*p == ')') {
            while (!isOpStackEmpty(ops) && *(ops->top - 1) != '(') {
                double b = popNumStack(values);
                double a = popNumStack(values);
                char op = popOpStack(ops);
                if (pushNumStack(values, applyOperator(a, b, op)) == ERROR) {
                    printf("Failed to push to NumStack.\n");
                    exit(1);
                }
            }
            if (!isOpStackEmpty(ops) && *(ops->top - 1) == '(') {
                ops->top--;  // 弹出 '('
            } else {
                printf("Mismatched parentheses.\n");
                exit(1);
            }
            p++;
        } else if (strchr("+-*/^", *p)) {
            while (!isOpStackEmpty(ops) && precedence(*(ops->top - 1)) >= precedence(*p)) {
                double b = popNumStack(values);
                double a = popNumStack(values);
                char op = popOpStack(ops);
                if (pushNumStack(values, applyOperator(a, b, op)) == ERROR) {
                    printf("Failed to push to NumStack.\n");
                    exit(1);
                }
            }
            if (pushOpStack(ops, *p) == ERROR) {
                printf("Failed to push to OpStack.\n");
                exit(1);
            }
            p++;
        } else {
            printf("Invalid character: %c\n", *p);
            exit(1);
        }
    }

    while (!isOpStackEmpty(ops)) {
        double b = popNumStack(values);
        double a = popNumStack(values);
        char op = popOpStack(ops);
        if (pushNumStack(values, applyOperator(a, b, op)) == ERROR) {
            printf("Failed to push to NumStack.\n");
            exit(1);
        }
    }
}

int main() {
    char expr[STACK_INIT_SIZE];
    NumStack values;
    OpStack ops;
    initNumStack(&values);
    initOpStack(&ops);

    printf("请输入表达式: ");
    fgets(expr, sizeof(expr), stdin);

    // 如果存在换行符就移除
    size_t len = strlen(expr);
    if (len > 0 && expr[len - 1] == '\n') {
        expr[len - 1] = '\0';
    }

    evaluateExpression(expr, &values, &ops);

    if (!isNumStackEmpty(&values)) {
        printf("结果: %.10f\n", popNumStack(&values));
    } 
    else {
    printf("没有计算结果\n");
    }
    destroyNumStack(&values);
    destroyOpStack(&ops);

    return 0;
}