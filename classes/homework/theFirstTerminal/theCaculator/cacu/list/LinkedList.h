#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "common.h"

// 链表节点结构
typedef struct Node {
    void* data;
    struct Node* next;
} Node;

// 链表节点操作函数原型
Node* createNode(void* data, int dataSize);
void freeNode(Node* node);

#endif // LINKEDLIST_H