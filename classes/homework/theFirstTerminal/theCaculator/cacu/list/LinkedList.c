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