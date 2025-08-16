#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h> // 添加这个头文件，用于支持bool类型以及true、false关键字

#define MAXSIZE 100

// Stack for double values
typedef struct {
    double data[MAXSIZE];
    int top;
} ValueStack;

// Stack for char operators
typedef struct {
    char data[MAXSIZE];
    int top;
} OpStack;

// Function prototypes
void initValueStack(ValueStack *s);
int isValueStackEmpty(ValueStack *s);
void pushValue(ValueStack *s, double num);
double popValue(ValueStack *s);
double peekValue(ValueStack *s);

void initOpStack(OpStack *s);
int isOpStackEmpty(OpStack *s);
void pushOp(OpStack *s, char op);
char popOp(OpStack *s);
char peekOp(OpStack *s);

int precedence(char op);
void applyOperations(ValueStack *values, OpStack *ops);
void evaluateExpression(ValueStack *values, OpStack *ops, char *expr, int *index);
double evaluate(char *expr);
int main();

// Value stack functions
void initValueStack(ValueStack *s) {
    s->top = -1;
}

int isValueStackEmpty(ValueStack *s) {
    return s->top == -1;
}

void pushValue(ValueStack *s, double num) {
    if (s->top < MAXSIZE - 1) {
        s->data[++(s->top)] = num;
        printf("Pushed value: %lf\n", num);
    } else {
        printf("Stack overflow\n");
    }
}

double popValue(ValueStack *s) {
    if (!isValueStackEmpty(s)) {
        double value = s->data[(s->top)--];
        printf("Popped value: %lf\n", value);
        return value;
    } else {
        printf("Stack underflow\n");
        return -1; // Return an error value
    }
}

double peekValue(ValueStack *s) {
    if (!isValueStackEmpty(s)) {
        return s->data[s->top];
    } else {
        printf("Stack underflow\n");
        return -1; // Return an error value
    }
}

// Operator stack functions
void initOpStack(OpStack *s) {
    s->top = -1;
}

int isOpStackEmpty(OpStack *s) {
    return s->top == -1;
}

void pushOp(OpStack *s, char op) {
    if (s->top < MAXSIZE - 1) {
        s->data[++(s->top)] = op;
        printf("Pushed operator: %c\n", op);
    } else {
        printf("Stack overflow\n");
    }
}

char popOp(OpStack *s) {
    if (!isOpStackEmpty(s)) {
        char op = s->data[(s->top)--];
        printf("Popped operator: %c\n", op);
        return op;
    } else {
        printf("Stack underflow\n");
        return '\0'; // Return an error value
    }
}

char peekOp(OpStack *s) {
    if (!isOpStackEmpty(s)) {
        return s->data[s->top];
    } else {
        printf("Stack underflow\n");
        return '\0'; // Return an error value
    }
}

// Precedence of operators
int precedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case 'S': // Represents "sqrt"
            return 3;
        default:
            return 0;
    }
}

// Apply operations based on operator
void applyOperations(ValueStack *values, OpStack *ops) {
    char op = popOp(ops);
    if (op == 'S') { // Square root operation
        double num = popValue(values);
        double result = sqrt(num);
        printf("Applied sqrt to %lf, result: %lf\n", num, result);
        pushValue(values, result);
    } else { // Binary operations
        double val2 = popValue(values);
        double val1 = popValue(values);
        double result = 0;
        switch (op) {
            case '+':
                result = val1 + val2;
                break;
            case '-':
                result = val1 - val2;
                break;
            case '*':
                result = val1 * val2;
                break;
            case '/':
                if (val2!= 0)
                    result = val1 / val2;
                else {
                    printf("Division by zero error\n");
                    return;
                }
                break;
        }
        printf("Applied %c to %lf and %lf, result: %lf\n", op, val1, val2, result);
        pushValue(values, result);
    }
}

// Evaluate expression tokens
void evaluateExpression(ValueStack *values, OpStack *ops, char *expr, int *index) {
    int len = strlen(expr);
    while (*index < len) {
        if (isspace(expr[*index])) {
            (*index)++;
            continue;
        }
        if (strncmp(&expr[*index], "sqrt", 4) == 0) {
            pushOp(ops, 'S');
            *index += 4; // Skip "sqrt"
        } else if (isdigit(expr[*index])) {
            double num = 0;
            while (*index < len && isdigit(expr[*index])) {
                num = num * 10 + (expr[*index] - '0');
                (*index)++;
            }
            if (*index < len && expr[*index] == '.') {
                (*index)++;
                double decimalPlace = 0.1;
                while (*index < len && isdigit(expr[*index])) {
                    num += (expr[*index] - '0') * decimalPlace;
                    decimalPlace *= 0.1;
                    (*index)++;
                }
            }
            pushValue(values, num);
        } else if (expr[*index] == '-' && (*index + 1 < len) && (isdigit(expr[*index + 1]) || expr[*index + 1] == '.')) {
            // 处理负数情况
            (*index)++;
            double num = 0;
            bool hasDecimal = false;
            double decimalPlace = 0.1;
            while (*index < len && (isdigit(expr[*index]) || expr[*index] == '.')) {
                if (expr[*index] == '.') {
                    hasDecimal = true;
                } else if (hasDecimal) {
                    num += (expr[*index] - '0') * decimalPlace;
                    decimalPlace *= 0.1;
                } else {
                    num = num * 10 + (expr[*index] - '0');
                }
                (*index)++;
            }
            num = -num;
            pushValue(values, num);
        } else if (expr[*index] == '+' || expr[*index] == '-' ||
                   expr[*index] == '*' || expr[*index] == '/') {
            while (!isOpStackEmpty(ops) && precedence(peekOp(ops)) >= precedence(expr[*index])) {
                applyOperations(values, ops);
            }
            pushOp(ops, expr[*index]);
            (*index)++;
        } else {
            printf("Invalid character encountered: %c\n", expr[*index]);
            (*index)++;
        }
    }
}

// Evaluate the entire expression
double evaluate(char *expr) {
    ValueStack values;
    OpStack ops;
    initValueStack(&values);
    initOpStack(&ops);
    int index = 0;

    evaluateExpression(&values, &ops, expr, &index);

    while (!isOpStackEmpty(&ops)) {
        applyOperations(&values, &ops);
    }

    return popValue(&values);
}

// Main function
int main() {
    char expr[256];
    printf("Enter an expression using 'sqrt' for square root (e.g., \"sqrt10 - sqrt9\"): ");
    fgets(expr, sizeof(expr), stdin);
    expr[strcspn(expr, "\n")] = '\0'; // Remove newline character

    double result = evaluate(expr);
    if (result!= -1)
        printf("The result is: %lf\n", result);
    else
        printf("An error occurred during evaluation.\n");
    return 0;
}