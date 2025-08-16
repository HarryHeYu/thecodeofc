#ifndef LINKEDSTACK_H
#define LINKEDSTACK_H

#include "common.h"
#include "LinkedList.h"

#include<string.h>

// 基于链表的栈结构
typedef LinkedList Stack;

// 栈操作函数原型
void initStack(Stack* s, int elementSize);
//初始化s指向的栈，元素大小为elementSize

Bool isStackEmpty(Stack* s);
//判断s指向的栈是否为空

ErrorCode pushStack(Stack* s, void* item);
//将item压入s指向的栈  成功返回OK，失败返回ERROR

void* popStack(Stack* s, ErrorCode* err);
//将s指向的栈顶元素弹出，并返回其值  成功返回栈顶元素，失败返回NULL

void* peekStack(Stack* s);
//返回s指向的栈顶元素，但不弹出  成功返回栈顶元素，失败返回NULL

void clearStack(Stack* s);
//清空s指向的栈

void destructStack(Stack* s);
//销毁s指向的栈

int StackLength(Stack *s);
// 返回栈的长度

Status GetTop(Stack *s, void *e);
// 取栈顶元素，用e返回栈顶元素

Status StackTraverse(Stack *s, void(*visit)(void*));
// 遍历栈，用visit函数访问栈中的每个元素

void StackFree(Stack *s);
// 释放栈的内存空间

Stack* StackConstruct();
// 创建一个栈，并返回指向该栈的指针


#endif // LINKEDSTACK_H
