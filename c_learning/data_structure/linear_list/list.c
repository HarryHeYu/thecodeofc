/************************** 
 * <name：链表
 * <author：He YU Hao
 * <date: 2025.3.4
 * ************************/
#include<stdio.h>
#include<stdlib.h>
/*直接使用 list 类型：当你需要定义一个具体的节点实例。
使用 list* 类型（指针）：当你需要操作链表、传递函数参数、访问和修改节点、或动态分配内存时。
list 是一个结构体类型的别名，用于定义链表节点的结构。
slist 是一个指针类型的别名，指向 list 结构体，用于方便地操作链表节点。
具体来说：
list 定义了一个链表节点的结构。
slist 则是一个指向链表节点的指针类型，简化了链表操作时指针的使用。
你完全可以只使用 list* 来定义指向链表节点的指针，而不用 slist。使用 slist 只是为了代码更加简洁和易读。*/
typedef struct{
    void* data;
}node;
typedef struct LIST{
    node data;
    struct LIST *next;
}ListNode;
typedef struct {
    ListNode *head;
    int length;
}List;
/*直接使用 list 类型：当你需要定义一个具体的节点实例。
使用 list* 类型（指针）：当你需要操作链表、传递函数参数、访问和修改节点、或动态分配内存时。
使用 slist 只是为了简化指针类型的书写，并且在某些项目中可能会看到这种命名约定，以提高代码的可读性和一致性。
你完全可以只使用 list* 来定义指向链表节点的指针，而不用 slist。使用 slist 只是为了代码更加简洁和易读。*/
typedef enum {ERROR = 0, OK = 1}Status;

