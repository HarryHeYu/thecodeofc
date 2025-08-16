#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 256
int main() {
    char exp[SIZE];
    double sum = 0, value;
    char op;
    int i = 0;
    printf("Enter an expression: ");
    fgets(exp, sizeof(exp), stdin);
    size_t len = strlen(exp);
    if (len > 0 && exp[len - 1] == '\n') {
        exp[len - 1] = '\0';
    }//检查是否为换行符，若是的话则将其换为'\0'将字符串结束
    while (isspace(exp[i])) {
        i++; 
    }
    if (isdigit(exp[i]) || exp[i] == '.') {
        value = atof(&exp[i]);
        while (isdigit(exp[i]) || exp[i] == '.') {
            i++;
        }
    } else {
        printf("Invalid expression.\n");
        return 1;
    }
    sum = value;
    while (exp[i] != '\0') {
        op = exp[i];
        if (op != '+' && op != '-' && op != '*' && op != '/') {
            printf("Invalid operator.\n");
            return 1;
        }
        i++;
        while (isspace(exp[i])) {
            i++; 
        }
        if (!isdigit(exp[i]) && exp[i] != '.') {
            printf("Invalid expression.\n");
            return 1;
        }
        value = atof(&exp[i]);
        while (isdigit(exp[i]) || exp[i] == '.') {
            i++;
        }
        switch (op) {
            case '+':
                sum += value;
                break;
            case '-':
                sum -= value;
                break;
            case '*':
                sum *= value;
                break;
            case '/':
                if (value == 0) {
                    printf("Division by zero.\n");
                    return 1;
                }
                sum /= value;
                break;
        }
    }
    printf("Value of expression: %lf\n", sum);
    return 0;
}