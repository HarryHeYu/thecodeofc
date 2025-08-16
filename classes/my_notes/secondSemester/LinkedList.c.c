/* Linked List */
#include<stdio.h>
#include<stdlib.h>
#define C24_VERSION 202311L
#if __STDC_VERSION__ < C24_VERSION
    #define nullptr NULL
    typedef enum {false=0,true=1}bool;
#endif // __STDC_VERSION__

#define N 10
typedef enum {ERROR=0,OK=1} Status;
typedef struct ListNode {
    struct ListNode *pNext;
    // ASSIGNMENT 2 TO DO: struct ListNode *pPrevious;
    void *pData;
}ListNode;
typedef struct{
    ListNode *pFirst;
    int length;
    // ASSIGNMENT 1 TO DO: int elemSize;
}List;

/* List Operations */
ListNode* List_makeNode(void* pData);
void List_freeNode(ListNode* pNode,void (*freeData)());
void* ListGetData(ListNode* pNode);
List* ListConstruct(void);
void ListClear(List* pList,void (*freeData)());
void ListDestruct(List* pList,void (*freeData)());
int ListLength(List* pList);
bool ListIsEmpty(List* pList);
Status ListInsertAtFront(List* pList,void* pData);
Status ListAppendToEnd(List* pList,void* pData);
Status ListInsertAfter(List* pList,
                       ListNode* pNode,void* pData);
Status ListDeleteFirstNode(List* pList,void (*freeData)());
Status ListDeleteLastNode(List* pList,void (*freeData)());
Status ListDeleteNode(List* pList,
                      ListNode* pNode,void (*freeData)());
ListNode* ListSearch(List* pList,
                     void* pKey,bool (*compare)());
Status ListTraverse(List* pList,Status (*visit)());

/* Application Functions */
Status PrintInteger(int* pInteger);
void FreeData(int* pDynamicInteger);
bool Compare(int* pData,int* pKey);


ListNode* List_makeNode(void* pData){
    ListNode *pNewNode;
    pNewNode=(ListNode*)malloc(sizeof(ListNode));
    if(pNewNode==nullptr){
        return nullptr;
    }
    else{
        pNewNode->pData=pData;
        pNewNode->pNext=nullptr;
        return pNewNode;
    }
}

void List_freeNode(ListNode* pNode,void (*freeData)()){
    if(pNode!=nullptr){
        freeData(pNode->pData);
    }
    free(pNode);
    return;
}

void* ListGetData(ListNode* pNode){
    return pNode->pData;
}

List* ListConstruct(void){
    List* pList=(List*)malloc(sizeof(List));
    if(pList==nullptr){
        return nullptr;
    }
    pList->pFirst=nullptr;
    pList->length=0;
    return pList;
}

void ListClear(List* pList,void (*freeData)()){
    ListNode *pCurrentNode,*pNext;
    pCurrentNode=pList->pFirst;
    while(pCurrentNode!=nullptr){
        pNext=pCurrentNode->pNext;
        List_freeNode(pCurrentNode,freeData);
        pCurrentNode=pNext;
    }
    pList->pFirst=nullptr;
    pList->length=0;
    return;
}

void ListDestruct(List* pList,void (*freeData)()){
    ListClear(pList,freeData);
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

Status ListInsertAtFront(List* pList,void* pData){
    ListNode *pNewNode;
    pNewNode=List_makeNode(pData);
    if(pNewNode==nullptr){
        return ERROR;
    }
    pNewNode->pNext=pList->pFirst;
    pList->pFirst=pNewNode;
    pList->length++;
    return OK;
}

Status ListAppendToEnd(List* pList,void* pData){
    ListNode *pNewNode,*pTail;
    pNewNode=List_makeNode(pData);
    if(pNewNode==nullptr){
        return ERROR;
    }
    if(pList->pFirst==nullptr){
        pList->pFirst=pNewNode;
    }
    else{
        pTail=pList->pFirst;
        while(pTail->pNext!=nullptr){
            pTail=pTail->pNext;
        }
        pTail->pNext=pNewNode;
    }
    pList->length++;
    return OK;
}

Status ListInsertAfter(List* pList,
                        ListNode* pNode,void* pData){
    ListNode* pNewNode;
    pNewNode=List_makeNode(pData);
    if(pNewNode==nullptr){
        return ERROR;
    }
    pNewNode->pNext=pNode->pNext;
    pNode->pNext=pNewNode;
    pList->length++;
    return OK;
}

Status ListDeleteFirstNode(List* pList,void (*freeData)()){
    ListNode *pTemp;
    if(pList->length==0){
        return ERROR;
    }
    pTemp=pList->pFirst;
    pList->pFirst=pTemp->pNext;
    List_freeNode(pTemp,freeData);
    pTemp=nullptr;
    pList->length--;
    return OK;
}

Status ListDeleteLastNode(List* pList,void (*freeData)()){
    ListNode *p,*q;
    if(pList->length==0){
        return ERROR;
    }
    q=pList->pFirst;
    if(q->pNext==nullptr){
        return ListDeleteFirstNode(pList,freeData);
    }
    while(q->pNext!=nullptr){
        p=q;
        q=q->pNext;
    }
    p->pNext=nullptr;
    List_freeNode(q,freeData);
    q=nullptr;
    pList->length--;
    return OK;
}

Status ListDeleteNode(List* pList,
                       ListNode* pNode,void (*freeData)()){
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
        while(pTemp!=nullptr &&
              pTemp->pNext!=pNode){
            pTemp=pTemp->pNext;
        }
        if(pTemp==nullptr){
            return ERROR;
        }
        pTemp->pNext=pNode->pNext;
    }
    List_freeNode(pNode,freeData);
    pNode=nullptr;
    pList->length--;
    return OK;
}

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

void FreeData(int* pDynamicInteger){
    free(pDynamicInteger);
}

bool Compare(int* pData,int* pKey){
    return (*pData==*pKey)?true:false;
}

int main(){
    int i,key;
    int *pInteger;
    ListNode* pNode;
    List* pList;
    pList=ListConstruct();
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
        if(ListInsertAtFront(pList,pInteger)==ERROR){
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
    if(ListDeleteNode(pList,pNode,FreeData)==ERROR){
        printf("Error.List deletion failed.\n");
        return 1;
    }
    printf("This is the list after deletion:\n");
    ListTraverse(pList,PrintInteger);
    ListDestruct(pList,FreeData);
    return 0;
}



