/***************************
 *  <name:双向链表
 *  <author: He YU Hao
 *  <date:2025.3.14
*****************************/
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>


#define DOUBLELIST_TEST

typedef struct Node{
    struct Node* prev;
    struct Node* next;
    int data;
}Node, *NodePtr;
/*这里的 DoubleNodeHandle_t 就是一个句柄类型，它代表了双向链表中的一个节点。通过这个句柄，你可以对节点进行各种操作，比如访问节点的数据、修改节点的前后指针等。
使用句柄的好处
封装性：使用句柄可以将节点的具体实现细节封装起来，调用者只需要通过句柄来操作节点，而不需要关心节点内部的结构和实现方式。例如，在操作节点时，你只需要使用 DoubleNodeHandle_t 类型的变量，而不需要了解 DoubleNode_t 结构体的具体成员。
安全性：句柄可以控制对资源的访问权限，避免直接访问资源可能带来的错误。比如，通过句柄函数来操作节点，而不是直接修改节点的指针，这样可以减少因误操作导致的链表结构损坏。
可维护性：如果节点的实现发生变化，只需要修改与节点相关的代码，而不需要修改使用句柄的代码。因为调用者只依赖于句柄，而不依赖于节点的具体实现。*/

typedef struct List{
    NodePtr head;
    NodePtr tail;
}List, *ListPtr;

typedef enum {ERROR = 0, OK = 1}Status;

//初始化链表
Status listInit(ListPtr list) {
    if (list == NULL) return ERROR;
    list->head = NULL;
    list->tail = NULL;
    return OK;
}
/*在双向链表的实现中，你的初始化函数 listInit 不需要分配内存给链表的头节点和尾节点。相反，你只是将头节点和尾节点的指针设置为 NULL，表示链表为空。
在这里，listInit 函数只是设置链表的头节点和尾节点的指针为 NULL，
而不需要分配额外的内存。这是因为在双向链表的实现中，头节点和尾节点的指针只是指向实际存在的节点，而不是一个独立的头节点。
在单链表的实现中，你的初始化函数 InitList 需要分配内存给链表的头节点。这是因为单链表的头节点通常是一个实际存在的节点（即使它不存储数据），
用来表示链表的开始。这个头节点的存在使得链表的操作（如插入和删除）更加简单，因为你不需要处理特殊情况（如链表为空时的操作）。

单链表：
在单链表中，头节点通常是一个实际存在的节点（即使它不存储数据），用来表示链表的开始。
这使得链表的操作更加简单，因为你不需要处理特殊情况。例如，当链表为空时，你可以直接从头节点开始操作，而不需要检查链表是否为空。
双向链表
：在双向链表中，头节点和尾节点的指针只是指向实际存在的节点，而不是一个独立的头节点。
这使得链表的操作更加灵活，因为你可以直接操作头节点和尾节点的指针，而不需要处理特殊情况。*/

//返回链表头结点
NodePtr listGetHead(ListPtr list) {
    return list->head;
}

//返回链表尾节点
NodePtr listGetTail(ListPtr list) {
    return list->tail;
}

//返回节点的前驱节点
NodePtr listGetPrev(NodePtr node) {
    return node->prev;
}

//返回节点的后继节点
NodePtr listGetNext(NodePtr node) {
    return node->next;
}

//创建一个新的双向链表
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

//创建一个新的双向链表节点
NodePtr nodeCreate(int data){
    NodePtr node = (NodePtr)malloc(sizeof(Node));
    if(node == NULL)return NULL;
    node->prev = NULL;
    node->next = NULL;
    node->data = data;
    return node;
}

/*通常不会有一个专门的“头结点”作为哨兵节点。头结点指的是链表的第一个实际存储数据的节点。
在插入第一个节点到空链表时，新的节点既是头节点也是尾节点。*/

//在链表的前端插入一个节点
Status listPushFront(ListPtr list, NodePtr node){
    if(list == NULL || node == NULL)return ERROR;
    node->prev = NULL;
    node->next = list->head;// 设置新节点的后继指针为当前的头节点
    // 设置新节点的后继指针为当前的头节点
    if(list->head){
        list->head->prev = node;
    }else{
        list->tail = node;
    }
    list->head = node;
    return OK;
}

//在连表的末端插入一个节点
Status listPushBack(ListPtr list, NodePtr node){
    if(list == NULL || node == NULL)return ERROR;
    node->next = NULL;
    node->prev = list->tail;
    if(list->tail){
        list->tail->next = node;
    }else{
        list->head = node;
    }
    list->tail = node;
    return OK;
}

/*在before之前插入节点，如果before为NULL，则插入到链表末尾
这个插入链表末尾是默认的，实际上我也可以将他插入链表前端
    *@param before 指向要插入位置之前的节点的指针*/
Status listInsertBefore(ListPtr list, NodePtr before, NodePtr node){
    if(list == NULL || node == NULL)return ERROR;
    if(before){
        if(before->prev){
            before->prev->next = node;
        }else{
            list->head = node;
        }
        node->next = before;
        node->prev = before->prev;
        before->prev = node;
    }else{
        return listPushBack(list, node);//也可以使用listPushFront
    }
    return OK;
}

