#include "LinkedStack.h"
#include<string.h>
#include <stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include<windows.h>

// 初始化栈
void initStack(Stack* s, int elementSize) {
    constructList(s, elementSize);
}

// 判断栈是否为空
Bool isStackEmpty(Stack* s) {
    return s->head == NULL;
}

// 压栈
ErrorCode pushStack(Stack* s, void* item) {
    Node* newNode = NULL;
    int retries = 3; // 重试次数

    while (retries-- > 0) {
        newNode = createNode(item, s->elementSize);
        if (newNode) break; // 分配成功，退出循环

        // 记录日志
        fprintf(stderr, "Memory allocation failed during push operation. Retrying...\n");

        // 等待一段时间后重试
        sleep(1);
    }

    if (!newNode) {
        fprintf(stderr, "Failed to allocate memory after multiple attempts.\n");
        return ERR_MEMORY_ALLOCATION;
    }

    // 插入节点到链表头部
    newNode->next = s->head;
    s->head = newNode;
    s->length++;
    return ERR_NONE;
}

// 出栈
void* popStack(Stack* s, ErrorCode* err) {
    if (isStackEmpty(s)) {
        *err = ERR_STACK_UNDERFLOW;
        return NULL;
    }
    Node* temp = s->head;
    void* item = malloc(s->elementSize);
    if (!item) {
        *err = ERR_MEMORY_ALLOCATION;
        return NULL;
    }
    memcpy(item, temp->data, s->elementSize);
    s->head = s->head->next;
    freeNode(temp);
    s->length--;
    *err = ERR_NONE;
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
    free(s);
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