//链表
/*链表中每个节点有两个分量，一个存数据一个指向下一个节点的指针
一般都用void做通用指针*/

//接上回
#include <stdio.h>
#include <stdlib.h>//(malloc,realloc,free在这里面)
#include<stdbool.h>
#define STACK_INIT_SIZE 100
#define STACK_INCREMENT 10
// typedef int StackElem;//不要定，不然就不好用了，但是此处又必须定义，不然报错
typedef enum{ERROR=0,OK=1}Status;
typedef enum{FALSE=0,TRUE=1}Bool;
// typedef struct{
//     StackElem* pTop;
//     StackElem* pBase;//pBase是栈底指针，pTop是栈顶指针.pBase 是一个指针，它指向栈分配的内存的起始位置
//     int size;
// }Stack;

typedef struct ListNode{
    struct ListNode *pFirstNode;/*这里的struct关键字是必须的，因为它告诉编译器ListNode是一个结构体类型，而不是一个已经定义的类型或者变量名。
        如果没有struct关键字，编译器会将ListNode视为一个已经定义的类型或变量名，这会导致编译错误，因为在这个结构体定义的上下文中，ListNode还没有被完全定义。*/
    struct ListNode *pNextNode;//用于把节点串起来
    void* pData;//指向数据的指针
}ListNode;//，第一个ListNode是结构名，最后这一个是类型名//链表节点
/*这是一个递归的定义，在定义ListNode时，又用到了ListNode。在一个节点中有两个分量：pNextNode是指向下一个节点的指针；pData是指向任意数据的通用指针。*/

typedef struct{
    ListNode *pFirstNode;
    int length;
}List;//表
//把表定义出来了，先定义节点，再利用节点定义表


//私有函数前面加了一个static（表示只能在本文件内使用），仅供链表使用   //私有操作
static ListNode* ListMakeNode(void* pData);//创建节点  返回值类型是节点指针，因为需要知道生成的在哪里
static void ListFreeNode(ListNode* pNode, void (*freeData)());//释放节点
//上面两个函数一般用不到，只有开发链表的人才会用到（叫做对象的私有函数：外部用不着）
//把链表单独做成一个文件就相当于是打包了

//Public functions
List* ListConstruct();//初始化函数和构造函数写一个就可以了（前提是在动态内存上,这里就不写了）
void ListDestruct(List* pList, void (*freeData)());//销毁函数（析构？）
Bool ListIsEmpty(List* pList);
int ListLength(List* pList);
void ListClear(List* pList, void (*freeData)());
void* ListGetData(ListNode* pNode);
Status ListAppendToTail(List* Plist,void* pData);//加到尾部
Status ListInsertAtFront(List* pList,void* pData);//加到头部
Status ListDeleteFirstNode(List* pList, void(*freeData)());//删除第一个节点
Status ListDeleteLastNode(List* pList, void(*freeData)());//删除最后一个节点
Status ListTraverse(List* pList, Status (*visit)());

// List* ListConstruct(){
//     List *pList;
//     pList=(List*)malloc(sizeof(List));
//     return pList;
// }

void freeData(void* data) {
    if (data != NULL) {
        free(data);
    }
}

static ListNode* ListMakeNode(void* pData){
    ListNode *pNode;
    pNode=(ListNode*)malloc(sizeof(ListNode));//gcc不需要强制转换，但是有些保守的编译器就需要
    if(pNode==NULL){
        return NULL;
    }//这个if语句不能省略，不然下一个语句可能会出错。  如果后面没有初始化那就可以省了
    pNode->pNextNode=NULL;//刚造出一个节点，没有下个节点，所以下一个节点为空
    pNode->pData=pData;
    return pNode;
}

