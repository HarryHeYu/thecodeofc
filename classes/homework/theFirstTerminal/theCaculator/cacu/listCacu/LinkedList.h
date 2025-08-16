#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "common.h"

// 链表节点结构
typedef struct Node {
    void* data;
    struct Node* next;
} Node;

// 链表结构和栈结构
typedef struct {
    Node* head;
    int elementSize;
    int length;
} LinkedList;

// 链表操作函数原型
Node* createNode(void* data, int dataSize);
//创建一个节点data，dataSize为数据大小   并返回该节点的指针

void freeNode(Node* node);
////释放一个node指向的节点

void constructList(LinkedList* list, int elementSize);
//创建一个链表List，elementSize为数据大小

void clearList(LinkedList* list);
//清空list指向的链表

void destructList(LinkedList* list);
//销毁list指向的链表

int getListLength(LinkedList* list);
//返回list指向的链表的长度

void* getNodeData(Node* node);
//返回node指向的节点的数据

Status insertAtFront(LinkedList* list, void* data);
//在list指向的链表头部插入一个data指向的数据 成功在头部插入元素，失败返回NULL

Status appendToEnd(LinkedList* list, void* data);
//在list指向的链表尾部插入一个data指向的数据  成功在尾部插入元素，失败返回NULL

Status insertAfter(LinkedList* list, Node* node, void* data);
//在list指向的链表中node指向的节点之后插入一个data指向的数据  成功在node之后插入元素，失败返回NULL

Status deleteFirstNode(LinkedList* list);
//删除list指向的链表头部的节点  成功删除头部节点，失败返回NULL

Status deleteLastNode(LinkedList* list);
//删除list指向的链表尾部的节点  成功删除尾部节点，失败返回NULL

Status deleteNode(LinkedList* list, Node* node);
//删除list指向的链表中node指向的节点  成功删除节点，失败返回NULL

Node* searchList(LinkedList* list, void* data, int (*cmp)(const void*, const void*));
//在list指向的链表中查找data指向的数据，cmp为比较函数  

void traverseList(LinkedList* list, void (*visit)(void*));
//遍历list指向的链表，visit为访问函数

void sortList(LinkedList* list, int (*cmp)(const void*, const void*));
//对list指向的链表进行冒泡排序，cmp为比较函数

#endif // LINKEDLIST_H
