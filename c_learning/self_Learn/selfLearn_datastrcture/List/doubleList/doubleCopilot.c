#include <stdlib.h>
#include <stdio.h>

// 双向链表节点结构体
typedef struct Node {
    struct Node *prev;
    struct Node *next;
    int data; // 节点数据
} Node, *NodePtr;

// 双向链表结构体
typedef struct List {
    NodePtr head;
    NodePtr tail;
} List, *ListPtr;

// 状态枚举类型，用于表示函数执行结果
typedef enum { ERROR = 0, OK = 1 } Status;

/**
 * @brief 初始化链表
 * @param list 指向链表结构体的指针
 * @return OK 表示成功，ERROR 表示失败
 */
Status listInit(ListPtr list) {
    if (list == NULL) {
        return ERROR;
    }
    list->head = NULL;
    list->tail = NULL;
    return OK;
}

/**
 * @brief 获取链表的头节点
 * @param list 指向链表结构体的指针
 * @return 链表的头节点指针
 */
NodePtr listGetHead(ListPtr list) {
    return list->head;
}

/**
 * @brief 获取链表的尾节点
 * @param list 指向链表结构体的指针
 * @return 链表的尾节点指针
 */
NodePtr listGetTail(ListPtr list) {
    return list->tail;
}

/**
 * @brief 获取当前节点的前一个节点
 * @param node 指向当前节点的指针
 * @return 当前节点的前一个节点指针
 */
NodePtr listGetPrev(NodePtr node) {
    return node->prev;
}

/**
 * @brief 获取当前节点的下一个节点
 * @param node 指向当前节点的指针
 * @return 当前节点的下一个节点指针
 */
NodePtr listGetNext(NodePtr node) {
    return node->next;
}

/**
 * @brief 创建一个新的双向链表
 * @return 指向创建的双向链表结构体的指针
 */
ListPtr listCreate(void) {
    ListPtr list = (ListPtr)malloc(sizeof(List));
    if (list != NULL) {
        if (listInit(list) == ERROR) {
            free(list);
            return NULL;
        }
    }
    return list;
}

/**
 * @brief 创建一个新的双向链表节点
 * @param data 节点数据
 * @return 指向创建的双向链表节点的指针
 */
NodePtr nodeCreate(int data) {
    NodePtr node = (NodePtr)malloc(sizeof(Node));
    if (node != NULL) {
        node->prev = NULL;
        node->next = NULL;
        node->data = data;
    }
    return node;
}

/**
 * @brief 在链表的前端插入一个节点
 * @param list 指向链表结构体的指针
 * @param node 指向要插入的节点的指针
 * @return OK 表示成功，ERROR 表示失败
 */
Status listPushFront(ListPtr list, NodePtr node) {
    if (list == NULL || node == NULL) {
        return ERROR;
    }
    node->prev = NULL;
    node->next = list->head;
    if (list->head) {
        list->head->prev = node;
    } else {
        list->tail = node;
    }
    list->head = node;
    return OK;
}

/**
 * @brief 在链表的末端插入一个节点
 * @param list 指向链表结构体的指针
 * @param node 指向要插入的节点的指针
 * @return OK 表示成功，ERROR 表示失败
 */
Status listPushBack(ListPtr list, NodePtr node) {
    if (list == NULL || node == NULL) {
        return ERROR;
    }
    node->next = NULL;
    node->prev = list->tail;
    if (list->tail) {
        list->tail->next = node;
    } else {
        list->head = node;
    }
    list->tail = node;
    return OK;
}

/**
 * @brief 在 before 之前插入节点，如果 before 为 NULL，则插入到链表末尾
 * @param list 指向链表结构体的指针
 * @param before 指向要插入位置之前的节点的指针
 * @param node 指向要插入的节点的指针
 * @return OK 表示成功，ERROR 表示失败
 */
Status listInsertBefore(ListPtr list, NodePtr before, NodePtr node) {
    if (list == NULL || node == NULL) {
        return ERROR;
    }
    if (before) {
        if (before->prev) {
            before->prev->next = node;
        } else {
            list->head = node;
        }
        node->next = before;
        node->prev = before->prev;
        before->prev = node;
    } else {
        return listPushBack(list, node);
    }
    return OK;
}

/**
 * @brief 在 after 之后插入节点，如果 after 为 NULL，则插入到链表前端
 * @param list 指向链表结构体的指针
 * @param after 指向要插入位置之后的节点的指针
 * @param node 指向要插入的节点的指针
 * @return OK 表示成功，ERROR 表示失败
 */
Status listInsertAfter(ListPtr list, NodePtr after, NodePtr node) {
    if (list == NULL || node == NULL) {
        return ERROR;
    }
    if (after) {
        if (after->next) {
            after->next->prev = node;
        } else {
            list->tail = node;
        }
        node->prev = after;
        node->next = after->next;
        after->next = node;
    } else {
        return listPushFront(list, node);
    }
    return OK;
}

/**
 * @brief 从链表的前端移除并返回节点
 * @param list 指向链表结构体的指针
 * @return 指向移除的节点的指针
 */
NodePtr listPopFront(ListPtr list) {
    if (list == NULL || list->head == NULL) {
        return NULL;
    }

    NodePtr node = list->head;
    if (list->head->next) {
        list->head->next->prev = NULL;
    } else {
        list->tail = NULL;
    }
    list->head = list->head->next;
    node->prev = node->next = NULL;
    return node;
}

/**
 * @brief 从链表的末端移除并返回节点
 * @param list 指向链表结构体的指针
 * @return 指向移除的节点的指针
 */