static void ListFreeNode(ListNode* pNode, void (*freeData)()){//此处选择传入函数是因为不知道会需要释放什么类型的数据
//而选择传入函数则可以根据要释放的数据类型来放入不同的释放函数
    if(pNode==NULL){
        return;
    }//需要先判断pNode是否为空，如果为空则直接返回，否则会出错
    freeData(pNode->pData);//需要先这一步释放数据，因为node是有两个域组成的，只freepNode会造成内存泄漏
    free(pNode);//还有一个问题就是这个Pdata到底是什么数据，他能不能够被释放掉
    //如果是栈的话，就不好说了，如果是复杂的类型可能会释放不了或释放不干净
    //因此把释放数据的操作交给函数来做，因此加一个函数指针：*freeData()（专门用来释放数据）
    return;
}

List* ListConstruct(){
    List *pList;
    pList = (List*)malloc(sizeof(List));////这里需要强制转换，因为malloc返回的是void*类型，而List是结构体类型
    if(pList==NULL){
        return NULL;
    }
    pList->pFirstNode = NULL;
    pList->length = 0;
    return pList;
}

void ListDestruct(List* pList, void (*freeData)()){
    //在freelist前应该先把所有的节点都释放掉
    // free(pList);//Wrong,只free掉了头，后面的数据就找不到了但是数据还在，会造成严重的内存泄漏
    ListClear(pList,freeData);
    free(pList);//先清空再释放掉
    return;
}

void ListClear(List* pList, void (*freeData)()){
    ListNode *pNode,*pNextNode;
    pNode=pList->pFirstNode;
    while(pNode!=NULL){
        pNextNode=pNode->pNextNode;//把pNode的下一个点保存起来
        ListFreeNode(pNode,freeData);//free的不是指针本身，free的是指针指向的那个点
        pNode=pNextNode;
    }
    pList->pFirstNode=NULL;
    pList->length=0;
}

void* ListGetData(ListNode* pNode){
    return pNode->pData;
}

Status ListInsertAtFront(List* pList,void* pData){
    ListNode *pNewNode;
    pNewNode=ListMakeNode(pData);//之前只是声明了一个指针，现在要给他分配内存
    //这里的pNewNode是一个指针，指向一个节点
    if(pNewNode==NULL){
        return ERROR;
    }
    pNewNode->pNextNode=pList->pFirstNode;//先把原来的第一个节点放到新节点的后面
    //将pNewNode设为新的第一个节点（即把原来的第一个节点设为newnode的nextnode）
    pList->pFirstNode=pNewNode;//然后重新将第一个节点改设为newnode
    pList->length++;
    return OK;
}

Bool ListIsEmpty(List* pList){
    return pList->length==0?TRUE:FALSE;//条件运算符，如果成立就选前面那一个
}

Status ListAppendToTail(List* pList,void* pData){
    ListNode *pNewNode,*pTail;
    pNewNode=ListMakeNode(pData);
    if(pNewNode==NULL){
        return ERROR;
    }
    if(ListIsEmpty(pList)){
        pList->pFirstNode=pNewNode;
        pList->length++;
        return OK;
    }
    //非空的话就需要先找到尾节点
    pTail=pList->pFirstNode;
    while(pTail->pNextNode!=NULL){//如何找：就看他的下一个节点是否为空
        pTail=pTail->pNextNode;//找到最后一个节点，就是从第一个节点开始往后确认节点是否存在至最后一个存在的节点，下一个点不存在就是最后一个点
        //就是一直往后移直到最后
    }
    pTail->pNextNode=pNewNode;
    pList->length++;
    return OK;
}

Status ListDeleteFirstNode(List *pList, void(*freeData)()){
    ListNode *pTemp;
    if(pList==NULL){
        return ERROR;
    }
    if(ListIsEmpty(pList)){
        return OK;
    }
    pTemp=pList->pFirstNode;
    pList->pFirstNode=pTemp->pNextNode;
    ListFreeNode(pTemp,freeData);
    pList->length--;
    return OK;
}

