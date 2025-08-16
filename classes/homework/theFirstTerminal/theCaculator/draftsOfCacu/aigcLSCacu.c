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

#define DEBUG 1

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

typedef struct Node {
    void* data;
    struct Node* next;
} Node;

typedef struct {
    Node* top;
    int elementSize;
} Stack;

// Function prototypes
void initStack(Stack* s, int elementSize);
int isStackEmpty(Stack* s);
ErrorCode pushStack(Stack* s, void* item);
void* popStack(Stack* s, ErrorCode* err);
void* peekStack(Stack* s);
void freeStack(Stack* s);

void RemoveSpaces(char* string);
void Replace(char* infix);
void AddZero(char* infix);
ErrorCode InfixToPostfix(char* infix, char* postfix);
ErrorCode applyOperations(Stack* stack);
double evaluatePostfix(char* postfix, ErrorCode* err);
double applyFunction(char func, double value, ErrorCode* err);
void printError(ErrorCode err);

void initStack(Stack* s, int elementSize) {
    s->top = NULL;
    s->elementSize = elementSize;
}

int isStackEmpty(Stack* s) {
    return s->top == NULL;
}

void freeStack(Stack* s) {
    while (!isStackEmpty(s)) {
        ErrorCode err;
        void* item = popStack(s, &err);
        if (item) {
            free(item);
        }
    }
    s->top = NULL;
    debugPrint("Stack memory freed successfully.\n");
}

ErrorCode pushStack(Stack* s, void* item) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        return ERR_MEMORY_ALLOCATION;
    }
    newNode->data = malloc(s->elementSize);
    if (!newNode->data) {
        free(newNode);
        return ERR_MEMORY_ALLOCATION;
    }
    memcpy(newNode->data, item, s->elementSize);
    newNode->next = s->top;
    s->top = newNode;
    debugPrint("Pushed item to stack\n");
    return ERR_NONE;
}

void* popStack(Stack* s, ErrorCode* err) {
    if (isStackEmpty(s)) {
        *err = ERR_STACK_UNDERFLOW;
        return NULL;
    }
    Node* temp = s->top;
    void* item = temp->data;
    s->top = temp->next;
    free(temp);
    debugPrint("Popped item from stack\n");
    *err = ERR_NONE;
    return item;
}

void* peekStack(Stack* s) {
    if (isStackEmpty(s)) {
        return NULL;
    }
    return s->top->data;
}

int GetOpPriority(char operator) {
    switch (operator) {
        case '#': return 0;
        case '(': return 1;
        case '+': case '-': return 2;
        case '*': case '/': return 3;
        case '^': return 4;
        default:
            if (strchr("QWERTYUIS", operator)) return 5;
            return -1;
    }
}

void RemoveSpaces(char* string) {
    int i = 0, j = 0;
    while (string[i] != '\0') {
        if (string[i] != ' ') {
            string[j++] = string[i];
        }
        i++;
    }
    string[j] = '\0';
    debugPrint("After removing spaces: %s\n", string);
}

void Replace(char* infix) {
    int i = 0, j = 0;
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

void AddZero(char* infix) {
    int i = 0, j = 0;
    char temp[INITIAL_SIZE * 2];
    int len = strlen(infix);

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

ErrorCode InfixToPostfix(char* infix, char* postfix) {
    Stack stack;
    initStack(&stack, sizeof(char));
    int i = 0, j = 0;
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
            err = pushStack(&stack, &infix[i++]);
            if (err != ERR_NONE) return err;
        } else if (infix[i] == ')') {
            char* elem;
            while (!isStackEmpty(&stack) && (*(elem = (char*)peekStack(&stack)) != '(')) {
                char* op = (char*)popStack(&stack, &err);
                postfix[j++] = *op;
                postfix[j++] = ' ';
                free(op);
            }
            if (!isStackEmpty(&stack)) {
                char* paren = (char*)popStack(&stack, &err);
                free(paren);
            } else {
                return ERR_INVALID_EXPRESSION;
            }
            i++;
        } else if (strchr("+-*/^QWERTYUIS", infix[i]) != NULL) {
            while (!isStackEmpty(&stack) && GetOpPriority(infix[i]) <= GetOpPriority(*(char*)peekStack(&stack))) {
                char* op = (char*)popStack(&stack, &err);
                if (err != ERR_NONE) return err;
                postfix[j++] = *op;
                postfix[j++] = ' ';
                free(op);
            }
            err = pushStack(&stack, &infix[i++]);
            if (err != ERR_NONE) return err;
        } else {
            return ERR_INVALID_CHARACTER;
        }
    }

    while (!isStackEmpty(&stack)) {
        char* op = (char*)popStack(&stack, &err);
        if (err != ERR_NONE) return err;
        if (*op == '(' || *op == ')') {
            free(op);
            return ERR_INVALID_EXPRESSION;
        }
        postfix[j++] = *op;
        postfix[j++] = ' ';
        free(op);
    }
    postfix[j] = '\0';
    freeStack(&stack);

    debugPrint("Postfix expression: %s\n", postfix);
    return ERR_NONE;
}

