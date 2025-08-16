#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#define STACK_INIT_SIZE 100

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

// 数值栈和运算符栈的初始化、销毁、扩展、检查空、入栈、出栈、运算符优先级判断和表达式计算函数声明
void initNumStack(NumStack* s);
void initOpStack(OpStack* s);
void pushNumStack(NumStack* s, double num);
void pushOpStack(OpStack* s, char op);
double popNumStack(NumStack* s);
char popOpStack(OpStack* s);
int isNumStackEmpty(NumStack* s);
int isOpStackEmpty(OpStack* s);
void destroyNumStack(NumStack* s);
void destroyOpStack(OpStack* s);
int precedence(char op);
double applyOperator(double a, double b, char op);
double parseNumber(const char** expr);
void evaluateExpression(const char* expr, NumStack* values, OpStack* ops);

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

void pushNumStack(NumStack* s, double num) {
    if (s->top - s->base >= s->size) {
        s->base = (double*)realloc(s->base, sizeof(double) * (s->size * 2));
        if (s->base == NULL) {
            printf("Failed to resize NumStack.\n");
            exit(1);
        }
        s->top = s->base + (s->top - s->base);
        s->size *= 2;
    }
    *(s->top) = num;
    s->top++;
}

void pushOpStack(OpStack* s, char op) {
    if (s->top - s->base >= s->size) {
        s->base = (char*)realloc(s->base, sizeof(char) * (s->size * 2));
        if (s->base == NULL) {
            printf("Failed to resize OpStack.\n");
            exit(1);
        }
        s->top = s->base + (s->top - s->base);
        s->size *= 2;
    }
    *(s->top) = op;
    s->top++;
}

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

int isNumStackEmpty(NumStack* s) {
    return s->top == s->base;
}

int isOpStackEmpty(OpStack* s) {
    return s->top == s->base;
}

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

void evaluateExpression(const char* expr, NumStack* values, OpStack* ops) {
    const char* p = expr;
    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }
        if (isdigit(*p) || *p == '.') {
            double value = parseNumber(&p);
            pushNumStack(values, value);
        } else if (*p == '(') {
            pushOpStack(ops, *p);
            p++;
        } else if (*p == ')') {
            while (!isOpStackEmpty(ops) && *(ops->top - 1) != '(') {
                double b = popNumStack(values);
                double a = popNumStack(values);
                char op = popOpStack(ops);
                pushNumStack(values, applyOperator(a, b, op));
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
                pushNumStack(values, applyOperator(a, b, op));
            }
            pushOpStack(ops, *p);
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
        pushNumStack(values, applyOperator(a, b, op));
    }
}

int main() {
    char expr[256];
    NumStack values;
    OpStack ops;
    initNumStack(&values);
    initOpStack(&ops);

    printf("请输入表达式: ");
    fgets(expr, sizeof(expr), stdin);

    // Remove newline character if present
    size_t len = strlen(expr);
    if (len > 0 && expr[len - 1] == '\n') {
        expr[len - 1] = '\0';
    }

    evaluateExpression(expr, &values, &ops);

    if (!isNumStackEmpty(&values)) {
        printf("结果: %.10f\n", popNumStack(&values));
    } else {
        printf("没有计算结果\n");
    }

    destroyNumStack(&values);
    destroyOpStack(&ops);

    return 0;
}