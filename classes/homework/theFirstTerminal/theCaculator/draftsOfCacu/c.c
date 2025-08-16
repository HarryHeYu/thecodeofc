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

typedef struct {
    void** items;
    int top;
    int size;
} Stack;

ErrorCode initStack(Stack* s);
void destroyStack(Stack* s);
Status pushStack(Stack* s, void* item);
void* popStack(Stack* s);
int isStackEmpty(Stack* s);
int precedence(char op);
double applyOperator(double a, double b, char op);
double parseNumber(const char** expr);
ErrorCode evaluate(const char** expr, double* result);
double applyFunction(const char* funcName, double arg);

ErrorCode initStack(Stack* s) {
    s->items = (void**)malloc(INITIAL_STACK_SIZE * sizeof(void*));
    if (s->items == NULL) {
        return ERR_MEMORY_ALLOCATION;
    }
    s->top = -1;
    s->size = INITIAL_STACK_SIZE;
    return ERR_NONE;
}

void destroyStack(Stack* s) {
    while (!isStackEmpty(s)) {
        free(popStack(s));
    }
    free(s->items);
    s->items = NULL;
    s->top = -1;
    s->size = 0;
}

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

void* popStack(Stack* s) {
    if (isStackEmpty(s)) {
        fprintf(stderr, "Stack underflow!\n");
        exit(1);
    }
    return s->items[(s->top)--];
}

int isStackEmpty(Stack* s) {
    return s->top == -1;
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
        case '/': return b != 0 ? a / b : NAN;
        case '^': return pow(a, b);
        default:
            fprintf(stderr, "Invalid operator: %c\n", op);
            exit(1);
    }
}

double parseNumber(const char** expr) {
    double result = 0.0;
    int decimalPlaces = 0;
    double fraction = 0.0;

    while (isdigit(**expr)) {
        result = result * 10 + (**expr - '0');
        (*expr)++;
    }

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

ErrorCode evaluate(const char** expr, double* result) {
    if (*expr == NULL || **expr == '\0') {
        return ERR_INVALID_EXPRESSION;
    }

    Stack valueStack;
    Stack operatorStack;
    if (initStack(&valueStack) != ERR_NONE || initStack(&operatorStack) != ERR_NONE) {
        destroyStack(&valueStack);
        destroyStack(&operatorStack);
        return ERR_MEMORY_ALLOCATION;
    }

    const char* p = *expr;
    int expectOperand = 1;
    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }
        if (isdigit(*p) || *p == '.' || (*p == '-' && isdigit(p[1]) && expectOperand)) {
            double* value = (double*)malloc(sizeof(double));
            if (*p == '-') {
                p++;
                *value = -parseNumber(&p);
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
                free(popStack(&operatorStack));
            }
            p++;
            expectOperand = 0;
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
            for (int i = 0; i < strlen(funcName); i++) {
                funcName[i] = tolower(funcName[i]);
            }

            if (*p == '(') {
                p++;
                double arg;
                ErrorCode ec = evaluate(&p, &arg);
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
    *expr = p;
    return ERR_NONE;
}

int main(){
    char expr[256];
    printf("请输入表达式: ");
    if (fgets(expr, sizeof(expr), stdin) == NULL) {
        fprintf(stderr, "Error reading input.\n");
        return 1;
    }
    size_t len = strlen(expr);
    if (len > 0 && expr[len - 1] == '\n') {
        expr[len - 1] = '\0';
    }

    double result;
    const char* p = expr;
    ErrorCode errorCode = evaluate(&p, &result);
    if (errorCode != ERR_NONE) {
        fprintf(stderr, "Error evaluating expression: %d\n", errorCode);
        return 1;
    }

    printf("结果: %.6f\n", result);

    return 0;
}