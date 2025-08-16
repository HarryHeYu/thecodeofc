/* Generic Stack and Queue*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define C24_VERSION 202311L
#if __STDC_VERSION__ < C24_VERSION
    #define nullptr NULL
    typedef enum {false=0,true=1}bool;
#endif // __STDC_VERSION__

/* This is Stack */
#define STACK_INIT_SIZE 10
#define STACK_INCREMENT 10
typedef enum {ERROR=0,OK=1} Status;
typedef struct{
    void *pBase;
    void *pTop;
    int stackSize;
    int elemSize;
}Stack;
Stack* StackConstruct(int sizeOfElement);
void StackDestruct(Stack *pStack);
void StackClear(Stack *pStack);
bool StackIsEmpty(Stack *pStack);
int StackLength(Stack *pStack);
Status StackGetTop(Stack *pStack,void *pElem);
Status StackPush(Stack *pStack,void *pElem);
Status StackPop(Stack *pStack,void *pElem);
Status StackTraverse(Stack *pStack,Status (*visit)());

/* This is Queue */
typedef struct {
    void *q;
    int front;
    int rear;
    int elemSize;
    int queueSize;
} Queue;
/* ASSIGNMENT 1 TO DO:
 * Implement the following operations on Queue. */
Queue* QueueConstruct(int initSize,int sizeOfElement);
void QueueDestruct(Queue *pQueue);
void QueueClear(Queue *pQueue);
bool QueueIsEmpty(Queue *pQueue);
int QueueLength(Queue *pQueue);
Status QueueEnter(Queue *pQueue,void *pElem);
Status QueueLeave(Queue *pQueue,void *pElem);
Status QueueTraverse(Queue *pQueue,Status (*visit)());

/* ASSIGMENT 2 TO DO
 * Implement the following Queue:
 * typedef struct QueueNode QueueNode;
 * struct QueueNode {
 *     void *pElem;
 *     QueueNode *pNext;
 * };
 * typedef struct {
 *     QueueNode *pFront;
 *     QueueNode *pRear;
 *     int elemSize;
 *     int queueSize;
 * } Queue;
 */

/* These are application functions */
Status Print(int *pi);  // Print *pi on the screen.
Status AddOne(int *pi); // Increase *pi by 1.

Stack* StackConstruct(int sizeOfElement){
    Stack* pStack;
    pStack=(Stack*)malloc(sizeof(Stack));
    if(pStack==NULL){
        return NULL;
    }
    pStack->pBase=malloc(STACK_INIT_SIZE*sizeOfElement);
    if(pStack->pBase==NULL){
        free(pStack);
        return NULL;
    }
    pStack->pTop=pStack->pBase;
    pStack->stackSize=STACK_INIT_SIZE;
    pStack->elemSize=sizeOfElement;
    return pStack;
}

void StackDestruct(Stack *pStack){
    if(pStack==NULL){
        return;
    }
    free(pStack->pBase);
    free(pStack);
    return;
}

void StackClear(Stack *pStack){
    pStack->pTop=pStack->pBase;
}

bool StackIsEmpty(Stack *pStack){
    if(pStack->pTop==pStack->pBase){
        return true;
    }
    else{
        return false;
    }
}

int StackLength(Stack *pStack){
    char *pcTop,*pcBase;
    int length;
    pcTop=pStack->pTop;
    pcBase=pStack->pBase;
    length=(pcTop-pcBase)/(pStack->elemSize);
    return length;
}

Status StackGetTop(Stack *pStack,void *pElem){
    char* pc;
    if(pStack->pTop==pStack->pBase){
        return ERROR;
    }
    pc=pStack->pTop;
    pc-=pStack->elemSize;
    memcpy(pElem,pc,pStack->elemSize);
    return OK;
}

Status StackPush(Stack *pStack,void *pElem){
    int newSize;
    void *pTemp;
    char* pc;
    if(StackLength(pStack)>=pStack->stackSize){
        newSize=pStack->stackSize+STACK_INCREMENT;
        pTemp=realloc(pStack->pBase,
                      newSize*(pStack->elemSize));
        if(pTemp==NULL){
            return ERROR;
        }
        else{
            pStack->pBase=pTemp;
            pc=(char*)pTemp;
            pc+=(pStack->stackSize)*(pStack->elemSize);
            pStack->pTop=pc;
            pStack->stackSize=newSize;
        }
    }
    memcpy(pStack->pTop,pElem,pStack->elemSize);
    pc=(char*)(pStack->pTop);
    pc+=pStack->elemSize;
    pStack->pTop=pc;
    return OK;
}

Status StackPop(Stack *pStack,void *pElem){
    char* pc;
    if(pStack->pTop==pStack->pBase){
        return ERROR;
    }
    else{
        pc=(char*)(pStack->pTop);
        pStack->pTop=pc-pStack->elemSize;
        memcpy(pElem,pStack->pTop,pStack->elemSize);
        return OK;
    }
}

Status StackTraverse(Stack *pStack,Status (*visit)()){
    int i,n;
    char* pc;
    n=StackLength(pStack);
    pc=(char*)(pStack->pBase);
    for(i=0;i<n;i++){
        if(visit(pc)==ERROR){
            return ERROR;
        }
        pc+=pStack->elemSize;
    }
    return OK;
}

Status Print(int *pi){
    printf("%d ",*pi);
    return OK;
}

Status AddOne(int *pi){
    *pi=*pi+1;
    return OK;
}

int main(){
    int i;
    int n=20;
    Stack *pIntStack;
    pIntStack=StackConstruct(sizeof(int));
    if(pIntStack==nullptr){
        return 1;
    }
    for(i=1;i<n;i++){
        StackPush(pIntStack,&i);
    }
    printf("This is the initial stack:\n");
    printf("From base to top: ");
    StackTraverse(pIntStack,Print);
    StackTraverse(pIntStack,AddOne);
    printf("\nIncrease each element by 1:\n");
    printf("From base to top: ");
    StackTraverse(pIntStack,Print);
    printf("\n");
    while(!StackIsEmpty(pIntStack)){
        StackPop(pIntStack,&i);
        printf("Pop: i=%d \n",i);
    }
    StackDestruct(pIntStack);
    /* ASSIGNMENT 3 TO DO: Test the Queue. */
    return 0;
}
