/* Linked List */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define C24_VERSION 202311L
#if __STDC_VERSION__ < C24_VERSION
    #define nullptr NULL
    typedef enum {false=0,true=1}bool;
#endif // __STDC_VERSION__

#define N 10
typedef enum {ERROR=0,OK=1} Status;
typedef struct ListNode {
    struct ListNode *pNext;
    struct ListNode *pPrevious;
    // ASSIGNMENT 2 TO DO: struct ListNode *pPrevious;
    void *pData;
}ListNode;
typedef struct{
    ListNode *pFirst;
    int length;
    int elemSize;
    // ASSIGNMENT 1 TO DO: int elemSize;
}List;

/* List Operations */
ListNode* List_makeNode(void* pData,int elemSize);
void List_freeNode(ListNode* pNode);
void* ListGetData(ListNode* pNode);
List* ListConstruct(int elemSize);
void ListClear(List* pList);
void ListDestruct(List* pList);
int ListLength(List* pList);
bool ListIsEmpty(List* pList);
Status ListInsertAtFront(List* pList,void* pData,int elemSize);
Status ListAppendToEnd(List* pList,void* pData,int elemSize);
Status ListInsertAfter(List* pList,
                       ListNode* pNode,void* pData,int elemSize);
Status ListDeleteFirstNode(List* pList);
Status ListDeleteLastNode(List* pList);
Status ListDeleteNode(List* pList,
                      ListNode* pNode);
ListNode* ListSearch(List* pList,
                     void* pKey,bool (*compare)());
Status ListTraverse(List* pList,Status (*visit)());

/* Application Functions */
Status PrintInteger(int* pInteger);
// void FreeData(int* pDynamicInteger);

/*在连表中加入elementsize后就可以不使用freeData了，因为使用了elementsize后，节点指向的pData区域就是链表创作者构造的了，而不是
链表使用者构造的。那么创作者就可以直接使用free将pData指向的那一块区域给free掉，因为那一块区域是创作者申请下来的*/

bool Compare(int* pData,int* pKey);


ListNode* List_makeNode(void* pData,int elemSize){
    ListNode *pNewNode;
    pNewNode=(ListNode*)malloc(sizeof(ListNode));
    if(pNewNode==nullptr){
        return nullptr;
    }
    else{
        pNewNode->pData = malloc(elemSize);
        if(pNewNode->pData == nullptr){
            free(pNewNode);
            return nullptr;
        }
        memcpy(pNewNode->pData, pData, elemSize);
        pNewNode->pNext = nullptr;
        pNewNode->pPrevious = nullptr;
        return pNewNode;
    }
}

void List_freeNode(ListNode* pNode){
    if(pNode!=nullptr){
        free(pNode->pData);
    }
    free(pNode);
    return;
}

void* ListGetData(ListNode* pNode){
    return pNode->pData;
}

List* ListConstruct(int elemSize){
    List* pList=(List*)malloc(sizeof(List));
    if(pList==nullptr){
        return nullptr;
    }
    pList->pFirst=nullptr;
    pList->length=0;
    pList->elemSize=elemSize;
    return pList;
}

void ListClear(List* pList){
    ListNode *pCurrentNode,*pNext;
    pCurrentNode=pList->pFirst;
    while(pCurrentNode!=nullptr){
        pNext=pCurrentNode->pNext;
        List_freeNode(pCurrentNode);
        pCurrentNode=pNext;
    }
    pList->pFirst=nullptr;
    pList->length=0;
    return;
}

void ListDestruct(List* pList){
    ListClear(pList);
    free(pList);
    return;
}

int ListLength(List* pList){
    return pList->length;
}

bool ListIsEmpty(List* pList){
    if(pList->length==0){
        return true;
    }
    else{
        return false;
    }
}

Status ListInsertAtFront(List* pList, void* pData, int elemSize) {
    ListNode *pNewNode;
    pNewNode = List_makeNode(pData, elemSize);
    if (pNewNode == nullptr) {
        return ERROR;
    }
    pNewNode->pNext = pList->pFirst;
    if (pList->pFirst != nullptr) {
        pList->pFirst->pPrevious = pNewNode;
    }
    pList->pFirst = pNewNode;
    pList->length++;
    return OK;
}

Status ListAppendToEnd(List* pList,void* pData,int elemSize){
    ListNode *pNewNode,*pTail;
    pNewNode=List_makeNode(pData,elemSize);
    if(pNewNode==nullptr){
        return ERROR;
    }
    if(pList->pFirst==nullptr){
        pList->pFirst=pNewNode;
        pNewNode->pPrevious = pList->pFirst;
    }
    else{
        pTail=pList->pFirst;
        while(pTail->pNext!=nullptr){
            pTail=pTail->pNext;
        }
        pTail->pNext=pNewNode;
        pNewNode->pPrevious = pTail;
    }
    pList->length++;
    return OK;
}

