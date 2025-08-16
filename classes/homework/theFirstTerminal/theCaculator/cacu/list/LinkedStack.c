#include <stdlib.h>
#include "LinkedStack.h"

// 初始化栈
void initStack(Stack* s, int elementSize) {
    s->top = NULL;
    s->elementSize = elementSize;
}

// 判断栈是否为空
Bool isStackEmpty(Stack* s) {
    return s->top == NULL;
}

// 压栈
Status pushStack(Stack* s, void* item) {
    Node* newNode = createNode(item, s->elementSize);
    if (!newNode) return ERROR;
    newNode->next = s->top;
    s->top = newNode;
    return OK;
}

// 出栈
void* popStack(Stack* s, Status* status) {
    if (isStackEmpty(s)) {
        *status = ERROR;
        return NULL;
    }
    Node* temp = s->top;
    void* item = temp->data;
    s->top = temp->next;
    free(temp);
    *status = OK;
    return item;
}

// 查看栈顶元素
void* peekStack(Stack* s) {
    if (isStackEmpty(s)) {
        return NULL;
    }
    return s->top->data;
}

// 释放栈
void freeStack(Stack* s) {
    while (!isStackEmpty(s)) {
        Status status;
        void* item = popStack(s, &status);
        if (item) {
            free(item);
        }
    }
    s->top = NULL;
}