NodePtr listPopBack(ListPtr list) {
    if (list == NULL || list->tail == NULL) {
        return NULL;
    }

    NodePtr node = list->tail;
    if (list->tail->prev) {
        list->tail->prev->next = NULL;
    } else {
        list->head = NULL;
    }
    list->tail = list->tail->prev;
    node->prev = node->next = NULL;
    return node;
}

/**
 * @brief 从链表中移除指定节点
 * @param list 指向链表结构体的指针
 * @param node 指向要移除的节点的指针
 * @return OK 表示成功，ERROR 表示失败
 */
Status listRemove(ListPtr list, NodePtr node) {
    if (list == NULL || node == NULL) {
        return ERROR;
    }
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
    }
    node->prev = NULL;
    node->next = NULL;
    return OK;
}

/**
 * @brief 检查链表是否为空
 * @param list 指向链表结构体的指针
 * @return 1 表示链表为空，0 表示链表不为空
 */
unsigned char listIsEmpty(ListPtr list) {
    if (list == NULL) {
        return 1;
    }
    if (list->head == NULL && list->tail == NULL) {
        return 1;
    }
    return 0;
}

/**
 * @brief 获取链表的大小
 * @param list 指向链表结构体的指针
 * @return 链表的节点数量
 */
unsigned long listGetSize(ListPtr list) {
    if (list == NULL) {
        return 0;
    }
    unsigned long size = 0;
    NodePtr node = listGetHead(list);
    while (node) {
        ++size;
        node = listGetNext(node);
    }
    return size;
}

/**
 * @brief 合并两个链表
 * @param list1 指向第一个链表结构体的指针
 * @param list2 指向第二个链表结构体的指针
 * @return OK 表示成功，ERROR 表示失败
 */
Status listCombine(ListPtr list1, ListPtr list2) {
    if (list1 == NULL || list2 == NULL) {
        return ERROR;
    }
    if (!listIsEmpty(list2)) {
        if (!listIsEmpty(list1)) {
            list1->tail->next = list2->head;
            list2->head->prev = list1->tail;
            list1->tail = list2->tail;
        } else {
            list1->head = list2->head;
            list1->tail = list2->tail;
        }
        list2->head = list2->tail = NULL;
    }
    return OK;
}

/**
 * @brief 清空链表
 * @param list 指向链表结构体的指针
 * @return OK 表示成功，ERROR 表示失败
 */
Status listClear(ListPtr list) {
    if (list == NULL) {
        return ERROR;
    }
    NodePtr node = list->head;
    while (node != NULL) {
        NodePtr temp = node;
        node = node->next;
        free(temp);
    }
    list->head = NULL;
    list->tail = NULL;
    return OK;
}

/**
 * @brief 查找链表中数据为 data 的节点
 * @param list 指向链表结构体的指针
 * @param data 要查找的节点数据
 * @return 指向找到的节点的指针，如果未找到则返回 NULL
 */
NodePtr listFind(ListPtr list, int data) {
    if (list == NULL) {
        return NULL;
    }
    NodePtr node = list->head;
    while (node != NULL) {
        if (node->data == data) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

/**
 * @brief 遍历链表并执行给定的函数
 * @param list 指向链表结构体的指针
 * @param func 要执行的函数
 * @return OK 表示成功，ERROR 表示失败
 */
Status listTraverse(ListPtr list, void (*func)(NodePtr)) {
    if (list == NULL || func == NULL) {
        return ERROR;
    }
    NodePtr node = list->head;
    while (node != NULL) {
        func(node);
        node = node->next;
    }
    return OK;
}

/**
 * @brief 销毁链表并释放内存
 * @param list 指向链表结构体的指针
 * @return OK 表示成功，ERROR 表示失败
 */
Status listDestroy(ListPtr list) {
    if (listClear(list) == ERROR) {
        return ERROR;
    }
    free(list);
    return OK;
}

#if defined DOUBLELIST_TEST
/**
 * @brief 打印节点信息
 * @param node 指向节点的指针
 */
void printNode(NodePtr node) {
    printf("Node data: %d\n", node->data);
}

/**
 * @brief 测试双向链表的功能
 */
void testList() {
    ListPtr list = listCreate();
    if (list == NULL) {
        printf("Failed to create double list.\n");
        return;
    }
    NodePtr node1 = nodeCreate(1);
    if (node1 == NULL) {
        printf("Failed to create node 1.\n");
        listDestroy(list);
        return;
    }
    NodePtr node2 = nodeCreate(2);
    if (node2 == NULL) {
        printf("Failed to create node 2.\n");
        free(node1);
        listDestroy(list);
        return;
    }

    if (listPushFront(list, node1) == ERROR) {
        printf("Failed to push node 1 to front.\n");
        free(node1);
        free(node2);
        listDestroy(list);
        return;
    }

    if (listPushBack(list, node2) == ERROR) {
        printf("Failed to push node 2 to back.\n");
        free(node1);
        free(node2);
        listDestroy(list);
        return;
    }

    printf("List size after insertions: %lu\n", listGetSize(list));

    listTraverse(list, printNode);

    NodePtr temp = listPopFront(list);
    printf("Popped from front: %d\n", temp->data);
    free(temp);

    temp = listPopBack(list);
    printf("Popped from back: %d\n", temp->data);
    free(temp);

    printf("List size after popping: %lu\n", listGetSize(list));

    listDestroy(list);
}

int main() {
    testList();
    return 0;
}
#endif