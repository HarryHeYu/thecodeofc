#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 定义链表节点结构体
typedef struct Node {
    void* data;
    struct Node* next;
} Node;

// 定义状态枚举
typedef enum {
    ERROR = 0,
    OK = 1
} Status;

// 创建一个新节点
Node* create_node(void* data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

// 初始化链表（创建头节点）
Node* init_list() {
    Node* head = create_node(NULL); // 创建头节点
    head->next = head; // 头节点指向自己，形成循环
    return head;
}

// 销毁链表
void destroy_list(Node* head) {
    Node* current = head->next;
    Node* next;
    while (current != head) {
        next = current->next;
        free(current);
        current = next;
    }
    free(head);
}

// 判断链表是否为空
bool is_empty(Node* head) {
    return head->next == head;
}

// 获取链表长度
int get_length(Node* head) {
    int length = 0;
    Node* current = head->next;
    while (current != head) {
        length++;
        current = current->next;
    }
    return length;
}

// 打印链表（假设数据类型为int）
void print_list(Node* head, void (*print_data)(void*)) {
    Node* current = head->next;
    while (current != head) {
        print_data(current->data);
        current = current->next;
    }
    printf("HEAD\n");
}

// 插入节点到链表头部
Status insert_head(Node* head, void* data) {
    Node* new_node = create_node(data);
    new_node->next = head->next;
    head->next = new_node;
    return OK;
}

// 插入节点到链表尾部
Status append_node(Node* head, void* data) {
    Node* new_node = create_node(data);
    Node* current = head;
    while (current->next != head) {
        current = current->next;
    }
    new_node->next = head;
    current->next = new_node;
    return OK;
}

// 删除指定节点
Status delete_node(Node* head, Node* node) {
    Node* current = head;
    while (current->next != head && current->next != node) {
        current = current->next;
    }
    if (current->next == node) {
        current->next = node->next;
        free(node);
        return OK;
    }
    return ERROR;
}

// 按条件删除节点
Status delete_if(Node* head, bool (*condition)(Node*)) {
    Node* current = head;
    while (current->next != head) {
        if (condition(current->next)) {
            Node* temp = current->next;
            current->next = temp->next;
            free(temp);
        } else {
            current = current->next;
        }
    }
    return OK;
}

// 删除头结点后的第一个节点
Status delete_head(Node* head) {
    if (is_empty(head)) return ERROR;
    Node* temp = head->next;
    head->next = temp->next;
    free(temp);
    return OK;
}

// 删除尾节点
Status delete_tail(Node* head) {
    if (is_empty(head)) return ERROR;
    Node* current = head;
    while (current->next->next != head) {
        current = current->next;
    }
    Node* temp = current->next;
    current->next = head;
    free(temp);
    return OK;
}

// 交换两个节点
Status swap_nodes(Node* head, int i, int j) {
    if (i == j) return OK;
    if (i > j) {
        int temp = i;
        i = j;
        j = temp;
    }
    Node* pre_i = head;
    for (int k = 1; k < i; k++) pre_i = pre_i->next;
    Node* pre_j = head;
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

// 升序排列链表（假设数据类型为int）
Status ascending_list(Node* head, int (*compare)(void*, void*)) {
    if (is_empty(head) || head->next->next == head) return OK;
    Node* p = head->next;
    while (p->next != head) {
        Node* q = p->next;
        while (q != head) {
            if (compare(p->data, q->data) > 0) {
                void* temp = p->data;
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
Status reverse_list(Node* head) {
    if (is_empty(head) || head->next->next == head) return OK;
    Node* prev = head;
    Node* current = head->next;
    Node* next;
    while (current != head) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    head->next->next = prev;
    head->next = prev;
    return OK;
}

// 遍历链表
Status traverse_list(Node* head, Status (*visit)(Node*)) {
    Node* current = head->next;
    while (current != head) {
        if (visit(current) != OK) return ERROR;
        current = current->next;
    }
    return OK;
}

// 打印节点数据（假设数据类型为int）
Status print_node(Node* node) {
    printf("%d -> ", *(int*)node->data);
    return OK;
}

// 插入节点到指定位置
Status insert_node(Node* head, void* data, int position) {
    if (position < 1 || position > get_length(head) + 1) {
        printf("Invalid position\n");
        return ERROR;
    }
    Node* new_node = create_node(data);
    Node* current = head;
    for (int i = 1; i < position; i++) {
        current = current->next;
    }
    new_node->next = current->next;
    current->next = new_node;
    return OK;
}

// 查找节点（假设数据类型为int）
Node* find_node(Node* head, void* data, int (*compare)(void*, void*)) {
    Node* current = head->next;
    while (current != head && compare(current->data, data) != 0) {
        current = current->next;
    }
    if (current == head) {
        return NULL;
    }
    return current;
}

// 获取前驱节点
Node* get_prev_node(Node* head, Node* node) {
    Node* current = head;
    while (current->next != head && current->next != node) {
        current = current->next;
    }
    if (current->next == head) {
        return NULL;
    }
    return current;
}

// 获取后继节点
Node* get_next_node(Node* node) {
    return node->next;
}

// 清空链表
Status clear_list(Node* head) {
    Node* current = head->next;
    Node* next;
    while (current != head) {
        next = current->next;
        free(current);
        current = next;
    }
    head->next = head;
    return OK;
}

// 获取节点所在位置
int get_node_position(Node* head, Node* node) {
    int position = 1;
    Node* current = head->next;
    while (current != head && current != node) {
        current = current->next;
        position++;
    }
    if (current == head) {
        return -1;
    }
    return position;
}

// 合并两个链表
Node* merge_lists(Node* head1, Node* head2) {
    Node* head = init_list();
    Node* current = head;
    
    Node* current1 = head1->next;
    Node* current2 = head2->next;
    
    while (current1 != head1 && current2 != head2) {
        current->next = create_node(current1->data);
        current = current->next;
        current1 = current1->next;
        
        current->next = create_node(current2->data);
        current = current->next;
        current2 = current2->next;
    }
    
    while (current1 != head1) {
        current->next = create_node(current1->data);
        current = current->next;
        current1 = current1->next;
    }
    
    while (current2 != head2) {
        current->next = create_node(current2->data);
        current = current->next;
        current2 = current2->next;
    }
    
    current->next = head;
    return head;
}

// 比较两个整数数据的函数
int compare_int(void* a, void* b) {
    return (*(int*)a) - (*(int*)b);
}

// 打印整数数据的函数
void print_int(void* data) {
    printf("%d -> ", *(int*)data);
}

// 测试代码
int main() {
    Node* list1 = init_list();
    Node* list2 = init_list();

    int data1[] = {3, 1, 4, 2, 5};
    int data2[] = {10, 20, 30, 40, 50};
    
    for (int i = 0; i < 5; i++) {
        append_node(list1, &data1[i]);
        append_node(list2, &data2[i]);
    }

    printf("List 1: ");
    print_list(list1, print_int);
    printf("List 2: ");
    print_list(list2, print_int);

    delete_head(list1);
    printf("List 1 after deleting head: ");
    print_list(list1, print_int);

    delete_tail(list1);
    printf("List 1 after deleting tail: ");
    print_list(list1, print_int);

    swap_nodes(list1, 2, 3);
    printf("List 1 after swapping nodes 2 and 3: ");
    print_list(list1, print_int);

    ascending_list(list1, compare_int);
    printf("List 1 after sorting in ascending order: ");
    print_list(list1, print_int);

    reverse_list(list1);
    printf("List 1 after reversing: ");
    print_list(list1, print_int);

    printf("Traversing List 1: ");
    traverse_list(list1, print_node);
    printf("HEAD\n");

    Node* merged_list = merge_lists(list1, list2);
    printf("Merged List: ");
    print_list(merged_list, print_int);

    destroy_list(list1);
    destroy_list(list2);
    destroy_list(merged_list);

    return 0;
}