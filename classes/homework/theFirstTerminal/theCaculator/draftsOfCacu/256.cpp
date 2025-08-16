#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>  // 需要包含这个头文件来使用 strchr 函数

#define MAX_STACK_SIZE 100

// 初始化栈
void initStack(Stack *s);
// 检查栈是否为空
int isEmpty(Stack *s);
// 检查栈是否已满
int isFull(Stack *s);
// 向栈中压入一个值
void push(Stack *s, double value);
// 从栈中弹出一个值
double pop(Stack *s);
// 查看栈顶的值，但不弹出
double peek(Stack *s);
// 获取运算符的优先级
int precedence(char op);
// 应用运算符进行计算
double applyOperator(double a, double b, char op);
// 解析字符串中的数字（支持小数）
double parseNumber(const char **expr);
// 计算给定表达式的值
double evaluate(const char *expr);

// 栈结构
typedef struct {
    double data[MAX_STACK_SIZE];
    int top;
} Stack;

// 栈操作函数
void initStack(Stack *s) {
    s->top = -1;
}

int isEmpty(Stack *s) {
    return s->top == -1;
}

int isFull(Stack *s) {
    return s->top == MAX_STACK_SIZE - 1;
}

void push(Stack *s, double value) {
    if (isFull(s)) {
        printf("栈溢出!\n");
        exit(1);
    }
    s->data[++(s->top)] = value;
}

double pop(Stack *s) {
    if (isEmpty(s)) {
        printf("栈为空!\n");
        exit(1);
    }
    return s->data[(s->top)--];
}

double peek(Stack *s) {
    if (isEmpty(s)) {
        printf("栈为空!\n");
        exit(1);
    }
    return s->data[s->top];
}

// 运算符优先级
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

// 运算函数
double applyOperator(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '^': return pow(a, b);
        default:
            printf("无效的操作符: %c\n", op);
            exit(1);
    }
}

// 解析数字（支持小数）
double parseNumber(const char **expr) {
    double result = 0.0;
    double factor = 1.0;
    
    while (isdigit(**expr)) {
        result = result * 10 + (**expr - '0');
        (*expr)++;
    }
    
    if (**expr == '.') {
        (*expr)++;
        while (isdigit(**expr)) {
            factor /= 10;
            result += (**expr - '0') * factor;
            (*expr)++;
        }
    }
    
    return result;
}

// 处理表达式
double evaluate(const char *expr) {
    Stack values, ops;
    initStack(&values);
    initStack(&ops);
    
    while (*expr) {
        if (isspace(*expr)) {
            expr++;
            continue;
        }
        
        if (isdigit(*expr) || *expr == '.') {
            double value = parseNumber(&expr);
            push(&values, value);
        } else if (*expr == '(') {
            push(&ops, *expr);
            expr++;
        } else if (*expr == ')') {
            while (!isEmpty(&ops) && peek(&ops) != '(') {
                double b = pop(&values);
                double a = pop(&values);
                char op = (char) pop(&ops);
                push(&values, applyOperator(a, b, op));
            }
            pop(&ops);  // 弹出 '('
            expr++;
        } else if (strchr("+-*/^", *expr)) {  // 需要包含 <string.h> 才能使用 strchr
            while (!isEmpty(&ops) && precedence(peek(&ops)) >= precedence(*expr)) {
                double b = pop(&values);
                double a = pop(&values);
                char op = (char) pop(&ops);
                push(&values, applyOperator(a, b, op));
            }
            push(&ops, *expr);
            expr++;
        } else {
            printf("无效字符: %c\n", *expr);
            exit(1);
        }
    }
    
    while (!isEmpty(&ops)) {
        double b = pop(&values);
        double a = pop(&values);
        char op = (char) pop(&ops);
        push(&values, applyOperator(a, b, op));
    }
    
    return pop(&values);
}

int main() {
    char expr[256];
    printf("请输入表达式: ");
    fgets(expr, sizeof(expr), stdin);
    
    double result = evaluate(expr);
    
    printf("结果: %.2f\n", result);
    
    return 0;
}

