#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "common.h"
#include "LinkedStack.h"

// 函数声明
void debugPrint(const char* format, ...);
/*debug的打印函数*/

int GetOpPriority(char operator);
/*操作符优先级 返回operator代表的操作符的（整型）优先级*/

void RemoveSpaces(char *string);
/*移除string指向的字符串中的空格*/

void Replace(char *infix);
/*替换infix指向的特殊函数为字符*/

void AddZero(char *infix);
/* 为infix指向的表达式中的一元操作符（如 -）添加显式的零*/

Status InfixToPostfix(char *infix, char *postfix);
/*将infix指向的中缀表达式转换为postfix指向的后缀表达式，返回错误代码*/

double evaluatePostfix(char *postfix, Status *status);
/*计算postfix指向的后缀表达式的结果，返回结果值并通过 status 参数设置错误代码*/

double applyFunction(char func, double value, Status *status);
/*应用func指向的单目函数（如 sin、sqrt 等），返回计算value指向的值的结果并通过 status 参数设置错误代码*/

void printError(Status status);
/*打印status指向的错误信息*/

// Debug print function
void debugPrint(const char* format, ...) {
    if (DEBUG) {
        va_list args;
        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
    }
}

// 操作符优先级
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

// 去除字符串中的空格
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

// 用特殊字符替换函数
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

// 负数前加显示0
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

// 中缀转后缀
Status InfixToPostfix(char* infix, char* postfix) {
    Stack stack;
    initStack(&stack, sizeof(char));
    int i = 0, j = 0;
    Status status = OK;

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
            status = pushStack(&stack, &infix[i++]);
            if (status != OK) return status;
        } else if (infix[i] == ')') {
            char* elem;
            while (!isStackEmpty(&stack) && (*(elem = (char*)peekStack(&stack)) != '(')) {
                char* op = (char*)popStack(&stack, &status);
                postfix[j++] = *op;
                postfix[j++] = ' ';
                free(op);
            }
            if (!isStackEmpty(&stack)) {
                char* paren = (char*)popStack(&stack, &status);
                free(paren);
            } else {
                return ERROR;
            }
            i++;
        } else if (strchr("+-*/^QWERTYUIS", infix[i]) != NULL) {
            while (!isStackEmpty(&stack) && GetOpPriority(infix[i]) <= GetOpPriority(*(char*)peekStack(&stack))) {
                char* op = (char*)popStack(&stack, &status);
                if (status != OK) return status;
                postfix[j++] = *op;
                postfix[j++] = ' ';
                free(op);
            }
            status = pushStack(&stack, &infix[i++]);
            if (status != OK) return status;
        } else {
            return ERROR;
        }
    }

    while (!isStackEmpty(&stack)) {
        char* op = (char*)popStack(&stack, &status);
        if (status != OK) return status;
        if (*op == '(' || *op == ')') {
            free(op);
            return ERROR;
        }
        postfix[j++] = *op;
        postfix[j++] = ' ';
        free(op);
    }
    postfix[j] = '\0';
    freeStack(&stack);

    debugPrint("Postfix expression: %s\n", postfix);
    return OK;
}

// 处理后缀表达式
double evaluatePostfix(char* postfix, Status* status) {
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
            *status = pushStack(&stack, &value);
            if (*status != OK) return 0.0;
        } else if (strchr("+-*/^", postfix[i]) != NULL) {
            double b = *(double*)popStack(&stack, status);
            if (*status != OK) return 0.0;
            double a = *(double*)popStack(&stack, status);
            if (*status != OK) return 0.0;

            switch (postfix[i]) {
                case '+': result = a + b; break;
                case '-': result = a - b; break;
                case '*': result = a * b; break;
                case '/':
                    if (b == 0.0) {
                        *status = ERROR;
                        return 0.0;
                    }
                    result = a / b;
                    break;
                case '^': result = pow(a, b); break;
                default: 
                    *status = ERROR;
                    return 0.0;
            }
            *status = pushStack(&stack, &result);
            if (*status != OK) return 0.0;
            i++;
        } else if (strchr("QWERTYUIS", postfix[i]) != NULL) {
            double value = *(double*)popStack(&stack, status);
            if (*status != OK) return 0.0;
            result = applyFunction(postfix[i], value, status);
            if (*status != OK) return 0.0;
            *status = pushStack(&stack, &result);
            if (*status != OK) return 0.0;
            i++;
        } else {
            i++;
        }
    }

    if (!isStackEmpty(&stack)) {
        result = *(double*)popStack(&stack, status);
    }

    freeStack(&stack);
    return result;
}

// 应用数学函数
double applyFunction(char func, double value, Status* status) {
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
            return 0.0;
    }
}

// 打印错误信息
void printError(Status status) {
    if (status == ERROR) {
        printf("Error occurred.\n");
    }
}

// 主函数
int main() {
    char inputFilename[INITIAL_SIZE];
    char outputFilename[INITIAL_SIZE];

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

        Status status = InfixToPostfix(infix, postfix);
        if (status != OK) {
            printError(status);
            fprintf(outputFile, "Error: ");
            printError(status);
            continue;
        }

        double result = evaluatePostfix(postfix, &status);
        if (status != OK) {
            printError(status);
            fprintf(outputFile, "Error: ");
            printError(status);
            continue;
        }

        printf("Result: %.15lf\n", result);
        fprintf(outputFile, "Result: %.15lf\n", result);
    }

    fclose(inputFile);
    fclose(outputFile);
    return 0;
}