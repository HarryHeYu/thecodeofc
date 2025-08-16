# include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
typedef int StackElem;
typedef enum{ERROR=0, OK=1}Status;
typedef enum{FALSE=0, TRUE=1}Bool;
typedef struct ListNode{
    struct ListNode *pFirstNode;
    struct ListNode *pNextNode;
    void *pData;
}ListNode;
typedef struct{
    ListNode *pFirstNode;
    int length;
}List;

void freeData(void* data) {
    if (data != NULL) {
        free(data);
    }
}
static ListNode* ListMakeNode(void* pData){
    ListNode* pNode;
    pNode=(ListNode*)malloc(sizeof(ListNode));
    if(pNode==NULL)return NULL;
    pNode->pNextNode=NULL;
    pNode->pData=pData;
    return pNode;
}
static void ListFreeNode(ListNode* pNode, void (*freeData)()){
    if(pNode==NULL)return;
    freeData(pNode->pData);
    free(pNode);
    return;
}
List* ListConstruct(){
    List* pList;
    pList=(List*)malloc(sizeof(List));
    if(pList==NULL)return NULL;
    pList->pFirstNode=NULL;
    pList->length=0;
    return pList;
}
void ListClear(List* pList, void(*freeData)()){
    ListNode *pNode,*pNextNode;
    pNode=pList->pFirstNode;
    while(pNode!=NULL){
        pNextNode=pNode->pNextNode;
        ListFreeNode(pNode,freeData);
        pNode=pNextNode;
    }
    pList->pFirstNode=NULL;
    pList->length=0;
}
void* ListGetData(ListNode* pNode){
    return pNode->pData;
}
Status ListInsertAtFront(List* pList, void* pData){
    ListNode* pNewNode;
    pNewNode=ListMakeNode(pData);
    if(pNewNode==NULL)return ERROR;
    pNewNode->pNextNode=pList->pFirstNode;//将pNewNode设为新的第一个节点（即把原来的第一个节点设为newnode的nextnode）
    pList->pFirstNode=pNewNode;//然后重新将第一个节点改设为newnode
    pList->length++;
    return OK;
}
bool ListIsEmpty(List *pList){
    return pList->length==0?TRUE:FALSE;
}
Status ListAppendToTail(List* pList,void* pData){
    ListNode* pNewNode,*pTail;
    pNewNode=ListMakeNode(pData);
    if(pNewNode==NULL)return ERROR;
    if(ListIsEmpty(pList)){
        pList->pFirstNode=pNewNode;
        pList->length++;
        return OK;
    }
    pTail=pList->pFirstNode;
    while(pTail->pNextNode!=NULL)pTail=pTail->pNextNode;
    pTail->pNextNode=pNewNode;
    pList->length++;
    return OK;
}
Status ListDeleteFirstNode(List* pList, void(*freeData)()){
    ListNode *pTemp;
    if(pList==NULL)return ERROR;
    if(ListIsEmpty(pList))return OK;
    pTemp=pList->pFirstNode;
    pList->pFirstNode=pTemp->pNextNode;
    ListFreeNode(pTemp,freeData);
    pList->length--;
    return OK;
}
Status ListDeleteNode(List* pList, ListNode* pNode, void(*freeData)()){
    ListNode* pTail,*pPreviousNode;
    if(pList==NULL)return ERROR;
    if(pList->length==1){
        ListFreeNode(pList->pFirstNode,freeData);
        pList->pFirstNode=NULL;
        pList->length--;
        return OK;
    }
    pPreviousNode=pList->pFirstNode;
    pTail=pList->pFirstNode->pNextNode;
    while(pTail->pNextNode!=NULL){
        pPreviousNode=pTail;
        pTail=pTail->pNextNode;
    }
    ListFreeNode(pTail,freeData);
    pPreviousNode->pNextNode=NULL;
    pList->length--;
    return OK;
}