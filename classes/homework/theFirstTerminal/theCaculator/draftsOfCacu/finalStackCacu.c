#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

#define INITIAL_SIZE 100
#define INCREMENT 10
#define PI 3.141592653589793

// Debugging switch
#define DEBUG 1

// Debug print function
void debugPrint(const char* format, ...) {
    if (DEBUG) {
        va_list args;
        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
    }
}

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
    void** data;
    int top;
    int elementSize;
    int stackSize;
} Stack;

// Function prototypes
void initStack(Stack *s, int elementSize);
int isStackEmpty(Stack *s);
ErrorCode pushStack(Stack *s, void* item);
void* popStack(Stack *s, ErrorCode *err);
void* peekStack(Stack *s);

void RemoveSpaces(char *string);
void Replace(char *infix);
void AddZero(char *infix);
ErrorCode InfixToPostfix(char *infix, char *postfix);
ErrorCode applyOperations(Stack *stack);
double evaluatePostfix(char *postfix, ErrorCode *err);
double applyFunction(char func, double value, ErrorCode *err);
void printError(ErrorCode err);

// Initialize stack
void initStack(Stack *s, int elementSize) {
    s->data = (void**)malloc(INITIAL_SIZE * elementSize);
    if (!s->data) {
        fprintf(stderr, "Memory allocation error during stack initialization\n");
        exit(EXIT_FAILURE);
    }
    s->top = -1;
    s->elementSize = elementSize;
    s->stackSize = INITIAL_SIZE;
}

int isStackEmpty(Stack *s) {
    return s->top == -1;
}

ErrorCode pushStack(Stack *s, void* item) {
    if (s->top >= s->stackSize - 1) {
        // Expand stack capacity
        s->stackSize += INCREMENT;
        void** newData = (void**)realloc(s->data, s->stackSize * s->elementSize);
        if (!newData) {
            return ERR_MEMORY_ALLOCATION;
        }
        s->data = newData;
        debugPrint("Expanded stack to new size: %d\n", s->stackSize);
    }
    s->data[++(s->top)] = item;
    debugPrint("Pushed item to stack\n");
    return ERR_NONE;
}

void* popStack(Stack *s, ErrorCode *err) {
    if (!isStackEmpty(s)) {
        void* item = s->data[(s->top)--];
        debugPrint("Popped item from stack\n");
        *err = ERR_NONE;
        return item;
    } else {
        *err = ERR_STACK_UNDERFLOW;
        return NULL;
    }
}

void* peekStack(Stack *s) {
    if (!isStackEmpty(s)) {
        return s->data[s->top];
    } else {
        return NULL;
    }
}

// Operator priority
int GetOpPriority(char operator) {
    if (operator == '#') {
        return 0;
    } else if (operator == '(') {
        return 1;
    } else if (operator == '+' || operator == '-') {
        return 2;
    } else if (operator == '*' || operator == '/') {
        return 3;
    } else if (operator == '^') {
        return 4;
    } else if (strchr("QWERTYUIS", operator)) {
        return 5;
    } else {
        return -1;
    }
}

// Remove spaces from a string
void RemoveSpaces(char *string) {
    int i = 0;
    int j = 0;
    while (string[i] != '\0') {
        if (string[i] != ' ') {
            string[j++] = string[i];
        }
        i++;
    }
    string[j] = '\0';
    debugPrint("After removing spaces: %s\n", string);
}

// Replace functions with single characters and handle 'pi'
void Replace(char *infix) {
    int i = 0;
    int j = 0;
    char infixTemp[INITIAL_SIZE];
    while (infix[i] != '\0') {
        if (strncmp(&infix[i], "sin", 3) == 0) {
            infixTemp[j++] = 'Q';
            i += 3;
        } else if (strncmp(&infix[i], "cos", 3) == 0) {
            infixTemp[j++] = 'W';
            i += 3;
        } else if (strncmp(&infix[i], "tan", 3) == 0) {
            infixTemp[j++] = 'E';
            i += 3;
        } else if (strncmp(&infix[i], "cot", 3) == 0) {
            infixTemp[j++] = 'R';
            i += 3;
        } else if (strncmp(&infix[i], "asin", 4) == 0) {
            infixTemp[j++] = 'T';
            i += 4;
        } else if (strncmp(&infix[i], "acos", 4) == 0) {
            infixTemp[j++] = 'Y';
            i += 4;
        } else if (strncmp(&infix[i], "atan", 4) == 0) {
            infixTemp[j++] = 'U';
            i += 4;
        } else if (strncmp(&infix[i], "sqrt", 4) == 0) {
            infixTemp[j++] = 'S';
            i += 4;
        } else if (strncmp(&infix[i], "pi", 2) == 0) {
            sprintf(&infixTemp[j], "%.15lf", PI);
            j += strlen(&infixTemp[j]);
            i += 2;
        } else {
            infixTemp[j++] = infix[i++];
        }
    }
    infixTemp[j] = '\0';
    strcpy(infix, infixTemp);
    debugPrint("After replacing functions: %s\n", infix);
}