/*在after之后插入节点，如果after为NULL，则插入到链表前端
    *@param after 指向要插入位置之后的的节点的指针*/
Status listInsertAfter(ListPtr list, NodePtr after, NodePtr node){
    if(list == NULL || node == NULL)return ERROR;
    if(after){
        if(after->next){
            after->next->prev = node;
        }else{
            list->tail = node;
        }
        node->prev = after;
        node->next = after->next;
        after->next = node;
    }else{
        return listPushFront(list, node);
/*return listPushFront(list, node);
这一行代码的意思是直接调用 listPushFront 函数，并将其返回值作为 listInsertAfter 函数的返回值。
这种方式在一个函数调用另一个函数的时候比较简洁，可以直接返回被调用函数的返回值。

listPushFront(list, node); return OK;
这种方式是先调用 listPushFront 函数，然后无论 listPushFront 的返回值是什么，都返回 OK。这种方式在你不关心被调用函数的返回值时使用。*/
    }
    return OK;
}

//从链表的前端移除并返回节点
NodePtr listPopFront(ListPtr list){
    if(list == NULL || list->head == NULL)return NULL;
    NodePtr node = list->head;
    if(list->head->next){
        list->head->next->prev = NULL;
    }else{
        list->tail = NULL;
    }
    list->head = list->head->next;
    node->prev = NULL; 
	node->next = NULL;
    return node;
}

//从链表的末端移除并返回节点
NodePtr listPopBack(ListPtr list){
    if(list == NULL || list->tail == NULL)return NULL;
    NodePtr node = list->tail;
    if(list->tail->prev){
        list->tail->prev->next = NULL;
    }else{
        list->head = NULL;
    }
    list->tail = list->tail->prev;
    node->prev = NULL; 
	node->next = NULL;
    return node;
}

//从链表中移除指定节点
Status listRemove(ListPtr list, NodePtr node){
    if(list == NULL || node == NULL)return ERROR;
    if(node->prev){
        node->prev->next = node->next;
    }else{
        list->head = node->next;
    }
    if(node->next){
        node->next->prev = node->prev;
    }else{
        list->tail = node->prev;
    }
    node->prev = NULL;
    node->next = NULL;
    return OK;
}

//检查链表是否为空
bool listIsEmpty(ListPtr list){
    if(list == NULL)return true;
    if(list->head == NULL && list->tail == NULL)return true;
    return false;
}

//获取链表的长度
/*非负性：链表长度不可能是负数，因此使用无符号类型 unsigned 能更好地表达这一点。
范围：unsigned long 提供更大的范围，能够表示更大的数值，适用于可能包含大量节点的链表。 unsigned long 在大多数系统中能够表示的范围是 0 到 4,294,967,295（32位系统）或更大（64位系统）。
效率：在某些体系结构中，无符号整数的运算可能比有符号整数更高效。*/
unsigned long listGetSize(ListPtr list){
    if(list == NULL)return 0;
    unsigned long size = 0;
    NodePtr node = listGetHead(list);//直接使用NodePtr node = list->head;也可以
    while(node){
        ++size;
        node = node->next;
    }
    return size;
}

/*// 切分链表，将链表分成两个子链表
void splitList(NodePtr source, NodePtr* frontRef, NodePtr* backRef) {
    NodePtr fast;
    NodePtr slow;
    slow = source;
    fast = source->next;

    // 快慢指针法找到中点
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    // 切分链表
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}

// 合并两个已排序的子链表
NodePtr sortedMerge(NodePtr a, NodePtr b) {
    NodePtr result = NULL;

    // 基本情况
    if (a == NULL)
        return b;
    else if (b == NULL)
        return a;

    // 选择a或b，并递归合并
    if (a->data <= b->data) {
        result = a;
        result->next = sortedMerge(a->next, b);
        result->next->prev = result;
        result->prev = NULL;
    } else {
        result = b;
        result->next = sortedMerge(a, b->next);
        result->next->prev = result;
        result->prev = NULL;
    }
    return result;
}

// 合并排序链表
void mergeSort(NodePtr* headRef) {
    NodePtr head = *headRef;
    NodePtr a;
    NodePtr b;

    // 基本情况
    if ((head == NULL) || (head->next == NULL)) {
        return;
    }

    // 切分链表
    splitList(head, &a, &b);

    // 递归排序两个子链表
    mergeSort(&a);
    mergeSort(&b);

    // 合并两个已排序的子链表
    *headRef = sortedMerge(a, b);
}

// 对链表进行排序
Status listSort(ListPtr list) {
    if (list == NULL) return ERROR;
    mergeSort(&list->head);

    // 更新尾指针
    NodePtr node = list->head;
    while (node->next != NULL) {
        node = node->next;
    }
    list->tail = node;

    return OK;
}*/