Status ListDeleteNode(List* pList, ListNode* pNode, void(*freeData)()){
    ListNode* pTempNode;
    if(pList==NULL)return ERROR;
    if(pNode==NULL)return OK;
    if(pList->length==0)return ERROR;
    pTempNode=pList->pFirstNode;
    if(pTempNode==pNode){
        pList->pFirstNode=pNode->pNextNode;//略过pNode，直接将firstNode设为pNode的nextNode
    }else{
        while(pTempNode!=NULL&& pTempNode->pNextNode!=pNode){//进行遍历，只要没有到尾（pTempNode不等于空，且当前节点的下一个节点还没有到pNode）
            pTempNode=pTempNode->pNextNode;//不停往后走
        }
        if(pTempNode==NULL)return ERROR;//次检查 pTemp 是否为 NULL 是为了确保要删除的节点存在于链表中，避免对不存在的节点进行删除操作，从而保证代码的正确性和稳定性。
        pTempNode->pNextNode=pNode->pNextNode;//循环后的删除节点
        /*... -> pTempNode -> pNode -> pNode->pNextNode -> ...
        执行 pTempNode->pNextNode = pNode->pNextNode; 后：... -> pTempNode -> pNode->pNextNode -> ...
pNode 从链表中被跳过。链表中不再有任何指针引用 pNode，可以安全释放其内存（ListFreeNode(pNode)）
*/
    }
    ListFreeNode(pNode,freeData);
    pList->length--;
    return OK;
}

Status ListDeleteLastNode(List* pList,void(*freeData)()){
    ListNode* p, *q;
    if(pList==NULL)return ERROR;
    if(pList->length==0)return ERROR;
    q=pList->pFirstNode;
    if(q->pNextNode==NULL)return ListDeleteFirstNode(pList,freeData);
    //如果只有一个节点，就直接删除第一个节点
    //如果不是只有一个节点，就要找到倒数第二个节点
    while(q->pNextNode!=NULL){
        p=q;
        q=q->pNextNode;
/*跟随 q：p 用于记录 q 的前一个节点。在 while 循环中，每次 q 移动到下一个节点之前，p 会被赋值为 q，这样 p 就始终指向 q 的前一个节点。
断开连接：当 while 循环结束，q 指向最后一个节点时，p 就指向了倒数第二个节点。
通过执行 p->pNextNode = NULL;，可以将倒数第二个节点的 pNextNode 指针置为 NULL，从而断开与最后一个节点的连接，将最后一个节点从链表中移除。*/
    }
    p->pNextNode=NULL;
    ListFreeNode(q,freeData);
    q=NULL;
    pList->length--;
    return OK;
}

Status InsertAfter(List* pList, ListNode *pNode, void* pData){
    ListNode* pNewNode;
    pNewNode = ListMakeNode(pList);
    if(pNewNode==NULL)return ERROR;
    pNewNode->pNextNode=pNode->pNextNode;
    pNode->pNextNode=pNewNode;
    pList->length++;
    return OK;
}

bool isThisOne(void* pData, void* pKey) {
    // 假设 pData 和 pKey 指向的是整数
    int data = *(int*)pData;
    int key = *(int*)pKey;
    // 检查两个整数是否相等
    return data == key;
}

ListNode* ListSearch(List* pList, void* pKey, bool(*isThisOne)(void*,void*)){
    ListNode* pTempNode;
    pTempNode=pList->pFirstNode;
    while(pTempNode!=NULL){
        if(isThisOne(pTempNode->pData,pKey)==TRUE)return pTempNode;
        pTempNode = pTempNode->pNextNode;
    }
    return NULL;
}

Status Traverse(List* pList, Status(*visit)(void*)) {
//(*visit) 表示 visit 是一个指向函数的指针。
// 用于表示 visit 是一个指向函数的指针，而不是一个普通的函数。没有 * 的话，编译器会认为 visit 是一个函数，而不是一个指向函数的指针。
    ListNode* pTempNode = pList->pFirstNode;
    while (pTempNode != NULL) {
        if (visit(pTempNode->pData) != OK) {
            return ERROR;
        }
        pTempNode = pTempNode->pNextNode;
    }
    return OK;
}