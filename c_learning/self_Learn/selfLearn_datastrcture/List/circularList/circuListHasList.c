#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 定义链表节点结构体
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// 定义链表结构体
typedef struct {
    Node* head;
    int length;
} CircularLinkedList;

// 定义状态枚举
typedef enum {
    ERROR = 0,
    OK = 1
} Status;

// 初始化循环链表
Status init_list(CircularLinkedList* list) {
    list->head = (Node*)malloc(sizeof(Node));
    if (!list->head) {
        printf("Memory allocation failed\n");
        return ERROR;
    }
    list->head->next = list->head; // 头节点指向自己，形成循环
    list->length = 0;
    return OK;
}

// 创建一个节点
Node* create_node(int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

// 销毁循环链表
void destroy_list(CircularLinkedList* list) {
    Node* current = list->head->next;
    Node* next;
    while (current != list->head) {
        next = current->next;
        free(current);
        current = next;
    }
    free(list->head);
    list->head = NULL;
    list->length = 0;
}

// 判断循环链表是否为空
bool is_empty(CircularLinkedList* list) {
    return list->head->next == list->head;
}

// 获取循环链表长度
int get_length(CircularLinkedList* list) {
    return list->length;
}

// 打印循环链表
void print_list(CircularLinkedList* list) {
    Node* current = list->head->next;
    while (current != list->head) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("HEAD\n");
}

// 插入节点到链表头部
Status insert_head(CircularLinkedList* list, int data) {
    Node* new_node = create_node(data);
    if (!new_node) return ERROR;
    new_node->next = list->head->next;
    list->head->next = new_node;
    list->length++;
    return OK;
}

// 插入节点到链表尾部
Status append_node(CircularLinkedList* list, int data) {
    Node* new_node = create_node(data);
    if (!new_node) return ERROR;
    Node* current = list->head;
    while (current->next != list->head) {
        current = current->next;
    }
    new_node->next = list->head;
    current->next = new_node;
    list->length++;
    return OK;
}

// 删除指定节点
Status delete_node(CircularLinkedList* list, Node* node) {
    if (is_empty(list) || !node) return ERROR;
    Node* current = list->head;
    while (current->next != list->head && current->next != node) {
        current = current->next;
    }
    if (current->next == node) {
        current->next = node->next;
        free(node);
        list->length--;
        return OK;
    }
    return ERROR;
}

// 按条件删除节点
Status delete_if(CircularLinkedList* list, bool (*condition)(Node*)) {
    Node* current = list->head;
    while (current->next != list->head) {
        if (condition(current->next)) {
            Node* temp = current->next;
            current->next = temp->next;
            free(temp);
            list->length--;
        } else {
            current = current->next;
        }
    }
    return OK;
}

// 删除头结点后的第一个节点
Status delete_head(CircularLinkedList* list) {
    if (is_empty(list)) return ERROR;
    Node* temp = list->head->next;
    list->head->next = temp->next;
    free(temp);
    list->length--;
    return OK;
}

// 删除尾节点
Status delete_tail(CircularLinkedList* list) {
    if (is_empty(list)) return ERROR;
    Node* current = list->head;
    while (current->next->next != list->head) {
        current = current->next;
    }
    Node* temp = current->next;
    current->next = list->head;
    free(temp);
    list->length--;
    return OK;
}

// 交换两个节点
Status swap_nodes(CircularLinkedList* list, int i, int j) {
    if (i == j || i < 1 || j < 1 || i > list->length || j > list->length) return ERROR;
    if (i > j) {
        int temp = i;
        i = j;
        j = temp;
    }
    Node* pre_i = list->head;
    for (int k = 1; k < i; k++) pre_i = pre_i->next;
    Node* pre_j = list->head;
    for (int k = 1; k < j; k++) pre_j = pre_j->next;
    Node* node_i = pre_i->next;
    Node* node_j = pre_j->next;
    if (node_i == node_j) {
        node_i->next = node_j->next;
        node_j->next = node_i;
        pre_i->next = node_j;
    } else {
        Node* temp = node_j->next;
        pre_i->next = node_j;
        node_j->next = node_i->next;
        pre_j->next = node_i;
        node_i->next = temp;
    }
    return OK;
}

// 升序排列链表
Status ascending_list(CircularLinkedList* list) {
    if (is_empty(list) || list->head->next->next == list->head) return OK;
    Node* p = list->head->next;
    while (p->next != list->head) {
        Node* q = p->next;
        while (q != list->head) {
            if (p->data > q->data) {
                int temp = p->data;
                p->data = q->data;
                q->data = temp;
            }
            q = q->next;
        }
        p = p->next;
    }
    return OK;
}

// 反转链表
Status reverse_list(CircularLinkedList* list) {
    if (is_empty(list) || list->head->next->next == list->head) return OK;
    Node* prev = list->head;
    Node* current = list->head->next;
    Node* next;
    while (current != list->head) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    list->head->next->next = prev;
    list->head->next = prev;
    return OK;
}

// 遍历链表
Status traverse_list(CircularLinkedList* list, Status (*visit)(Node*)) {
    Node* current = list->head->next;
    while (current != list->head) {
        if (visit(current) != OK) return ERROR;
        current = current->next;
    }
    return OK;
}

// 打印节点数据
Status print_node(Node* node) {
    printf("%d -> ", node->data);
    return OK;
}

// 查找节点
Node* find_node(CircularLinkedList* list, int data) {
    Node* current = list->head->next;
    while (current != list->head && current->data != data) {
        current = current->next;
    }
    return current != list->head ? current : NULL;
}

// 清空链表
Status clear_list(CircularLinkedList* list) {
    Node* current = list->head->next;
    Node* next;
    while (current != list->head) {
        next = current->next;
        free(current);
        current = next;
    }
    list->head->next = list->head;
    list->length = 0;
    return OK;
}

// 测试代码
int main() {
    CircularLinkedList list;
    if (init_list(&list) == ERROR) {
        return 1;
    }

    append_node(&list, 3);
    append_node(&list, 1);
    append_node(&list, 4);
    append_node(&list, 2);
    append_node(&list, 5);

    printf("链表中的元素为：\n");
    print_list(&list);

    delete_head(&list);
    printf("删除头结点后的链表：\n");
    print_list(&list);

    delete_tail(&list);
    printf("删除尾节点后的链表：\n");
    print_list(&list);

    swap_nodes(&list, 2, 3);
    printf("交换第2和第3个节点后的链表：\n");
    print_list(&list);

    ascending_list(&list);
    printf("升序排列后的链表：\n");
    print_list(&list);

    reverse_list(&list);
    printf("反转后的链表：\n");
    print_list(&list);

    destroy_list(&list);
    return 0;
}