//对链表进行插入排序
Status listInsertionSort(ListPtr list){
    if(list == NULL || list->head == NULL)return ERROR;

    NodePtr sorted = NULL;//用于存储已排序接的链表
    NodePtr current = list->head;//当前节点
    
    while(current != NULL){
        NodePtr next = current->next;
        if(sorted == NULL || sorted->data >= current->data){
            //插入到已排序链表的头部
            current->next = sorted;//将current的next指针指向sorted的头节点
            if(sorted)sorted->prev = current;
            sorted = current;//更新头结点，即current成为新的头结点
            sorted->prev = NULL;//更新头结点的prev指针
        }else{
            //插入到已排序链表的适当位置
            NodePtr temp = sorted;
            while(temp != NULL && temp->next != NULL && temp->next->data < current->data){
                temp = temp->next;
            }
            current->next = temp->next;
            if(temp->next)temp->next->prev = current;
            temp->next = current;
            current->prev = temp;
        }
        current = next;//移动到下一个节点
    }
    //更新链表的头尾指针
    list->head = sorted;
    NodePtr tail = sorted;
    while(tail->next != NULL)tail = tail->next;
    list->tail = tail;
    return OK;
}

//合并两个按数据大小排序的链表
ListPtr  listCombine(ListPtr list1, ListPtr list2){
    if(list1 == NULL || list2 == NULL)return NULL;

    listInsertionSort(list1);
    listInsertionSort(list2);

    ListPtr mergedList = listCreate();
    if(mergedList == NULL)return NULL;

    NodePtr node1 = list1->head;
    NodePtr node2 = list2->head;

    while(node1 != NULL && node2 != NULL){
        if(node1->data <= node2->data){
            NodePtr nextNode = node1->next;
            listPushBack(mergedList, node1);
            node1 = nextNode;
        }else{
            NodePtr nextNode = node2->next;
            listPushBack(mergedList, node2);
            node2 = nextNode;
        }
    }
    while(node1 != NULL){
        NodePtr nextNode = node1->next;
        listPushBack(mergedList, node1);
        node1 = nextNode;
    }
    while(node2 != NULL){
        NodePtr nextNode = node2->next;
        listPushBack(mergedList, node2);
        node2 = nextNode;
    }

    list1->head = list1->tail = NULL;
    list2->head = list2->tail = NULL;
    return mergedList;
}

//清空链表
Status listClear(ListPtr list){
    if(list == NULL)return ERROR;
    NodePtr node = list->head;
    while(node != NULL){
        NodePtr tmp = node;
        node = node->next;
        free(tmp);
    }
    /*或者：
        NodePtr next = node->next;
        free(node);
        node = next;*/
    list->head = list->tail = NULL;
    return OK;
}

//查找链表中数据为data的节点
NodePtr listFind(ListPtr list, int data){
    if(list == NULL)return NULL;
    NodePtr node = list->head;
    while(node != NULL){
        if(node->data == data)return node;
        node = node->next;
    }
    return NULL;
}

//遍历链表并执行给定的函数
Status listTraverse(ListPtr list, void(*func)(NodePtr)){
    if(list == NULL || func == NULL)return ERROR;
    NodePtr node = list->head;
    while(node != NULL){
        func(node);
        node = node->next;
    }
    return OK;
}

//销毁链表并释放内存
Status listDestroy(ListPtr list){
    if(list == NULL)return ERROR;
    NodePtr node = list->head;
    while(node != NULL){
        NodePtr temp = node;
        node = node->next;
        free(temp);
    }
    free(list);
    return OK;
}

#if defined DOUBLELIST_TEST
//打印节点信息
void printNode(NodePtr node){
    printf("Node data: %d\n", node->data);
}

// 测试函数
void testList() {
    ListPtr list = listCreate();
    if (list == NULL) {
        printf("Failed to create double list.\n");
        exit(1);
    }
    NodePtr node1 = nodeCreate(1);
    if (node1 == NULL) {
        printf("Failed to create node1.\n");
        listDestroy(list);
        exit(1);
    }
    NodePtr node2 = nodeCreate(2);
    if (node2 == NULL) {
        printf("Failed to create node2.\n");
        free(node1);
        listDestroy(list);
        exit(1);
    }

    if (listPushFront(list, node1) == ERROR) {
        printf("Failed to push node1 to front.\n");
        free(node1);
        free(node2);
        listDestroy(list);
        exit(1);
    }

    if (listPushBack(list, node2) == ERROR) {
        printf("Failed to push node2 to back.\n");
        free(node1);
        free(node2);
        listDestroy(list);
        exit(1);
    }

    printf("List size after insertions: %lu\n", listGetSize(list));

    listTraverse(list, printNode);

    NodePtr temp = listPopFront(list);
    if (temp != NULL) {
        printf("Popped from front: %d\n", temp->data);
        free(temp);
    }

    temp = listPopBack(list);
    if (temp != NULL) {
        printf("Popped from back: %d\n", temp->data);
        free(temp);
    }

    printf("List size after popping: %lu\n", listGetSize(list));

    listDestroy(list);
    exit(0);
}

int main() {
    testList();
    return 0;
}

#endif