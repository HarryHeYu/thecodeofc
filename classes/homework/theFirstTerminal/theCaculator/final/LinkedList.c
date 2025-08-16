#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"

// 创建新节点
Node* createNode(void* data, int dataSize) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) return NULL;
    newNode->data = malloc(dataSize);
    if (!newNode->data) {
        free(newNode);
        return NULL;
    }
    memcpy(newNode->data, data, dataSize);
    newNode->next = NULL;
    return newNode;
}

// 释放节点
void freeNode(Node* node) {
    if (node) {
        free(node->data);
        free(node);
    }
}

// 构造函数
void constructList(LinkedList* list, int elementSize) {
    list->head = NULL;
    list->elementSize = elementSize;
    list->length = 0;
}

// 清空链表
void clearList(LinkedList* list) {
    while (list->head) {
        Node* temp = list->head;
        list->head = list->head->next;
        freeNode(temp);
    }
    list->length = 0;
}

// 析构函数
void destructList(LinkedList* list) {
    clearList(list);
}

// 获取链表长度
int getListLength(LinkedList* list) {
    return list->length;
}

// 获取节点数据
void* getNodeData(Node* node) {
    return node->data;
}

// 头部插入
Status insertAtFront(LinkedList* list, void* data) {
    Node* newNode = createNode(data, list->elementSize);
    if (!newNode) return ERROR;
    newNode->next = list->head;
    list->head = newNode;
    list->length++;
    return OK;
}

// 尾部插入
Status appendToEnd(LinkedList* list, void* data) {
    Node* newNode = createNode(data, list->elementSize);
    if (!newNode) return ERROR;
    if (!list->head) {
        list->head = newNode;
    } else {
        Node* p = list->head;
        while (p->next) {
            p = p->next;
        }
        p->next = newNode;
    }
    list->length++;
    return OK;
}

// 在指定节点后插入
Status insertAfter(LinkedList* list, Node* node, void* data) {
    if (!node) return ERROR;
    Node* newNode = createNode(data, list->elementSize);
    if (!newNode) return ERROR;
    newNode->next = node->next;
    node->next = newNode;
    list->length++;
    return OK;
}

// 删除第一个节点
Status deleteFirstNode(LinkedList* list) {
    if (!list->head) return ERROR;
    Node* temp = list->head;
    list->head = list->head->next;
    freeNode(temp);
    list->length--;
    return OK;
}

// 删除最后一个节点
Status deleteLastNode(LinkedList* list) {
    if (!list->head) return ERROR;
    Node* p = list->head;
    if (!p->next) {
        list->head = NULL;
    } else {
        while (p->next->next) {
            p = p->next;
        }
        freeNode(p->next);
        p->next = NULL;
    }
    list->length--;
    return OK;
}

// 删除指定节点
Status deleteNode(LinkedList* list, Node* node) {
    if (!list->head || !node) return ERROR;
    if (list->head == node) {
        list->head = list->head->next;
        freeNode(node);
    } else {
        Node* p = list->head;
        while (p->next && p->next != node) {
            p = p->next;
        }
        if (!p->next) return ERROR;
        p->next = node->next;
        freeNode(node);
    }
    list->length--;
    return OK;
}

// 查找节点
Node* searchList(LinkedList* list, void* data, int (*cmp)(const void*, const void*)) {
    Node* p = list->head;
    while (p) {
        if (cmp(p->data, data) == 0) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

// 遍历链表
void traverseList(LinkedList* list, void (*visit)(void*)) {
    Node* p = list->head;
    while (p) {
        visit(p->data);
        p = p->next;
    }
}

// 链表冒泡排序
void sortList(LinkedList* list, int (*cmp)(const void*, const void*)) {
    if (!list->head || !list->head->next) return;
    for (Node* i = list->head; i->next; i = i->next) {
        for (Node* j = list->head; j->next; j = j->next) {
            if (cmp(j->data, j->next->data) > 0) {
                void* temp = j->data;
                j->data = j->next->data;
                j->next->data = temp;
            }
        }
    }
}