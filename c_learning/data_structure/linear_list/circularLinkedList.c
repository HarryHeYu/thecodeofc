/***************************
 *  <name:循环链表
 *  <author: He YU Hao
 *  <date:2025.3.6
*****************************/
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
typedef enum {ERROR = 0, OK = 1}Status;
typedef struct Node{
    int data;
    struct Node* next;
}Node;
//创建一个新节点
Node* createNode(int data){
    Node *newNode = (Node*)malloc(sizeof(Node));
    if(!newNode){//其实newNode == NULL 这种写法会更具有可读性，当然两种写法都是正确的
        printf("Memory allocation failed!\n");
        exit(1);//是完全退出程序，而return只是推出当前函数
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}
//只有在极其严重的情况下才会选择exit(1)，不然还是只需要推出当前函数(也就是使用return)
//但是无论是exit(1)还是exit(0)都是直接退出程序，只不过exit(0)是代表正常退出

//初始化链表（创建头结点）
Node* initList(){
    Node* head = createNode(0);//创建头结点
    //头结点通常不存储有效数据，仅作为链表的入口点。此处将data设为 0 是为了初始化时的默认值，避免未初始化的垃圾值。
/*"垃圾值" 是指在内存中未初始化的变量或未赋值的内存区域中的数据。这些数据是随机的、不可预测的，并且可能会导致程序行为异常。*/
    head->next = head;//头结点指向自己，形成循环
    return head;
}//这相当于就是创建一个新链表了
//创建一个新链表
Node* createList(){
    return initList();
}
//销毁链表
void destroyList(Node* head){
    Node* current = head->next;
    Node* next;// 这个就相当于是一个temp
    while(current != head){
        next = current->next;
        free(current);
        current = next;
    }
    free(head);
}
//判断链表是否为空
bool isEmpty(Node* head){
    return head->next == head;//若为空，则head的下一个节点就是自己(就是说只有头结点在链表里)
}
//获取链表长度
int getLength(Node* head){
    int length = 0;
    Node* current = head->next;
    while(current != head){
        length++;
        current = current->next;
    }
    return length;
}
//打印链表
void printList(Node* head) {
    Node* current = head->next;
    if (current == head) {
        printf("Empty list\n");
        return;
    }
    while (current->next != head) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("%d\n", current->data); // 打印最后一个节点
}
//插入节点到链表头部
Status insertHead(Node* head, int data){
    Node* newNode = createNode(data);
    newNode->next = head->next;
    head->next = newNode;
    return OK;
}
//插入节点到链表尾部
Status appendNode(Node* head, int data){
    Node* newNode = createNode(data);
    Node* current = head;
/*若将current设置为head->next:
空链表时：head->next == head，此时 current 是 head，循环条件 current->next != head 不成立，直接执行插入操作，导致 newNode 插入到 head 之后，但此时链表为空，插入逻辑正确。
非空链表时：需要额外处理 current 为 NULL 的情况（例如链表只有头结点），增加代码复杂度。*/
    while(current->next != head)current = current->next;
    //最后current为尾节点
    newNode->next = head;
    current->next = newNode;
    return OK;
}
//删除指定节点
Status deleteNode(Node* head, Node* node){
    Node* current = head;
    while(current->next != head && current->next != node)current = current->next;
//设置current->next != head是为了保证在遍历循环链表时不会陷入无限循环
//同时还检查了避免链表不为空链表
    if(current->next == node){//为了确保在遍历链表后确实找到了待删除的节点，并处理节点不存在的情况。
        current->next = node->next;
        free(node);
        return OK;
    }
    return ERROR;
}
//按条件删除节点
Status delete_if(Node* head, bool(*condition)(Node*)){
    Node* current = head;
    while(current->next != head){
        if(condition(current->next)){
            Node* temp = current->next;
            current->next = temp->next;
            free(temp);
        }else{
            current = current->next;
        }
    }
    return OK;
}
//删除头结点后的第一个节点
Status deleteHead(Node* head){
    if(isEmpty(head))return ERROR;
    Node* temp = head->next;
    head->next = temp->next;
    free(temp);
    return OK;
}
//删除尾节点
Status deleteTail(Node* head){
    if(isEmpty(head))return ERROR;
    Node* current = head;
    while(current->next->next != head)current = current->next;
    Node* temp = current->next;
    current->next = head;
    free(temp);
    return OK;
}
//交换两个节点
Status swapNodes(Node* head, int i, int j){
    if(i == j)return OK;
    if(i > j){//保证i一直小于等于j
        int temp = i;
        i = j;
        j = temp;
    }
    Node* pre_i = head;
    for(int k = 1; k < i; k++)pre_i = pre_i->next;
    Node* pre_j = head;
    for(int k = 1; k < j; k++)pre_j = pre_j->next;
    Node* node_i = pre_i->next;
    Node* node_j = pre_j->next;
    if(pre_i == node_j || pre_j == node_i){//判断两点是否临近
        if (pre_j == node_i) { // node_i 是 node_j 的前驱
            pre_i->next = node_j;
            node_i->next = node_j->next;
            node_j->next = node_i;
        } else { // node_j 是 node_i 的前驱
            pre_j->next = node_i;
            node_j->next = node_i->next;
            node_i->next = node_j;
        }
    }else{
        Node* temp = node_j->next;
        pre_i->next = node_j;
        node_j->next = node_i->next;
        pre_j->next = node_i;
        node_i->next = temp;
    }
    return OK;
}
//升序排列链表
Status ascendingList(Node* head){
    if(isEmpty(head) || head->next->next == head)return OK;
    //head->next->next == head: 这是一个直接检查链表中是否只有一个有效节点的条件。
    Node* p = head->next;
    while(p->next != head){
        Node* q = p->next;
        while(q != head){
            if(p->data > q->data){
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
//反转链表
Status reverseList(Node* head){
    if(isEmpty(head) || head->next->next == head)return OK;
    //无节点和一个节点的时候都不需要载进行翻转了
    Node* prev = head;
    Node* current = head->next;
    Node* next;
    while(current != head){
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }//此时结束循环后，链表并不是循环的是:last->last+1->....->3->2->1->head
    // Complete the circular link after reversing the list
    head->next->next = head; // 使原链表的第一个节点指向 head
// 这行代码的作用是确保反转后的新头节点的 next 指针指向链表的头节点，使链表保持循环特性。
//再反转过程中，原第一个数据点的next已经被设置为了head，然而，头结点head的next仍然指向
//原第一个数据点，这导致反转后的链表结构不完整（也就是单向的）
//该行代码分作用是显示地确保原第一个数据点的next指向head，尽管在循环中已经设置过，
//这行代码可以防止某些边界情况下的错误（如中间步骤未修改正确·）
//head->next->next=head增强了代码的鲁棒性

    head->next = prev;// 直接将head的next指向反转后的新头节点前驱
    return OK;
}
//遍历链表
Status traverseList(Node* head, Status(*visit)(Node*)){
    Node* current = head->next;
    while(current != head){
        if(visit(current) != OK)return ERROR;
        current = current->next;
    }
    return OK;
}
// 打印节点数据
Status printNode(Node* node) {
    printf("%d -> ", node->data);
    return OK;
}
//插入节点到指定位置
//需要找到插入位置的前一个节点，然后调整指针将新节点插入到该位置。
Status insertNode(Node* head, int data, int position){
    if(position < 1 || position > getLength(head) + 1){
        printf("Invalid position!\n");
        return ERROR;
    }
    Node* newNode = createNode(data);
    Node* current = head;
/*在链表中插入节点时，需要将新节点插入到指定位置的前一个节点之后。
将 current 初始化为 head，可以从链表的头节点开始遍历，
通过 for 循环 position - 1 次，current 最终会指向插入位置的前一个节点。*/
    for(int i = 1; i < position; i++)current = current->next;
    newNode->next = current->next;
    current->next = newNode;
    return OK;
}
// 查找节点
Node* findNode(Node* head, int data){
    Node* current = head->next;
/*头节点通常不存储实际的数据，只是作为链表的一个起始标志。
在查找数据时，不需要从 head 开始，而是从第一个有效节点（即 head->next）开始遍历，
这样可以提高查找效率，避免对无用的头节点进行不必要的比较。*/
    while(current != head && current->data != data)current = current->next;
    if(current == head)return NULL;
    return current;
}
//获取前驱节点
Node* getPreNode(Node* head, Node* node){
    Node* current = head;
/*头结点不存储有效数据，但其 next 指向第一个有效节点。
通过头结点作为起点遍历链表，确保能覆盖所有有效节点。*/
    while(current->next != head && current->next != node)current = current->next;
    //current->next != head避免无线循环
    //current->next != node说明current的下一个节点就是node的位置
    if(current->next == head)return NULL;
    return current;
}
//获取后继节点
Node* getNextNode(Node* node){
    return node->next;
}
//清空链表
Status clearList(Node* head){
    Node* current = head->next;
    Node* next;
    while(current != head){
        next = current->next;
        free(current);
        current = next;
    }
    head->next = head;
    return OK;
}
//获取节点所在的位置
int getNodePosition(Node* head, Node* node){
    int pos = 1;
    Node* current = head->next;
    while(current != head && current != node){
        current = current->next;
        pos++;
    }
    if(current == head)return -1;
    return pos;
}
//合并两个链表
Node* mergeLists(Node* head1, Node* head2){
    Node* head = initList();
    Node* current = head;
    //设置出新的连表
    Node* current1 = head1->next;
    Node* current2 = head2->next;
    while(current1 != head1 && current2 != head2){
        if(current1->data <= current2->data){
            current->next = createNode(current1->data);
            current1 = current1->next;
        }else{
            current->next = createNode(current2->data);
            current2 = current2->next;
        }
        current = current->next;
    }
    while(current1 != head1){
        current->next = createNode(current1->data);
        current1 = current1->next;
        current = current->next;
    }
    while(current2 != head2){
        current->next = createNode(current2->data);
        current2 = current2->next;
        current = current->next;
    }
    current->next = head;
    return head;
}
//测试函数
int main(){
    Node* l1 = createList();
    Node* l2 = createList();
    appendNode(l1, 3);
    appendNode(l1, 1);
    appendNode(l1, 4);
    appendNode(l1, 2);
    appendNode(l1, 5);

    appendNode(l2, 10);
    appendNode(l2, 20);
    appendNode(l2, 30);
    appendNode(l2, 40);
    appendNode(l2, 50);

    printf("List 1: ");
    printList(l1);
    printf("List 2: ");
    printList(l2);

    deleteHead(l1);
    printf("List 1 after deleting head: ");
    printList(l1);

    deleteTail(l1);
    printf("List 1 after deleting tail: ");
    printList(l1);

    swapNodes(l1,2,3);
    printf("List 1 after deleting tail: ");
    printList(l1);

    ascendingList(l1);
    printf("List 1 after swapping nodes 2 and 3: ");
    printList(l1);

    reverseList(l1);
    printf("List 1 after reversing: ");
    printList(l1);

    printf("Traversing List 1: ");
    traverseList(l1, printNode);
    printf("HEAD\n");

    Node* merged_list = mergeLists(l1, l2);
    printf("Merged List: ");
    printList(merged_list);

    destroyList(l1);
    destroyList(l2);
    destroyList(merged_list);

    return 0;
}