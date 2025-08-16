#ifndef STACK_H
#define STACK_H
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include"common.h"
#define INITIAL_SIZE 256
#define INCREMENT 10
/*定义栈*/
typedef struct {
    void* data;
    int top;
    int elementSize;
    int stackSize;
} Stack;
// 函数声明
void initStack(Stack *s, int elementSize);

/*初始化栈 s为指向栈的指针返回为空*/
int isStackEmpty(Stack *s);
/*判断栈是否为空 返回s指向的栈是否为空，1为空否则为0*/
ErrorCode pushStack(Stack *s, void* item);
/*入栈 将item压入s指向的栈中，成功则返回ERR_NONE，若溢栈则扩栈*/
void* popStack(Stack *s, ErrorCode *err);
/*出栈 返回s指向的栈中的栈顶元素，错误则返回NULL*/
void* peekStack(Stack *s);
/*查看栈顶元素 返回s指向的栈的栈顶元素，错误则返回NULL*/
void clearStack(Stack *s);
/*清空栈 返回s指向的栈的栈顶元素，错误则返回NULL*/
int StackLength(Stack *s);
/*返回栈的长度*/
Status StackTraverse(Stack *s, void(*visit)(void*));
/*遍历栈，用visit函数访问栈中的每个元素*/
void stackFree(Stack *s);
/*释放栈的内存空间*/
Stack* StackConstruct();
/*创建一个栈，并返回指向该栈的指针*/
void StackDestruct(Stack *s);
/*销毁栈，释放其内存空间*/
int isStackFull(Stack *s);
/*判断栈是否已满，返回1表示已满，返回0表示未满*/

#endif // STACK_H
