#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#define INITIAL_STACK_SIZE 100
#define INCREMENT_SIZE 10

typedef enum {
    NUMBER,
    OPERATOR,
    PAREN,
    FUNCTION
} ItemType;

typedef struct {
    union {
        double num;
        char op;
    } data;
    ItemType type;
} StackItem;

typedef struct {
    StackItem* items;
    int top;
    int size;
} Stack;

typedef enum { ERROR = 0, OK = 1 } Status;

typedef enum {
    ERR_NONE, ERR_STACK_OVERFLOW,
    ERR_STACK_UNDERFLOW, ERR_INVALID_CHARACTER,
    ERR_INVALID_OPERATOR, ERR_DIVISION_BY_ZERO,
    ERR_MEMORY_ALLOCATION, ERR_INVALID_EXPRESSION,
    ERR_MULTIPLE_DECIMAL_POINTS, ERR_INVALID_FUNCTION,
    ERR_INVALID_ARGUMENT
} ErrorCode;

typedef double (*MathFunction)(double);

// Function prototypes
ErrorCode initStack(Stack* s);
void destroyStack(Stack* s);
Status pushStack(Stack* s, StackItem item);
StackItem popStack(Stack* s);
int isStackEmpty(Stack* s);
int precedence(char op);
int isRightAssociative(char op);
double applyOperator(double a, double b, char op);
double parseNumber(const char** expr);
double applyFunction(double arg, MathFunction func);
ErrorCode evaluate(const char* expr, double* result);
double neg(double x); // Prototype for neg function

typedef struct {
    const char* name;
    MathFunction func;
} Function;

Function functions[] = {
    {"sin", sin},
    {"cos", cos},
    {"tan", tan},
    {"asin", asin},
    {"acos", acos},
    {"atan", atan},
    {"sqrt", sqrt},
    {"log", log10},
    {"ln", log},
    {"arccot", atan},
    {"arccos", acos},
    {"neg", (MathFunction)neg}, // Use the neg function here
    {NULL, NULL}
};

// Function definitions
ErrorCode initStack(Stack* s) {
    s->items = (StackItem*)malloc(INITIAL_STACK_SIZE * sizeof(StackItem));
    if (s->items == NULL) {
        return ERR_MEMORY_ALLOCATION;
    }
    s->top = -1;
    s->size = INITIAL_STACK_SIZE;
    return ERR_NONE;
}

void destroyStack(Stack* s) {
    free(s->items);
    s->items = NULL;
    s->top = -1;
    s->size = 0;
}

Status pushStack(Stack* s, StackItem item) {
    if (s->top >= s->size - 1) {
        StackItem* newItems = (StackItem*)realloc(s->items, (s->size + INCREMENT_SIZE) * sizeof(StackItem));
        if (newItems == NULL) {
            return ERROR;
        }
        s->items = newItems;
        s->size += INCREMENT_SIZE;
    }
    s->items[++(s->top)] = item;
    return OK;
}

StackItem popStack(Stack* s) {
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
    } else if (op == 'U') { // Unary operator
        return 4;
    }
    return 0;
}

int isRightAssociative(char op) {
    if (op == '^') return 1;
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
    int hasDecimal = 0;
    int dotCount = 0;
    int negative = 0;

    if (**expr == '-') {
        negative = 1;
        (*expr)++;
    }

    while (isdigit(**expr) || **expr == '.') {
        if (**expr == '.') {
            if (hasDecimal) {
                return NAN;
            }
            hasDecimal = 1;
            dotCount++;
        } else {
            if (hasDecimal) {
                fraction += (**expr - '0') * pow(10, -++decimalPlaces);
            } else {
                result = result * 10 + (**expr - '0');
            }
        }
        (*expr)++;
    }
    if (dotCount > 1) {
        return NAN;
    }
    result += fraction;
    if (negative) {
        result = -result;
    }
    return result;
}

double applyFunction(double arg, MathFunction func) {
    return func(arg);
}

