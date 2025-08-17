#include <stdio.h>
#include <stdlib.h>

typedef int DataType;
typedef enum { ERR_NONE = 0, ERR_OVERFLOW = 1, ERR_UNDERFLOW = 2 } ErrorCode;

/* 链栈结点定义 */
typedef struct LStackNode {
    DataType data;
    struct LStackNode* next;
} LStackNode;

/* 链栈结构定义 */
typedef struct {
    LStackNode* top;
    int count;
} LinkedStack;

/* 初始化链栈 */
void initStack(LinkedStack* s) {
    s->top = NULL;
    s->count = 0;
}

/* 判断栈是否为空 */
int isStackEmpty(LinkedStack* s) {
    return s->top == NULL;
}

/* 入栈操作 */
ErrorCode pushStack(LinkedStack* s, DataType item) {
    LStackNode* node = (LStackNode*)malloc(sizeof(LStackNode));
    if (!node) {
        return ERR_OVERFLOW;
    }
    node->data = item;
    node->next = s->top;
    s->top = node;
    s->count++;
    return ERR_NONE;
}

/* 出栈操作 */
DataType* popStack(LinkedStack* s, ErrorCode* err) {
    if (isStackEmpty(s)) {
        *err = ERR_UNDERFLOW;
        return NULL;
    }
    LStackNode* node = s->top;
    s->top = node->next;
    DataType* item = &(node->data);
    free(node);
    s->count--;
    *err = ERR_NONE;
    return item;
}

/* 查看栈顶元素 */
DataType* peekStack(LinkedStack* s) {
    if (isStackEmpty(s)) {
        return NULL;
    }
    return &(s->top->data);
}

/* 清空栈 */
void clearStack(LinkedStack* s) {
    LStackNode* node;
    while (s->top != NULL) {
        node = s->top;
        s->top = node->next;
        free(node);
    }
    s->count = 0;
}

/* 获取栈的长度 */
int stackLength(LinkedStack* s) {
    return s->count;
}

/* 释放栈 */
void stackFree(LinkedStack* s) {
    clearStack(s);
    free(s);
}

/* 遍历栈 */
void stackTraverse(LinkedStack* s, void (*visit)(DataType)) {
    LStackNode* node = s->top;
    while (node != NULL) {
        visit(node->data);
        node = node->next;
    }
}

/* 创建一个栈 */
LinkedStack* stackConstruct() {
    LinkedStack* s = (LinkedStack*)malloc(sizeof(LinkedStack));
    if (s == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    initStack(s);
    return s;
}

/* 销毁栈 */
void stackDestruct(LinkedStack* s) {
    stackFree(s);
}

/* 打印元素 */
void printElem(DataType elem) {
    printf("%d ", elem);
}

/* 排序栈中的元素 */
void sortStack(LinkedStack* s) {
    if (stackLength(s) < 2) {
        return;
    }
    for (LStackNode* i = s->top; i != NULL; i = i->next) {
        for (LStackNode* j = i->next; j != NULL; j = j->next) {
            if (i->data > j->data) {
                DataType temp = i->data;
                i->data = j->data;
                j->data = temp;
            }
        }
    }
}

/* 检查括号匹配 */
int checkBrackets(LinkedStack* s, const char* expr) {
    ErrorCode err;
    for (int i = 0; expr[i] != '\0'; i++) {
        if (expr[i] == '(' || expr[i] == '{' || expr[i] == '[') {
            pushStack(s, expr[i]);
        } else if (expr[i] == ')' || expr[i] == '}' || expr[i] == ']') {
            if (isStackEmpty(s)) {
                return 0; // 不匹配
            }
            DataType* top = popStack(s, &err);
            if ((expr[i] == ')' && *top != '(') ||
                (expr[i] == '}' && *top != '{') ||
                (expr[i] == ']' && *top != '[')) {
                return 0; // 不匹配
            }
        }
    }
    return isStackEmpty(s);
}

/* 求后缀表达式的值 */
int evaluatePostfix(LinkedStack* s, char exp[]) {
    int i, a, b, c;
    ErrorCode err;
    for (i = 0; exp[i] != '\0'; i++) {
        if ('0' <= exp[i] && exp[i] <= '9') {
            pushStack(s, exp[i] - '0');
        } else {
            b = *popStack(s, &err);
            a = *popStack(s, &err);
            switch (exp[i]) {
                case '+': c = a + b; break;
                case '-': c = a - b; break;
                case '*': c = a * b; break;
                case '/': c = a / b; break;
            }
            pushStack(s, c);
        }
    }
    return *popStack(s, &err);
}

/* 测试函数 */
void testLinkedStack() {
    LinkedStack* stack = stackConstruct();
    for (int i = 0; i < 10; i++) {
        pushStack(stack, i);
    }
    printf("Stack elements: ");
    stackTraverse(stack, printElem);
    printf("\nTop element: %d\n", *peekStack(stack));
    sortStack(stack);
    printf("Sorted stack elements: ");
    stackTraverse(stack, printElem);
    printf("\n");

    const char* expr = "{[()]}";
    printf("Bracket matching for \"%s\": %s\n", expr, checkBrackets(stack, expr) ? "matched" : "not matched");

    char postfixExp[] = "512+4*+3-";
    printf("Postfix expression evaluation for \"%s\": %d\n", postfixExp, evaluatePostfix(stack, postfixExp));

    DataType x;
    ErrorCode err;
    while (!isStackEmpty(stack)) {
        DataType* elem = popStack(stack, &err);
        if (elem != NULL) {
            printf("Popped element: %d\n", *elem);
        }
    }
    stackDestruct(stack);
}

int main() {
    testLinkedStack();
    return 0;
}