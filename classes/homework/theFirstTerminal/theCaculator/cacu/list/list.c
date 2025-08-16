#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

#define INITIAL_SIZE 100
#define PI 3.141592653589793

typedef enum {ERROR = 0, OK = 1} Status;
typedef enum {FALSE = 0, TRUE = 1} Bool;

// Debugging switch
#define DEBUG 1

// Debug print function
void debugPrint(FILE *outputFile, const char* format, ...) {
    if (DEBUG) {
        va_list args;
        va_start(args, format);
        vfprintf(outputFile, format, args);
        va_end(args);
    }
}

// Definition of a node in the linked list
typedef struct Node {
    void* data;
    struct Node* next;
} Node;

// Function prototypes for linked list
Node* createNode(void* data);
void deleteNode(Node* node);

// Definition of the linked stack
typedef struct {
    Node* top;
} LinkedStack;

// Function prototypes for linked stack
void initLinkedStack(LinkedStack* stack);
Bool isLinkedStackEmpty(LinkedStack* stack);
Status pushLinkedStack(LinkedStack* stack, void* item);
void* popLinkedStack(LinkedStack* stack, Status* status);
void* peekLinkedStack(LinkedStack* stack);

// Function prototypes for calculator
void RemoveSpaces(char *string);
void Replace(char *infix);
void AddZero(char *infix);
Status InfixToPostfix(char *infix, char *postfix, FILE* outputFile);
Status applyOperations(LinkedStack *stack, FILE* outputFile);
double evaluatePostfix(char *postfix, Status* status, FILE* outputFile);
double applyFunction(char func, double value, Status* status);
void printError(Status status, FILE* outputFile);

// Create a new node
Node* createNode(void* data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) return NULL;
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Delete a node
void deleteNode(Node* node) {
    if (node) {
        free(node->data);
        free(node);
    }
}

// Initialize linked stack
void initLinkedStack(LinkedStack* stack) {
    stack->top = NULL;
}

Bool isLinkedStackEmpty(LinkedStack* stack) {
    return stack->top == NULL;
}

Status pushLinkedStack(LinkedStack* stack, void* item) {
    Node* newNode = createNode(item);
    if (!newNode) {
        debugPrint(stderr, "Memory allocation error during push operation\n");
        return ERROR;
    }

    newNode->next = stack->top;
    stack->top = newNode;

    debugPrint(stderr, "Pushed item to linked stack\n");
    return OK;
}

void* popLinkedStack(LinkedStack* stack, Status* status) {
    if (isLinkedStackEmpty(stack)) {
        *status = ERROR;
        debugPrint(stderr, "Stack underflow error during pop operation\n");
        return NULL;
    }

    Node* topNode = stack->top;
    void* item = topNode->data;
    stack->top = topNode->next;
    deleteNode(topNode);

    debugPrint(stderr, "Popped item from linked stack\n");
    *status = OK;
    return item;
}

void* peekLinkedStack(LinkedStack* stack) {
    if (!isLinkedStackEmpty(stack)) {
        return stack->top->data;
    } else {
        return NULL;
    }
}

// Operator priority
int GetOpPriority(char operator) {
    switch (operator) {
        case '#': return 0;
        case '(': return 1;
        case '+': case '-': return 2;
        case '*': case '/': return 3;
        case '^': return 4;
        case 'Q': case 'W': case 'E': case 'R':
        case 'T': case 'Y': case 'U': case 'S': return 5;
        default: return -1;
    }
}

// Remove spaces from a string
void RemoveSpaces(char *string) {
    int i = 0, j = 0;
    while (string[i] != '\0') {
        if (!isspace(string[i])) {
            string[j++] = string[i];
        }
        i++;
    }
    string[j] = '\0';
    debugPrint(stderr, "After removing spaces: %s\n", string);
}

// Replace functions with single characters and handle 'pi'
void Replace(char *infix) {
    int i = 0, j = 0;
    char infixTemp[INITIAL_SIZE * 2];
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
    debugPrint(stderr, "After replacing functions: %s\n", infix);
}

// Add zero before unary operators
void AddZero(char *infix) {
    int i = 0, j = 0;
    char temp[INITIAL_SIZE * 2];
    int len = strlen(infix);

    if (infix[0] == '+' || infix[0] == '-') {
        temp[j++] = '0';
    }

    while (infix[i] != '\0') {
        if ((infix[i] == '+' || infix[i] == '-') && 
            (i == 0 || strchr("+-*/^(", infix[i - 1]))) {
            temp[j++] = '0';
            temp[j++] = infix[i++];
        } else {
            temp[j++] = infix[i++];
        }
    }
    temp[j] = '\0';
    strcpy(infix, temp);
    debugPrint(stderr, "After adding zeros: %s\n", infix);
}