// Add zero before unary operators
void AddZero(char *infix) {
    int i = 0;
    int j = 0;
    char temp[INITIAL_SIZE * 2];
    int len = strlen(infix);

    // Check if the first character is '+' or '-'
    if (infix[0] == '+' || infix[0] == '-') {
        temp[j++] = '0';
        temp[j++] = infix[i++];
    } else {
        temp[j++] = infix[i++];
    }

    while (i < len) {
        if ((infix[i] == '+' || infix[i] == '-') && 
            (i == 0 || strchr("+-*/^(", infix[i - 1]))) {
            temp[j++] = '0';
            temp[j++] = infix[i];
            i++;
        } else {
            temp[j++] = infix[i++];
        }
    }
    temp[j] = '\0';
    strcpy(infix, temp);
    debugPrint("After adding zeros: %s\n", infix);
}

// Convert infix to postfix
ErrorCode InfixToPostfix(char *infix, char *postfix) {
    Stack stack;
    initStack(&stack, sizeof(char*));
    int i = 0;
    int j = 0;
    ErrorCode err = ERR_NONE;

    RemoveSpaces(infix);
    Replace(infix);
    AddZero(infix);

    debugPrint("Infix expression: %s\n", infix);

    while (infix[i] != '\0') {
        if (isdigit(infix[i]) || (infix[i] == '-' && (i == 0 || strchr("+-*/^(", infix[i - 1])))) {
            while (isdigit(infix[i]) || infix[i] == '.') {
                postfix[j++] = infix[i++];
            }
            postfix[j++] = ' ';
        } else if (infix[i] == '(') {
            char *op = malloc(sizeof(char));
            if (!op) return ERR_MEMORY_ALLOCATION;
            *op = infix[i++];
            err = pushStack(&stack, op);
            if (err != ERR_NONE) return err;
        } else if (infix[i] == ')') {
            char *elem;
            while (!isStackEmpty(&stack) && (*(elem = (char*)peekStack(&stack)) != '(')) {
                char *op = (char*)popStack(&stack, &err);
                postfix[j++] = *op;
                free(op);
            }
            if (!isStackEmpty(&stack)) {
                char *paren = (char*)popStack(&stack, &err);
                free(paren);
            } else {
                return ERR_INVALID_EXPRESSION;
            }
            i++;
        } else if (strchr("+-*/^QWERTYUIS", infix[i]) != NULL) {
            while (!isStackEmpty(&stack) && GetOpPriority(infix[i]) <= GetOpPriority(*(char*)peekStack(&stack))) {
                char *op = (char*)popStack(&stack, &err);
                postfix[j++] = *op;
                free(op);
            }
            char *op = malloc(sizeof(char));
            if (!op) return ERR_MEMORY_ALLOCATION;
            *op = infix[i++];
            err = pushStack(&stack, op);
            if (err != ERR_NONE) return err;
        } else {
            return ERR_INVALID_CHARACTER;
        }
    }
    while (!isStackEmpty(&stack)) {
        char *op = (char*)popStack(&stack, &err);
        if (*op == '(') {
            free(op);
            return ERR_INVALID_EXPRESSION;
        }
        postfix[j++] = *op;
        free(op);
    }
    postfix[j] = '\0';

    debugPrint("Postfix expression: %s\n", postfix);
    return ERR_NONE;
}

// Apply operations
ErrorCode applyOperations(Stack *stack) {
    ErrorCode err = ERR_NONE;
    char *opPtr = (char*)popStack(stack, &err);
    if (err != ERR_NONE) return err;
    char op = *opPtr;
    free(opPtr);

    if (op == 'Q' || op == 'W' || op == 'E' || op == 'R' ||
        op == 'T' || op == 'Y' || op == 'U' || op == 'I' || op == 'S') {
        double *numPtr = (double*)popStack(stack, &err);
        if (err != ERR_NONE) return err;
        double num = *numPtr;
        free(numPtr);

        double result = applyFunction(op, num, &err);
        if (err != ERR_NONE) return err;

        double *resPtr = malloc(sizeof(double));
        if (!resPtr) return ERR_MEMORY_ALLOCATION;
        *resPtr = result;
        err = pushStack(stack, resPtr);
        if (err != ERR_NONE) return err;

        debugPrint("Applied function: %c, result: %lf\n", op, result);
    } else {
        double *val2Ptr = (double*)popStack(stack, &err);
        if (err != ERR_NONE) return err;
        double val2 = *val2Ptr;
        free(val2Ptr);

        double *val1Ptr = (double*)popStack(stack, &err);
        if (err != ERR_NONE) return err;
        double val1 = *val1Ptr;
        free(val1Ptr);

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
                if (val2 != 0) {
                    result = val1 / val2;
                } else {
                    return ERR_DIVISION_BY_ZERO;
                }
                break;
            case '^':
                result = pow(val1, val2);
                break;
            default:
                return ERR_INVALID_OPERATOR;
        }
        double *resPtr = malloc(sizeof(double));
        if (!resPtr) return ERR_MEMORY_ALLOCATION;
        *resPtr = result;
        err = pushStack(stack, resPtr);
        if (err != ERR_NONE) return err;

        debugPrint("Applied operator: %c, result: %lf\n", op, result);
    }
    return ERR_NONE;
}