double evaluatePostfix(char* postfix, ErrorCode* err) {
    Stack stack;
    initStack(&stack, sizeof(double));
    int i = 0;
    double result = 0.0;

    debugPrint("Evaluating postfix expression: %s\n", postfix);

    while (postfix[i] != '\0') {
        if (isdigit(postfix[i]) || (postfix[i] == '-' && isdigit(postfix[i + 1]))) {
            char number[INITIAL_SIZE];
            int j = 0;
            while (isdigit(postfix[i]) || postfix[i] == '.' || postfix[i] == '-') {
                number[j++] = postfix[i++];
            }
            number[j] = '\0';
            double value = atof(number);
            *err = pushStack(&stack, &value);
            if (*err != ERR_NONE) return 0.0;
        } else if (strchr("+-*/^", postfix[i]) != NULL) {
            double b = *(double*)popStack(&stack, err);
            if (*err != ERR_NONE) return 0.0;
            double a = *(double*)popStack(&stack, err);
            if (*err != ERR_NONE) return 0.0;

            switch (postfix[i]) {
                case '+': result = a + b; break;
                case '-': result = a - b; break;
                case '*': result = a * b; break;
                case '/':
                    if (b == 0.0) {
                        *err = ERR_DIVISION_BY_ZERO;
                        return 0.0;
                    }
                    result = a / b;
                    break;
                case '^': result = pow(a, b); break;
                default: 
                    *err = ERR_INVALID_OPERATOR;
                    return 0.0;
            }
            *err = pushStack(&stack, &result);
            if (*err != ERR_NONE) return 0.0;
            i++;
        } else if (strchr("QWERTYUIS", postfix[i]) != NULL) {
            double value = *(double*)popStack(&stack, err);
            if (*err != ERR_NONE) return 0.0;
            result = applyFunction(postfix[i], value, err);
            if (*err != ERR_NONE) return 0.0;
            *err = pushStack(&stack, &result);
            if (*err != ERR_NONE) return 0.0;
            i++;
        } else {
            i++;
        }
    }

    if (!isStackEmpty(&stack)) {
        result = *(double*)popStack(&stack, err);
    }

    freeStack(&stack);
    return result;
}

double applyFunction(char func, double value, ErrorCode* err) {
    switch (func) {
        case 'Q': return sin(value);
        case 'W': return cos(value);
        case 'E': return tan(value);
        case 'R': return 1.0 / tan(value);
        case 'T': return asin(value);
        case 'Y': return acos(value);
        case 'U': return atan(value);
        case 'S': return sqrt(value);
        default:
            *err = ERR_INVALID_FUNCTION;
            return 0.0;
    }
}

void printError(ErrorCode err) {
    switch (err) {
        case ERR_NONE:
            printf("No error occurred.\n");
            break;
        case ERR_STACK_OVERFLOW:
            printf("Error: Stack overflow occurred.\n");
            break;
        case ERR_STACK_UNDERFLOW:
            printf("Error: Stack underflow occurred.\n");
            break;
        case ERR_INVALID_CHARACTER:
            printf("Error: Invalid character encountered in expression.\n");
            break;
        case ERR_INVALID_OPERATOR:
            printf("Error: Invalid operator in expression.\n");
            break;
        case ERR_DIVISION_BY_ZERO:
            printf("Error: Division by zero is undefined.\n");
            break;
        case ERR_MEMORY_ALLOCATION:
            printf("Error: Memory allocation failed.\n");
            break;
        case ERR_INVALID_EXPRESSION:
            printf("Error: The expression is invalid.\n");
            break;
        case ERR_INVALID_FUNCTION:
            printf("Error: Invalid mathematical function encountered.\n");
            break;
        default:
            printf("Error: An unknown error occurred.\n");
            break;
    }
}

int main() {
    char inputFilename[256];
    char outputFilename[256];

    // 输入文件名
    printf("Enter the name of the file containing expressions: ");
    if (!fgets(inputFilename, sizeof(inputFilename), stdin)) {
        printf("Error: Failed to read input.\n");
        return 1;
    }
    size_t len = strlen(inputFilename);
    if (len > 0 && inputFilename[len - 1] == '\n') {
        inputFilename[len - 1] = '\0';
    }

    // 输出文件名
    printf("Enter the name of the output file: ");
    if (!fgets(outputFilename, sizeof(outputFilename), stdin)) {
        printf("Error: Failed to read output filename.\n");
        return 1;
    }
    len = strlen(outputFilename);
    if (len > 0 && outputFilename[len - 1] == '\n') {
        outputFilename[len - 1] = '\0';
    }

    FILE *inputFile = fopen(inputFilename, "r");
    if (!inputFile) {
        printf("Error: Could not open file %s\n", inputFilename);
        return 1;
    }

    FILE *outputFile = fopen(outputFilename, "w");
    if (!outputFile) {
        printf("Error: Could not open output file %s\n", outputFilename);
        fclose(inputFile);
        return 1;
    }

    char infix[INITIAL_SIZE];
    char postfix[INITIAL_SIZE];
    while (fgets(infix, sizeof(infix), inputFile)) {
        len = strlen(infix);
        if (len > 0 && infix[len - 1] == '\n') {
            infix[len - 1] = '\0';
        }

        printf("\nInfix expression: %s\n", infix);
        fprintf(outputFile, "Infix expression: %s\n", infix);

        ErrorCode err = InfixToPostfix(infix, postfix);
        if (err != ERR_NONE) {
            printError(err);
            fprintf(outputFile, "Error: ");
            printError(err);
            continue;
        }

        double result = evaluatePostfix(postfix, &err);
        if (err != ERR_NONE) {
            printError(err);
            fprintf(outputFile, "Error: ");
            printError(err);
            continue;
        }

        printf("Result: %.15lf\n", result);
        fprintf(outputFile, "Result: %.15lf\n", result);
    }

    fclose(inputFile);
    fclose(outputFile);
    return 0;
}