// Convert infix to postfix
Status InfixToPostfix(char *infix, char *postfix, FILE* outputFile) {
    LinkedStack stack;
    initLinkedStack(&stack);
    int i = 0, j = 0;
    Status status = OK;

    RemoveSpaces(infix);
    Replace(infix);
    AddZero(infix);

    debugPrint(stderr, "Infix expression: %s\n", infix);

    while (infix[i] != '\0') {
        if (isdigit(infix[i]) || (infix[i] == '.' && isdigit(infix[i + 1]))) {
            while (isdigit(infix[i]) || infix[i] == '.') {
                postfix[j++] = infix[i++];
            }
            postfix[j++] = ' ';
        } else if (infix[i] == '(') {
            char* op = malloc(sizeof(char));
            if (!op) return ERROR;
            *op = infix[i++];
            status = pushLinkedStack(&stack, op);
            if (status != OK) return status;
        } else if (infix[i] == ')') {
            while (!isLinkedStackEmpty(&stack) && (*(char*)peekLinkedStack(&stack)) != '(') {
                char* op = (char*)popLinkedStack(&stack, &status);
                postfix[j++] = *op;
                free(op);
            }
            if (!isLinkedStackEmpty(&stack)) {
                char* paren = (char*)popLinkedStack(&stack, &status);
                free(paren);
            } else {
                return ERROR;
            }
            i++;
        } else if (strchr("+-*/^QWERTYUIS", infix[i]) != NULL) {
            while (!isLinkedStackEmpty(&stack) && 
                   GetOpPriority(infix[i]) <= GetOpPriority(*(char*)peekLinkedStack(&stack))) {
                char* op = (char*)popLinkedStack(&stack, &status);
                postfix[j++] = *op;
                free(op);
            }
            char* op = malloc(sizeof(char));
            if (!op) return ERROR;
            *op = infix[i++];
            status = pushLinkedStack(&stack, op);
            if (status != OK) return status;
        } else {
            return ERROR;
        }
    }
    while (!isLinkedStackEmpty(&stack)) {
        char* op = (char*)popLinkedStack(&stack, &status);
        if (*op == '(') {
            free(op);
            return ERROR;
        }
        postfix[j++] = *op;
        free(op);
    }
    postfix[j] = '\0';

    debugPrint(stderr, "Postfix expression: %s\n", postfix);
    return OK;
}

// Apply operations
Status applyOperations(LinkedStack *stack, FILE* outputFile) {
    Status status = OK;
    char *opPtr = (char*)popLinkedStack(stack, &status);
    if (status != OK) return status;
    char op = *opPtr;
    free(opPtr);

    if (strchr("QWERTYUIS", op) != NULL) {
        double *numPtr = (double*)popLinkedStack(stack, &status);
        if (status != OK) return status;
        double num = *numPtr;
        free(numPtr);

        double result = applyFunction(op, num, &status);
        if (status != OK) return status;

        double *resPtr = malloc(sizeof(double));
        if (!resPtr) return ERROR;
        *resPtr = result;
        status = pushLinkedStack(stack, resPtr);
        if (status != OK) return status;

        debugPrint(stderr, "Applied function: %c, result: %lf\n", op, result);
    } else {
        double *val2Ptr = (double*)popLinkedStack(stack, &status);
        if (status != OK) return status;
        double val2 = *val2Ptr;
        free(val2Ptr);

        double *val1Ptr = (double*)popLinkedStack(stack, &status);
        if (status != OK) return status;
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
                if (val2 == 0) {
                    return ERROR;
                }
                result = val1 / val2;
                break;
            case '^':
                result = pow(val1, val2);
                break;
            default:
                return ERROR;
        }
        double *resPtr = malloc(sizeof(double));
        if (!resPtr) return ERROR;
        *resPtr = result;
        status = pushLinkedStack(stack, resPtr);
        if (status != OK) return status;

        debugPrint(stderr, "Applied operator: %c, result: %lf\n", op, result);
    }
    return OK;
}

// Evaluate postfix expression
double evaluatePostfix(char *postfix, Status* status, FILE* outputFile) {
    LinkedStack stack;
    initLinkedStack(&stack);
    int i = 0, len = strlen(postfix);
    *status = OK;

    while (i < len) {
        if (isspace(postfix[i])) {
            i++;
            continue;
        }
        if (isdigit(postfix[i]) || (postfix[i] == '.' && isdigit(postfix[i + 1]))) {
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
                *status = ERROR;
                return NAN;
            }
            *numPtr = sign * num;
            *status = pushLinkedStack(&stack, numPtr);
            if (*status != OK) return NAN;
            debugPrint(stderr, "Pushed number: %lf\n", *numPtr);
        } else {
            char *op = malloc(sizeof(char));
            if (!op) {
                *status = ERROR;
                return NAN;
            }
            *op = postfix[i++];
            *status = pushLinkedStack(&stack, op);
            if (*status != OK) return NAN;
            debugPrint(stderr, "Pushed operator: %c\n", *op);

            *status = applyOperations(&stack, outputFile);
            if (*status != OK) return NAN;
        }
    }
    double *resultPtr = (double*)popLinkedStack(&stack, status);
    if (*status != OK) return NAN;
    double result = *resultPtr;
    free(resultPtr);
    return result;
}

// Apply function
double applyFunction(char func, double value, Status* status) {
    *status = OK;
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
            *status = ERROR;
            return NAN;
    }
}

// Print error messages based on Status
void printError(Status status, FILE* outputFile) {
    if (status == ERROR) {
        fprintf(outputFile, "An error occurred.\n");
    } else {
        fprintf(outputFile, "No error.\n");
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

    char infix[INITIAL_SIZE * 2];
    char postfix[INITIAL_SIZE * 2];

    while (fgets(infix, sizeof(infix), inputFile)) {
        infix[strcspn(infix, "\n")] = '\0'; // Remove newline character

        debugPrint(stderr, "Processing expression: %s\n", infix);

        Status status = InfixToPostfix(infix, postfix, outputFile);
        if (status == OK) {
            double result = evaluatePostfix(postfix, &status, outputFile);
            if (status == OK) {
                fprintf(outputFile, "Expression: %s\nResult: %lf\n", infix, result);
            } else {
                fprintf(outputFile, "Expression: %s\n", infix);
                fprintf(outputFile, "Error: ");
                printError(status, outputFile);
            }
        } else {
            fprintf(outputFile, "Expression: %s\n", infix);
            fprintf(outputFile, "Error: ");
            printError(status, outputFile);
        }
    }

    fflush(outputFile); // Ensure all data is flushed to disk
    fclose(inputFile);
    fclose(outputFile);
    return 0;
}