ErrorCode evaluate(const char* expr, double* result) {
    Stack valueStack, operatorStack;
    if (initStack(&valueStack) != ERR_NONE || initStack(&operatorStack) != ERR_NONE) {
        destroyStack(&valueStack);
        destroyStack(&operatorStack);
        return ERR_MEMORY_ALLOCATION;
    }

    const char* p = expr;
    ItemType prevTokenType = OPERATOR;

    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }
        if (isdigit(*p) || *p == '.') {
            double value = parseNumber(&p);
            if (isnan(value)) {
                destroyStack(&valueStack);
                destroyStack(&operatorStack);
                return ERR_MULTIPLE_DECIMAL_POINTS;
            }
            StackItem item = {.type = NUMBER, .data.num = value};
            if (pushStack(&valueStack, item) != OK) {
                destroyStack(&valueStack);
                destroyStack(&operatorStack);
                return ERR_STACK_OVERFLOW;
            }
            prevTokenType = NUMBER;
        } else if (*p == '(') {
            StackItem item = {.type = PAREN, .data.op = *p};
            if (pushStack(&operatorStack, item) != OK) {
                destroyStack(&valueStack);
                destroyStack(&operatorStack);
                return ERR_STACK_OVERFLOW;
            }
            p++;
            prevTokenType = PAREN;
        } else if (*p == ')') {
            while (!isStackEmpty(&operatorStack) && operatorStack.items[operatorStack.top].data.op != '(') {
                if (operatorStack.items[operatorStack.top].data.op == 'U') {
                    double a = popStack(&valueStack).data.num;
                    char op = popStack(&operatorStack).data.op;
                    double res = applyOperator(0, a, op);
                    StackItem item = {.type = NUMBER, .data.num = res};
                    if (pushStack(&valueStack, item) != OK) {
                        destroyStack(&valueStack);
                        destroyStack(&operatorStack);
                        return ERR_STACK_OVERFLOW;
                    }
                } else {
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
                    StackItem item = {.type = NUMBER, .data.num = res};
                    if (pushStack(&valueStack, item) != OK) {
                        destroyStack(&valueStack);
                        destroyStack(&operatorStack);
                        return ERR_STACK_OVERFLOW;
                    }
                }
            }
            if (!isStackEmpty(&operatorStack) && operatorStack.items[operatorStack.top].data.op == '(') {
                popStack(&operatorStack); // Pop '('
            }
            p++;
            prevTokenType = PAREN;
        } else if (strchr("+-*/^", *p)) {
            char op = *p;
            if (op == '-' && (prevTokenType == OPERATOR || prevTokenType == PAREN)) {
                op = 'U'; // Unary minus
            }
            while (!isStackEmpty(&operatorStack) && operatorStack.items[operatorStack.top].type == OPERATOR &&
                   (precedence(operatorStack.items[operatorStack.top].data.op) > precedence(op) ||
                    (precedence(operatorStack.items[operatorStack.top].data.op) == precedence(op) &&
                     isRightAssociative(op)))) {
                if (operatorStack.items[operatorStack.top].data.op == 'U') {
                    double a = popStack(&valueStack).data.num;
                    char popOp = popStack(&operatorStack).data.op;
                    double res = applyOperator(0, a, popOp);
                    StackItem item = {.type = NUMBER, .data.num = res};
                    if (pushStack(&valueStack, item) != OK) {
                        destroyStack(&valueStack);
                        destroyStack(&operatorStack);
                        return ERR_STACK_OVERFLOW;
                    }
                } else {
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
                    StackItem item = {.type = NUMBER, .data.num = res};
                    if (pushStack(&valueStack, item) != OK) {
                        destroyStack(&valueStack);
                        destroyStack(&operatorStack);
                        return ERR_STACK_OVERFLOW;
                    }
                }
            }
            StackItem item = {.type = OPERATOR, .data.op = op};
            if (pushStack(&operatorStack, item) != OK) {
                destroyStack(&valueStack);
                destroyStack(&operatorStack);
                return ERR_STACK_OVERFLOW;
            }
            p++;
            prevTokenType = OPERATOR;
        } else if (isalpha(*p)) {
            const char* funcStart = p;
            while (isalnum(*p)) p++;
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

                if ((strcmp(funcName, "asin") == 0 || strcmp(funcName, "acos") == 0) && (arg < -1 || arg > 1)) {
                    destroyStack(&valueStack);
                    destroyStack(&operatorStack);
                    return ERR_INVALID_ARGUMENT;
                }
                if (strcmp(funcName, "sqrt") == 0 && arg < 0) {
                    destroyStack(&valueStack);
                    destroyStack(&operatorStack);
                    return ERR_INVALID_ARGUMENT;
                }
                if ((strcmp(funcName, "log") == 0 || strcmp(funcName, "ln") == 0) && arg <= 0) {
                    destroyStack(&valueStack);
                    destroyStack(&operatorStack);
                    return ERR_INVALID_ARGUMENT;
                }
                if (strcmp(funcName, "arccot") == 0) {
                    if (arg == 0) {
                        destroyStack(&valueStack);
                        destroyStack(&operatorStack);
                        return ERR_INVALID_ARGUMENT;
                    }
                    arg = 1.0 / arg;
                }

                double funcRes = applyFunction(arg, functions[funcIndex].func);
                StackItem item = {.type = NUMBER, .data.num = funcRes};
                if (pushStack(&valueStack, item) != OK) {
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
        if (operatorStack.items[operatorStack.top].data.op == 'U') {
            if (isStackEmpty(&valueStack)) {
                destroyStack(&valueStack);
                destroyStack(&operatorStack);
                return ERR_STACK_UNDERFLOW;
            }
            double a = popStack(&valueStack).data.num;
            char op = popStack(&operatorStack).data.op;
            double res = applyOperator(0, a, op);
            StackItem item = {.type = NUMBER, .data.num = res};
            if (pushStack(&valueStack, item) != OK) {
                destroyStack(&valueStack);
                destroyStack(&operatorStack);
                return ERR_STACK_OVERFLOW;
            }
        } else {
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
            StackItem item = {.type = NUMBER, .data.num = res};
            if (pushStack(&valueStack, item) != OK) {
                destroyStack(&valueStack);
                destroyStack(&operatorStack);
                return ERR_STACK_OVERFLOW;
            }
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

double neg(double x) {
    return -x;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }
    char* inputFileName = argv[1];
    char* outputFileName = argv[2];
    FILE* inputFile = fopen(inputFileName, "r");
    FILE* outputFile = fopen(outputFileName, "w");

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

        errorCode = evaluate(expr, &result);
        if (errorCode != ERR_NONE) {
            switch (errorCode) {
                case ERR_MULTIPLE_DECIMAL_POINTS:
                    fprintf(outputFile, "Error: Multiple decimal points in number.\n");
                    break;
                case ERR_INVALID_CHARACTER:
                    fprintf(outputFile, "Error: Invalid character in expression.\n");
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
                    fprintf(outputFile, "Error: Invalid argument for function.\n");
                    break;
                default:
                    fprintf(outputFile, "Error evaluating expression: %d\n", errorCode);
                    break;
            }
        } else {
            fprintf(outputFile, "结果: %.2f\n", result);
        }
    }

    fclose(inputFile);
    fclose(outputFile);
    return 0;
}