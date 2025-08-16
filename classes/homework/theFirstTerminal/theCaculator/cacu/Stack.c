#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

#include"Stack.h"
#include"common.h"

#define INITIAL_SIZE 100
#define INCREMENT 10
#define PI 3.141592653589793

typedef struct {
    void** data;
    int top;
    int elementSize;
    int stackSize;
} Stack;


//初始化函数
void initStack(Stack *s, int elementSize) {
    s->data = (void**)malloc(INITIAL_SIZE * elementSize);
    if (!s->data) {
        fprintf(stderr, "Memory allocation error during stack initialization\n");
        exit(EXIT_FAILURE);
    }
    s->top = -1;
    s->elementSize = elementSize;
    s->stackSize = INITIAL_SIZE;
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
        void** newData = (void**)realloc(s->data, s->stackSize * s->elementSize);
        if (!newData) {
            return ERR_MEMORY_ALLOCATION;
        }
        s->data = newData;
        debugPrint("Expanded stack to new size: %d\n", s->stackSize);
    }
    s->data[++(s->top)] = item;
    debugPrint("Pushed item to stack\n");
    return ERR_NONE;
}

//出栈
void* popStack(Stack *s, ErrorCode *err) {
    if (!isStackEmpty(s)) {
        void* item = s->data[(s->top)--];
        debugPrint("Popped item from stack\n");
        *err = ERR_NONE;
        return item;
    } else {
        *err = ERR_STACK_UNDERFLOW;
        return NULL;
    }
}

//查看栈顶元素
void* peekStack(Stack *s) {
    if (!isStackEmpty(s)) {
        return s->data[s->top];
    } else {
        return NULL;
    }
}

// 清空栈
void clearStack(Stack *s) {
    s->top = -1;
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
        visit(s->data[i]);
    }
    return OK;
}

// 释放栈的内存空间
void stackFree(Stack *s) {
    if (s->data) {
        for (int i = 0; i <= s->top; i++) {
            free(s->data[i]); // 释放每个元素的内存
        }
        free(s->data);
        s->data = NULL;
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