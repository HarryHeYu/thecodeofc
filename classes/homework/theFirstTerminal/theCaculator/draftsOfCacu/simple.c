#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAXSIZE 100
#define PI 3.141592653589793

// 泛型栈定义
typedef struct {
    void *data[MAXSIZE]; // 栈中存储 void* 数据
    int top;
} GenericStack;

void initGenericStack(GenericStack *s) {
    s->top = -1;
}

int isGenericStackEmpty(GenericStack *s) {
    return s->top == -1;
}

int isGenericStackFull(GenericStack *s) {
    return s->top == MAXSIZE - 1;
}

int pushGenericStack(GenericStack *s, void *value) {
    if (isGenericStackFull(s)) {
        return 0; // 栈满，返回失败
    }
    s->data[++(s->top)] = value;
    return 1;
}

void *popGenericStack(GenericStack *s) {
    if (isGenericStackEmpty(s)) {
        return NULL; // 栈空，返回 NULL
    }
    return s->data[(s->top)--];
}

void *peekGenericStack(GenericStack *s) {
    if (isGenericStackEmpty(s)) {
        return NULL; // 栈空，返回 NULL
    }
    return s->data[s->top];
}

// 操作符优先级
int getOperatorPriority(char operator) {
    switch (operator) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '^':           return 3;
        case 'Q': case 'W': case 'E': case 'S': // sin, cos, tan, sqrt
        case 'A': case 'B': case 'C':           // asin, acos, atan
            return 4;
        default: return 0;
    }
}

// 替换函数
void replaceFunctions(char *infix) {
    char temp[MAXSIZE];
    int i = 0, j = 0;
    while (infix[i] != '\0') {
        if (strncmp(&infix[i], "sin", 3) == 0) {
            temp[j++] = 'Q';
            i += 3;
        } else if (strncmp(&infix[i], "cos", 3) == 0) {
            temp[j++] = 'W';
            i += 3;
        } else if (strncmp(&infix[i], "tan", 3) == 0) {
            temp[j++] = 'E';
            i += 3;
        } else if (strncmp(&infix[i], "sqrt", 4) == 0) {
            temp[j++] = 'S';
            i += 4;
        } else if (strncmp(&infix[i], "asin", 4) == 0) {
            temp[j++] = 'A';
            i += 4;
        } else if (strncmp(&infix[i], "acos", 4) == 0) {
            temp[j++] = 'B';
            i += 4;
        } else if (strncmp(&infix[i], "atan", 4) == 0) {
            temp[j++] = 'C';
            i += 4;
        } else if (strncmp(&infix[i], "pi", 2) == 0) {
            sprintf(&temp[j], "%.15lf", PI);
            j += strlen(&temp[j]);
            i += 2;
        } else {
            temp[j++] = infix[i++];
        }
    }
    temp[j] = '\0';
    strcpy(infix, temp);
}

// 中缀转后缀
int infixToPostfix(char *infix, char *postfix) {
    GenericStack operatorStack;
    initGenericStack(&operatorStack);
    int i = 0, j = 0;
    char token;

    replaceFunctions(infix);

    while (infix[i] != '\0') {
        token = infix[i];

        if (isdigit(token) || token == '.') {
            while (isdigit(infix[i]) || infix[i] == '.') {
                postfix[j++] = infix[i++];
            }
            postfix[j++] = ' ';
        } else if (token == '(') {
            char *op = malloc(sizeof(char));
            *op = token;
            pushGenericStack(&operatorStack, op);
            i++;
        } else if (token == ')') {
            while (!isGenericStackEmpty(&operatorStack) &&
                   *(char *)peekGenericStack(&operatorStack) != '(') {
                postfix[j++] = *(char *)popGenericStack(&operatorStack);
                postfix[j++] = ' ';
            }
            free(popGenericStack(&operatorStack)); // 弹出左括号并释放内存
            i++;
        } else {
            while (!isGenericStackEmpty(&operatorStack) &&
                   getOperatorPriority(*(char *)peekGenericStack(&operatorStack)) >= getOperatorPriority(token)) {
                postfix[j++] = *(char *)popGenericStack(&operatorStack);
                postfix[j++] = ' ';
            }
            char *op = malloc(sizeof(char));
            *op = token;
            pushGenericStack(&operatorStack, op);
            i++;
        }
    }

    while (!isGenericStackEmpty(&operatorStack)) {
        postfix[j++] = *(char *)popGenericStack(&operatorStack);
        postfix[j++] = ' ';
    }

    postfix[j] = '\0';
    return 1;
}

// 计算后缀表达式
double evaluatePostfix(char *postfix) {
    GenericStack operandStack;
    initGenericStack(&operandStack);
    int i = 0;
    double *num, left, right, result;

    while (postfix[i] != '\0') {
        if (isspace(postfix[i])) {
            i++;
            continue;
        }
        if (isdigit(postfix[i]) || postfix[i] == '.') {
            num = malloc(sizeof(double));
            sscanf(&postfix[i], "%lf", num);
            pushGenericStack(&operandStack, num);
            while (isdigit(postfix[i]) || postfix[i] == '.') i++;
        } else {
            if (postfix[i] == 'Q' || postfix[i] == 'W' || postfix[i] == 'E' ||
                postfix[i] == 'S' || postfix[i] == 'A' || postfix[i] == 'B' || postfix[i] == 'C') {
                double *value = popGenericStack(&operandStack);
                right = *value;
                free(value);
                switch (postfix[i]) {
                    case 'Q': result = sin(right); break;
                    case 'W': result = cos(right); break;
                    case 'E': result = tan(right); break;
                    case 'A': result = asin(right); break;
                    case 'B': result = acos(right); break;
                    case 'C': result = atan(right); break;
                    case 'S': result = sqrt(right); break;
                }
            } else {
                double *valueRight = popGenericStack(&operandStack);
                double *valueLeft = popGenericStack(&operandStack);
                right = *valueRight;
                left = *valueLeft;
                free(valueRight);
                free(valueLeft);
                switch (postfix[i]) {
                    case '+': result = left + right; break;
                    case '-': result = left - right; break;
                    case '*': result = left * right; break;
                    case '/': result = left / right; break;
                    case '^': result = pow(left, right); break;
                }
            }
            double *resPtr = malloc(sizeof(double));
            *resPtr = result;
            pushGenericStack(&operandStack, resPtr);
            i++;
        }
    }

    double *finalResult = popGenericStack(&operandStack);
    result = *finalResult;
    free(finalResult);
    return result;
}

int main() {
    char infix[MAXSIZE];
    char postfix[MAXSIZE];

    printf("Enter an expression (e.g., \"asin(0.5) + 1\"): ");
    fgets(infix, MAXSIZE, stdin);
    infix[strcspn(infix, "\n")] = '\0';

    if (!infixToPostfix(infix, postfix)) {
        printf("Error converting to postfix expression.\n");
        return 1;
    }

    double result = evaluatePostfix(postfix);
    printf("Result: %lf\n", result);
    return 0;
}
