#include "LinkedStack.h"

#include <stdlib.h>

// 错误代码
typedef enum {
    ERR_NONE,
    ERR_STACK_OVERFLOW,
    ERR_STACK_UNDERFLOW,
    ERR_MEMORY_ALLOCATION
} ErrorCode;

// 初始化栈
void initStack(Stack* s, int elementSize) {
    constructList(s, elementSize);
}

// 判断栈是否为空
Bool isStackEmpty(Stack* s) {
    return s->head == NULL;
}

// 压栈
Status pushStack(Stack* s, void* item) {
    return insertAtFront(s, item);
}

// 出栈
void* popStack(Stack* s, Status* status) {
    if (isStackEmpty(s)) {
        *status = ERROR;
        return NULL;
    }
    Node* temp = s->head;
    void* item = temp->data;
    s->head = s->head->next;
    free(temp);
    s->length--;
    *status = OK;
    return item;
}

// 查看栈顶元素
void* peekStack(Stack* s) {
    if (isStackEmpty(s)) {
        return NULL;
    }
    return s->head->data;
}

// 清空栈
void clearStack(Stack* s) {
    clearList(s);
}

// 析构函数
void destructStack(Stack* s) {
    destructList(s);
}

// 栈长度
int StackLength(Stack *s) {
    return s->length;
}

// 获取栈顶元素
Status GetTop(Stack *s, void *e) {
    if (isStackEmpty(s)) {
        return ERROR;
    }
    e = peekStack(s);
    return OK;
}

// 遍历栈
Status StackTraverse(Stack *s, void(*visit)(void*)) {
    Node *current = s->head;
    while (current) {
        visit(current->data);
        current = current->next;
    }
    return OK;
}

// 释放栈的内存空间
void StackFree(Stack *s) {
    clearStack(s);
}

// 创建一个栈
Stack* StackConstruct() {
    Stack *s = (Stack*)malloc(sizeof(Stack));
    if (s) {
        constructList(s, sizeof(void*));
        return s;
    }
    return NULL;
}