Status ListInsertAfter(List* pList,
                        ListNode* pNode,void* pData,int elemSize){
    if (pNode == nullptr) {
        return ERROR;
    }
    ListNode* pNewNode;
    pNewNode=List_makeNode(pData,elemSize);
    if(pNewNode==nullptr){
        return ERROR;
    }
    pNewNode->pNext=pNode->pNext;
    if(pNode->pNext != nullptr)pNode->pNext->pPrevious = pNewNode;
    pNewNode->pPrevious = pNode;
    pNode->pNext=pNewNode;
    pList->length++;
    return OK;
}

Status ListDeleteFirstNode(List* pList){
    ListNode *pTemp;
    if(pList->length==0){
        return ERROR;
    }
    pTemp=pList->pFirst;
    pList->pFirst=pTemp->pNext;
    if(pList->pFirst != nullptr)pTemp->pNext->pPrevious = pList->pFirst;
    List_freeNode(pTemp);
    pTemp=nullptr;
    pList->length--;
    return OK;
}

Status ListDeleteLastNode(List* pList){
    ListNode *p,*q;
    if(pList->length==0){
        return ERROR;
    }
    q=pList->pFirst;
    if(q->pNext==nullptr){
        return ListDeleteFirstNode(pList);
    }
    while(q->pNext!=nullptr){
        p=q;
        q=q->pNext;
    }
    p->pNext=nullptr;
    List_freeNode(q);
    q=nullptr;
    pList->length--;
    return OK;
}

Status ListDeleteNode(List* pList,
                       ListNode* pNode){
    ListNode* pTemp;
    if(pNode==nullptr){
        return OK;
    }
    if(pList->length==0){
        return ERROR;
    }
    pTemp=pList->pFirst;
    if(pTemp==pNode){
        pList->pFirst=pNode->pNext;
    }
    else{
        while(pTemp!=nullptr &&pTemp->pNext!=pNode){
            pTemp=pTemp->pNext;
        }
        if(pTemp==nullptr){
            return ERROR;
        }
        pTemp->pNext=pNode->pNext;
    }
    List_freeNode(pNode);
    pNode=nullptr;
    pList->length--;
    return OK;
}
/*Status ListDeleteNode(List* pList, ListNode* pNode) {
    if (pNode == nullptr || pList->length == 0 || pList->pFirst == nullptr) {
        return ERROR;
    }
    if (pList->pFirst == pNode) {
        return ListDeleteFirstNode(pList);
    }
    ListNode *pTemp = pList->pFirst;
    while (pTemp != nullptr && pTemp->pNext != pNode) {
        pTemp = pTemp->pNext;
    }
    if (pTemp == nullptr) {
        return ERROR;
    }
    pTemp->pNext = pNode->pNext;
    if (pNode->pNext != nullptr) {
        pNode->pNext->pPrevious = pTemp;
    }
    List_freeNode(pNode);
    pList->length--;
    return OK;
}*/

ListNode* ListSearch(List* pList,
                      void* pKey,bool (*compare)()){
    ListNode* pTemp;
    pTemp=pList->pFirst;
    while(pTemp!=nullptr){
        if(compare(pTemp->pData,pKey)==true){
            return pTemp;
        }
        pTemp=pTemp->pNext;
    }
    return nullptr;
}

Status ListTraverse(List* pList,Status (*visit)()){
    ListNode* pTemp;
    pTemp=pList->pFirst;
    while(pTemp!=nullptr){
        if(visit(pTemp->pData)==ERROR){
            return ERROR;
        }
        pTemp=pTemp->pNext;
    }
    return OK;
}

Status PrintInteger(int* pInteger){
    printf("%d\t",*pInteger);
    return OK;
}

// void FreeData(int* pDynamicInteger){
//     free(pDynamicInteger);
// }

bool Compare(int* pData,int* pKey){
    return (*pData==*pKey)?true:false;
}

int main(){
    int i,key;
    int *pInteger;
    ListNode* pNode;
    List* pList;
    int elemSize;
    printf("Please input the size of the element:");
    scanf("%d",&elemSize);
    pList=ListConstruct(elemSize);
    if(pList==nullptr){
        printf("Error.List construction failed.\n");
        return 1;
    }
    // Insert N elements.
    for(i=N-1;i>=0;i--){
        pInteger=(int*)malloc(sizeof(int));
        if(pInteger==nullptr){
            printf("malloc failed.\n");
            return 1;
        }
        *pInteger=i;
        if(ListInsertAtFront(pList,pInteger,elemSize)==ERROR){
            printf("Error.List insertion failed.\n");
            return 1;
        }
    }
    printf("There are %d elements in the list:\n",N);
    ListTraverse(pList,PrintInteger);
    printf("\n");
    key=4;
    printf("\nIs there any element with a value of 4?\n");
    printf("If there is, delete it.\n\n");
    pNode=ListSearch(pList,&key,Compare);
    if(ListDeleteNode(pList,pNode)==ERROR){
        printf("Error.List deletion failed.\n");
        return 1;
    }
    printf("This is the list after deletion:\n");
    ListTraverse(pList,PrintInteger);
    ListDestruct(pList);
    return 0;
}