//初始化链表
Status InitList(List* list){
    list->head = (ListNode*)malloc(sizeof(ListNode));
    if(!list->head)return ERROR;
    list->head->next = NULL;
    list->length = 0;
    return OK;
}
//创建一个链表
List* CreateNewList(){
    List* list = (List*)malloc(sizeof(List));
    if(list == NULL)return NULL;
    if(InitList(list) == ERROR){
        free(list);
        return NULL;
    }
    return list;
}
//创建一个节点
ListNode* CreateNode(void* data){
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    if(newNode == NULL)return NULL;
    newNode->data.data = data;
    newNode->next = NULL;
    return newNode;
}
//删除指定节点
Status DeleteNode(List* list ,ListNode *node){
    if(list->head->next == NULL || node == NULL)return ERROR;
    ListNode* p = list->head;
    while(p->next != NULL && p->next != node)p = p->next;//循环来寻找指定节点 node 的前一个节点。
    if(p->next == NULL)return ERROR;//若为NULL则意味着没有找到node
    p->next = node->next;
    free(node->data.data);
    free(node);
    list->length--;
    return OK;
}
//逆置链表
void InvertedList(List* list){
    if(list->head->next == NULL){
        printf("链表为空无法实现逆置操作\n");
        return;
    }
    ListNode *prev = NULL, *current = list->head->next, *next = NULL;
    //current 指针初始化为 list->head->next，即链表的第一个有效节点，用于遍历链表。
    while(current != NULL){
        next = current->next;//将当前节点的下一个节点暂存到 next 指针中，防止在反转指针时丢失后续节点的引用。
        current->next = prev;//将当前节点的 next 指针指向前一个节点，实现节点的反转。
        prev = current;//将 prev 指针移动到当前节点，为下一次反转做准备。
        current = next;//将 current 指针移动到暂存的下一个节点，继续遍历链表。
    }
    list->head->next = prev;
    //自己画图来理解
    printf("逆置成功！\n");
} 
//尾插法
void InsertListTail(List* list, void* data){
    ListNode *newNode = CreateNode(data);
    if(newNode == NULL)return;
    ListNode* p = list->head;
    while(p->next != NULL)p = p->next;
    p->next = newNode;
    list->length++;
}
//头插法
void InsertListHead(List* list, void* data){
    ListNode* newNode = CreateNode(data);
    if(newNode == NULL)return;
    newNode->next = list->head->next;
    list->head->next = newNode;
    list->length++;
}
//交换第i个节点和第j个节点
Status SwapListNode(List* list, int i, int j){
    if(i == j || i < 1 || i > list->length || j > list->length){
        printf("所要交换的两个节点有误!\n");
        return ERROR;
    }
    if(i > j){//确保i小于j
        int temp = i; 
        i = j;
        j = temp;
    }
    ListNode* pre_i = list->head;//得到原来的第i个节点的前一个节点
    for(int k = 1; k < i; k++)pre_i = pre_i->next;
    ListNode* pre_j = list->head;//得到原来的第j个节点的前一个节点
    for(int k = 1; k < j; k++)pre_j = pre_j->next;
    ListNode* node_i = pre_i->next;//第i个节点
    ListNode* node_j = pre_j->next;//第j个节点
    if(pre_i == node_j || pre_j == node_i){//相邻节点直接交换
        node_i->next = node_j->next;
        node_j->next = node_i;
        pre_i->next = node_j;//画图即可理解
    }else{
        ListNode* temp = node_j->next;//保存第 j 个节点的下一个节点。
        pre_i->next = node_j;
        node_j->next = node_i->next;
        pre_j->next = node_i;
        node_i->next = temp;//同样画图即可
    }
    return OK;
}
/*cmp 函数指针的作用
cmp 函数指针用于指定一个比较函数，该比较函数会在链表的升序排序过程中，对链表节点的数据进行比较。
由于链表节点的数据类型是 void*，这意味着链表可以存储任意类型的数据，所以不能直接使用固定的比较方式，而是通过传入的比较函数来确定数据之间的大小关系。
cmp 与什么进行比较:
cmp 函数会对链表中相邻两个节点的数据进行比较。具体来说，在链表排序时，会依次取出相邻的两个节点，
将它们的数据作为参数传递给 cmp 函数，通过 cmp 函数的返回值来判断这两个数据的大小关系，进而决定是否需要交换这两个节点的数据位置。
cmp 函数的使用方式:
cmp 函数的原型为 int (*cmp)(void *, void *)，它接收两个 void* 类型的参数，返回一个整数值，该返回值有以下含义：
返回值小于 0：表示第一个参数所指向的数据小于第二个参数所指向的数据。
返回值等于 0：表示第一个参数所指向的数据等于第二个参数所指向的数据。
返回值大于 0：表示第一个参数所指向的数据大于第二个参数所指向的数据。*/
//升序排列链表
// Status AscendingList(List* list, int (*cmp)(void*, void*)){
//     if(list->head->next == NULL || list->head->next->next == NULL)return OK;
//     ListNode *p, *q;
//     for(int i = 1; i < list->length; i++){
//         p = list->head->next;
//         for(int j = 1; j < list->length; j++){
//             q = p->next;
//             if(q && cmp(p->data.data, q->data.data) > 0){
//                 void* temp = p->data.data;
//                 p->data.data = q->data.data;
//                 q->data.data = temp;
//             }
//             p = p->next;
//         }
//     }
//     return OK;
// }
//此代码时间复杂度更高，虽然改完后的代码的时间复杂度也是O(n^2)，但是实际要快点
//该代码相比上面是在内层函数里少了一点，是让j直接从i处开始遍历
Status AscendingList(List* list, int (*cmp)(void*, void*)){
    if(list->head->next == NULL || list->head->next->next == NULL)return OK;
    ListNode *p, *q;
    for(int i = 1; i < list->length; i++){
        p = list->head->next;
        for(int k = 1; k < i; k++)p = p->next;//移动p的指针到第i个节点
        for(int j = i; j < list->length; j++){
            q = p->next;
            if(q && cmp(p->data.data, q->data.data) > 0){//此处q作为一个判断条件是指q!=NULL
                void* temp = p->data.data;
                p->data.data = q->data.data;
                q->data.data = p->data.data;
            }
            p = p->next;
        }
    }
    return OK;
}
//合并链表
Status AddList(List* la, List* lb, List* lc, int (*cmp)(void*, void*)){//int (*add)(void*, void*)
    ListNode* p = la->head->next;
    ListNode* q = lb->head->next;
    while(p != NULL && q != NULL){
        if(cmp(p->data.data, q->data.data) < 0){
            InsertListTail(lc, p->data.data);
            p = p->next;
        }else if(cmp(p->data.data, q->data.data) > 0){
            InsertListTail(lc, q->data.data);
            q = q->next;
        }else{
            // void* e = add(p->data.data, q->data.data);
            InsertListTail(lc, p->data.data);
            InsertListTail(lc, q->data.data);
            p = p->next;
            q = q->next;
        }
    }
    while(p != NULL){
        InsertListTail(lc, p->data.data);
        p = p->next;
    }
    while(q != NULL){
        InsertListTail(lc, q->data.data);
        q = q->next;
    }
    return OK;
}
//删除第i个节点
Status DeleteNodeByIndex(List* list, int i){
    if(i < 1 || i > list->length)return ERROR;
    ListNode *p = list->head;
    for(int j = 1; j < i; j++)p = p->next;//此时p为要删的节点的前一个节点
    ListNode* q = p->next;
    p->next = q->next;
    DeleteNode(list,q);
    return OK;
}
//删除头结点后的第一个节点
Status DeleteHeadNode(List* list){
    if(list->head->next == NULL)return ERROR;
    ListNode* node = list->head->next;
    list->head->next = node->next;
    DeleteNode(list, node);
    return OK;
}
//删除尾节点
Status DeleteTailNode(List* list){
    if(list->head->next == NULL)return ERROR;
    ListNode *p = list->head;
    while(p->next != NULL && p->next->next != NULL)p = p->next;//最后p是倒数第二个点
    ListNode *node = p->next;
    p->next = NULL;
    DeleteNode(list, node);
    return OK;
}
//查找值为num的节点
ListNode* FindListNode(List* list, void* num, int (*cmp)(void*,void*)){
    ListNode *p = list->head->next;
    while(p != NULL && cmp(p->data.data, num) != 0)p = p->next;
    return p;
}
//获取链表长度
int GetListLength(List* list){
    return list->length;
}
//判断链表是否为空
Status IsListEmpty(List* list){
    return list->head->next == NULL ? OK : ERROR;
}
//遍历并操作链表中的所有节点
Status traverseList(List *list, Status(*visit)(void*)){
    if(list->head->next == NULL){
        printf("链表为空或不存在\n");
        return ERROR;
    }
    ListNode* p = list->head->next;
    while(p != NULL){
        if(visit(p->data.data) != OK)return ERROR;
        p = p->next;
    }
    return OK;
}
//访问函数，用于打印节点信息
Status visit(void* e){
    int *p = (int*)e;
    printf("num = %d\n",*p);
    return OK;
}
//销毁链表
void destroyList(List* list){
    ListNode *p = list->head;
    while(p != NULL){
        ListNode *q = p;
        p = p->next;
        free(q);//这里不能使用deleteNode，会造成重复释放，会致使无法退出程序
    }
    free(list);
}
//比较函数（用于排序和查找）
int compareInt(void* a, void *b){
    int *pa = (int*)a;
    int *pb = (int*)b;
    return (*pa - *pb);
}
//加法函数(用于合并)//已经不用了，只是留下来看看而已
void* add(void* a, void* b){
    int *pa = (int*)a;
    int *pb = (int*)b;
    int *result = (int*)malloc(sizeof(int));
    *result = *pa + *pb;
/*使用 int* 指针是因为在链表中存储的是指向数据的指针，而不是直接存储数据本身。
具体而言，当我们使用 void* 类型的指针时，它可以指向任何类型的数据。在 compareInt 函数中，
我们将 void* 类型的指针转换为 int* 类型的指针，以便能够访问和比较指向的整数值。*/
    return result;
}
//测试函数
int main(){
    List *l1 = CreateNewList();
    List *l2 = CreateNewList();
    List *l3 = CreateNewList();
    int n1 = 1, n2 = 2, n3 = 3, n4 = 4, n5 = 5;

    InsertListTail(l1, &n1);
    InsertListTail(l1, &n3);
    InsertListTail(l1, &n5);
    InsertListTail(l2, &n2);
    InsertListTail(l2, &n4);

    printf("链表1中的元素：\n");
    traverseList(l1, visit);
    printf("\n");
    printf("链表2中的元素：\n");
    traverseList(l2, visit);
    printf("\n");

    AddList(l1, l2, l3, compareInt);
    printf("合并后的链表3中的元素：\n");
    traverseList(l3, visit);
    printf("\n");

    SwapListNode(l3, 1, 3);
    printf("交换第1和第3个节点后的链表3中的元素：\n");
    traverseList(l3, visit);
    printf("\n");

    AscendingList(l3, compareInt);
    printf("升序排列后的链表3中的元素：\n");
    traverseList(l3, visit);
    printf("\n");

    DeleteNodeByIndex(l3, 2);
    printf("删除第2个结点后的链表3中的元素：\n");
    traverseList(l3, visit);
    printf("\n");

    DeleteHeadNode(l3);
    printf("删除头结点后的链表3中的元素：\n");
    traverseList(l3, visit);
    printf("\n");

    DeleteTailNode(l3);
    printf("删除尾节点后的链表3中的元素：\n");
    traverseList(l3, visit);
    printf("\n");

    ListNode *found = FindListNode(l3, &n4, compareInt);
    if (found) {
        printf("找到的结点值为：%d\n", *(int *)found->data.data);
    } else {
        printf("未找到值为4的结点\n");
    }

    destroyList(l1);
    destroyList(l2);
    destroyList(l3);

    return 0;
}