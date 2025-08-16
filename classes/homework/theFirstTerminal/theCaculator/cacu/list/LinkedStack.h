#ifndef LINKEDSTACK_H
#define LINKEDSTACK_H

#include "common.h"
#include "LinkedList.h"

// 基于链表的栈结构
typedef struct {
    Node* top;
    int elementSize;
} Stack;

// 栈操作函数原型
void initStack(Stack* s, int elementSize);
Bool isStackEmpty(Stack* s);
Status pushStack(Stack* s, void* item);
void* popStack(Stack* s, Status* status);
void* peekStack(Stack* s);
void freeStack(Stack* s);

#endif // LINKEDSTACK_H