#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

#include"Stack.h"
#include"common.h"

#define INITIAL_SIZE 256
#define INCREMENT 10
#define PI 3.141592653589793

//初始化函数
void initStack(Stack *s, int elementSize) {
    s->data = malloc(INITIAL_SIZE * elementSize);
    if (!s->data) {
        fprintf(stderr, "Memory allocation error during stack initialization\n");
        exit(EXIT_FAILURE);
    }
    s->top = -1;
    s->elementSize = elementSize;
    s->stackSize = INITIAL_SIZE;
    debugPrint("Initialized stack with size: %d\n", INITIAL_SIZE);
}

//判断栈是否为空
int isStackEmpty(Stack *s) {
    return s->top == -1;
}

//入栈
ErrorCode pushStack(Stack *s, void* item) {
    if (s->top >= s->stackSize - 1) {
        // Expand stack capacity
        s->stackSize += INCREMENT;
        void* newData = realloc(s->data, s->stackSize * s->elementSize);
        if (!newData) {
            return ERR_MEMORY_ALLOCATION;
        }
        s->data = newData;
        debugPrint("Expanded stack to new size: %d\n", s->stackSize);
    }
    memcpy((char*)s->data + (++s->top) * s->elementSize, item, s->elementSize);
    debugPrint("Pushed item to stack, new top index: %d\n", s->top);
    return ERR_NONE;
}

//出栈
void* popStack(Stack *s, ErrorCode *err) {
    if (!isStackEmpty(s)) {
        void* item = malloc(s->elementSize);
        if (!item) {
            *err = ERR_MEMORY_ALLOCATION;
            return NULL;
        }
        memcpy(item, (char*)s->data + s->top * s->elementSize, s->elementSize);
        s->top--;
        *err = ERR_NONE;
        debugPrint("Popped item from stack, new top index: %d\n", s->top);
        return item;
    } else {
        *err = ERR_STACK_UNDERFLOW;
        debugPrint("Pop operation failed: stack underflow\n");
        return NULL;
    }
}


//查看栈顶元素
void* peekStack(Stack *s) {
    if (!isStackEmpty(s)) {
        void* item = (char*)s->data + s->top * s->elementSize;
        debugPrint("Peeked at stack, top index: %d\n", s->top);
        return item;
    } else {
        debugPrint("Peek operation failed: stack is empty\n");
        return NULL;
    }
}

// 清空栈
void clearStack(Stack *s) {
    s->top = -1;
    debugPrint("Cleared stack\n");
}

// 返回栈的长度
int stackLength(Stack *s) {
    return s->top + 1;
}

// 遍历栈
Status stackTraverse(Stack *s, void(*visit)(void*)) {
    if (isStackEmpty(s)) {
        return ERROR;
    }
    for (int i = 0; i <= s->top; i++) {
        visit((char*)s->data + i * s->elementSize);
    }
    return OK;
}

// 释放栈的内存空间
void stackFree(Stack *s) {
    if (s->data) {
        free(s->data);
        s->data = NULL;
        debugPrint("Freed stack memory\n");
    }
    s->top = -1;
    s->stackSize = 0;
}

// 创建一个栈，并返回指向该栈的指针
Stack* stackConstruct(int elementSize) {
    Stack *s = (Stack*)malloc(sizeof(Stack));
    if (!s) {
        fprintf(stderr, "Memory allocation error during stack construction\n");
        exit(EXIT_FAILURE);
    }
    initStack(s, elementSize);
    return s;
}

// 销毁栈
void stackDestruct(Stack *s) {
    stackFree(s);
    free(s);
}

//判断栈是否已满
int isStackFull(Stack *s){
    return s->top >= s->stackSize - 1;
}