// Evaluate postfix expression
double evaluatePostfix(char *postfix, ErrorCode *err) {
    Stack stack;
    initStack(&stack, sizeof(double*));
    int i = 0;
    int len = strlen(postfix);
    *err = ERR_NONE;

    while (i < len) {
        if (isspace(postfix[i])) {
            i++;
            continue;
        }
        if (isdigit(postfix[i]) || (postfix[i] == '-' && isdigit(postfix[i + 1]))) {
            double num = 0;
            int sign = 1;
            if (postfix[i] == '-') {
                sign = -1;
                i++;
            }
            while (isdigit(postfix[i])) {
                num = num * 10 + (postfix[i] - '0');
                i++;
            }
            if (postfix[i] == '.') {
                i++;
                double decimalPlace = 0.1;
                while (isdigit(postfix[i])) {
                    num += (postfix[i] - '0') * decimalPlace;
                    decimalPlace *= 0.1;
                    i++;
                }
            }
            double *numPtr = malloc(sizeof(double));
            if (!numPtr) {
                *err = ERR_MEMORY_ALLOCATION;
                return NAN;
            }
            *numPtr = sign * num;
            *err = pushStack(&stack, numPtr);
            if (*err != ERR_NONE) return NAN;
            debugPrint("Pushed number: %lf\n", *numPtr);
        } else {
            char *op = malloc(sizeof(char));
            if (!op) {
                *err = ERR_MEMORY_ALLOCATION;
                return NAN;
            }
            *op = postfix[i++];
            *err = pushStack(&stack, op);
            if (*err != ERR_NONE) return NAN;
            debugPrint("Pushed operator: %c\n", *op);

            *err = applyOperations(&stack);
            if (*err != ERR_NONE) return NAN;
        }
    }
    double *resultPtr = (double*)popStack(&stack, err);
    if (*err != ERR_NONE) return NAN;
    double result = *resultPtr;
    free(resultPtr);
    return result;
}

// Apply function
double applyFunction(char func, double value, ErrorCode *err) {
    *err = ERR_NONE;
    switch (func) {
        case 'Q': return sin(value);
        case 'W': return cos(value);
        case 'E': return tan(value);
        case 'R': return 1.0 / tan(value);
        case 'T': return asin(value);
        case 'Y': return acos(value);
        case 'U': return atan(value);
        case 'I': return atan(1.0 / value);
        case 'S': return sqrt(value);
        default:
            *err = ERR_INVALID_FUNCTION;
            return NAN;
    }
}

// Print error messages based on ErrorCode
void printError(ErrorCode err) {
    switch (err) {
        case ERR_NONE:
            printf("No error.\n");
            break;
        case ERR_STACK_OVERFLOW:
            printf("Stack overflow error.\n");
            break;
        case ERR_STACK_UNDERFLOW:
            printf("Stack underflow error.\n");
            break;
        case ERR_INVALID_CHARACTER:
            printf("Invalid character error.\n");
            break;
        case ERR_INVALID_OPERATOR:
            printf("Invalid operator error.\n");
            break;
        case ERR_DIVISION_BY_ZERO:
            printf("Division by zero error.\n");
            break;
        case ERR_MEMORY_ALLOCATION:
            printf("Memory allocation error.\n");
            break;
        case ERR_INVALID_EXPRESSION:
            printf("Invalid expression error.\n");
            break;
        case ERR_INVALID_FUNCTION:
            printf("Invalid function error.\n");
            break;
        default:
            printf("Unknown error.\n");
            break;
    }
}

// Main function
int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        fprintf(stderr, "Could not open input file: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    FILE *outputFile = fopen(argv[2], "w");
    if (!outputFile) {
        fprintf(stderr, "Could not open output file: %s\n", argv[2]);
        fclose(inputFile);
        return EXIT_FAILURE;
    }

    char infix[INITIAL_SIZE];
    char postfix[INITIAL_SIZE];

    while (fgets(infix, sizeof(infix), inputFile)) {
        infix[strcspn(infix, "\n")] = '\0'; // Remove newline character

        ErrorCode err = InfixToPostfix(infix, postfix);
        if (err == ERR_NONE) {
            double result = evaluatePostfix(postfix, &err);
            if (err == ERR_NONE) {
                fprintf(outputFile, "Expression: %s\nResult: %lf\n", infix, result);
            } else {
                fprintf(outputFile, "Expression: %s\n", infix);
                fprintf(outputFile, "Error: ");
                printError(err);
            }
        } else {
            fprintf(outputFile, "Expression: %s\n", infix);
            fprintf(outputFile, "Error: ");
            printError(err);
        }
    }

    fclose(inputFile);
    fclose(outputFile);
    return 0;
}
