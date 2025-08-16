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
// typedef struct {
    //     void *q;
    //     int front;
    //     int rear;
    //     int elemSize;
    //     int queueSize;
    // } Queue;
    /* ASSIGNMENT 1 TO DO:
    * Implement the following operations on Queue. */

/* ASSIGMENT 2 TO DO
 *Implement the following Queue:*/
typedef struct QueueNode {
    void *pElem;
    struct QueueNode *pNext;
} QueueNode;
typedef struct Queue{
    struct QueueNode *pFront;
    struct QueueNode *pRear;
    int elemSize;
    int queueSize;
} Queue;
QueueNode* QueueNodeConstruct(void* pElem, int elemSize);
Queue* QueueConstruct(int initSize,int sizeOfElement);
Status QueueDestruct(Queue *pQueue);
Status QueueClear(Queue *pQueue);
bool QueueIsEmpty(Queue *pQueue);
bool QueueIsFull(Queue *pQueue);
int QueueLength(Queue *pQueue);
Status QueueEnter(Queue *pQueue,void *pElem);
Status QueueLeave(Queue *pQueue,void *pElem);
Status QueueTraverse(Queue *pQueue,Status (*visit)());



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
/*pTemp 是 void* 类型，表示它是一个通用指针，不能直接进行指针算术操作。
为了能够进行字节级别的偏移操作，需要将 pTemp 转换为 char*，因为 char 的大小是 1 字节。
pTemp 是一个 void* 类型的指针，而 void* 是通用指针类型，不能直接进行指针算术操作。将其转换为 char* 后，可以以字节为单位进行偏移操作。*/
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

// //创造一个队列
// Queue* QueueConstruct(int initSize, int sizeOfElement){
//     Queue* pQueue;
//     pQueue = (Queue*)malloc(sizeof(Queue));
//     if(pQueue == NULL){
//         return NULL;
//     }
//     pQueue->q = malloc(initSize * sizeOfElement);
//     if(pQueue->q == NULL){
//         free(pQueue);
//         return NULL;
//     }
//     pQueue->front = 0;
//     pQueue->rear = 0;
//     pQueue->elemSize = sizeOfElement;
//     pQueue->queueSize = initSize;
//     return pQueue;
// }

// //销毁队列
// void QueueDestruct(Queue* pQueue){
//     if(pQueue == NULL){
//         return;
//     }
//     free(pQueue->q);
//     free(pQueue);
//     return;
// }

// //清空队列
// void QueueClear(Queue* pQueue){
//     if(pQueue == NULL){
//         return;
//     }
//     pQueue->front = 0;
//     pQueue->rear = 0;
//     return;
// }

// //判断队列是否为空
// bool QueueIsEmpty(Queue* pQueue){
//     return pQueue->front == pQueue->rear;
// }

// //判断队列是否为满
// bool QueueIsFull(Queue* pQueue){
//     return (pQueue->rear + 1) % pQueue->queueSize == pQueue->front;
// }

// //获取队列的长度
// int QueueLength(Queue* pQueue){
//     return(pQueue->rear - pQueue->front+ pQueue->queueSize) % pQueue->queueSize;
// }

// //加入队列
// Status QueueEnter(Queue* pQueue, void* pElem){
//     if(QueueIsFull(pQueue)) return ERROR;
//     char* pRear = (char*)pQueue->q + pQueue->elemSize * pQueue->rear;
//     memcpy(pRear, pElem, pQueue->elemSize);
//     pQueue->rear = (pQueue->rear + 1) % pQueue->queueSize;
//     return OK;
// }

// //离开队列
// Status QueueLeave(Queue* pQueue, void* pElem){
//     if(QueueIsEmpty(pQueue)) return ERROR;
//     char* pc = (char*)pQueue->q[pQueue->front];
//     memcpy(pElem, pc, pQueue->elemSize);
//     pQueue->front = (pQueue->front + 1) % pQueue->queueSize;
//     return OK;
// }

// //遍历队列
// Status QueueTraverse(Queue* pQueue, Status(*visit)()){
//     if(QueueIsEmpty(pQueue)) return ERROR;
//     for(int i = pQueue->front; i != pQueue->rear; (i+1) % pQueue->queueSize){
//         if(visit((char*)pQueue->q + i * pQueue->elemSize) == ERROR){
//             return ERROR;
//         }
//     }
//     return OK;
// }

// 创建一个队列节点
QueueNode* QueueNodeConstruct(void* pElem, int elemSize) {
    QueueNode* pNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (pNode == NULL) return NULL;
    pNode->pElem = malloc(elemSize);
    if (pNode->pElem == NULL) {
        free(pNode);
        return NULL;
    }
    memcpy(pNode->pElem, pElem, elemSize);
    pNode->pNext = NULL; // 初始化 pNext
    return pNode;
}

//创建一个队列
Queue* QueueConstruct(int initSize, int sizeOfElemt){
    Queue* pQueue = (Queue*)malloc(sizeof(Queue));
    if (pQueue == NULL) return NULL;
    pQueue->pFront = NULL;
    pQueue->pRear = NULL;
    pQueue->elemSize = sizeOfElemt;
    pQueue->queueSize = initSize;
    return pQueue;
}

//销毁队列节点
Status QueueNodeDestruct(QueueNode* pNode){
    if(pNode == NULL) return ERROR;
    free(pNode->pElem);
    free(pNode);
    return OK;
}

//销毁队列
Status QueueDestruct(Queue* pQueue){
    if(pQueue == NULL) return ERROR;
    QueueNode* pTemp;
    while(pQueue->pFront != NULL){
        pTemp = pQueue->pFront;
        pQueue->pFront = pQueue->pFront->pNext;
        free(pTemp->pElem);
        free(pTemp);
    }
    free(pQueue);
    return OK;
}

//清空队列
Status QueueClear(Queue* pQueue){
    if(pQueue == NULL) return ERROR;
    QueueNode* pTemp;
    while(pQueue->pFront != NULL){
        pTemp = pQueue->pFront;
        pQueue->pFront = pQueue->pFront->pNext;
        free(pTemp->pElem);
        free(pTemp);
    }
    pQueue->pFront = NULL;
    pQueue->pRear = NULL;
    return OK;
}

//判断队列是否为空
bool QueueIsEmpty(Queue* pQueue){
    return pQueue->pFront == NULL;
}

//判断队列是否为满
bool QueueIsFull(Queue* pQueue) {
    // 判断队列是否达到最大大小
    int currentLength = QueueLength(pQueue);
    return currentLength >= pQueue->queueSize;
}

//获取队列的长度
int QueueLength(Queue* pQueue){
    int length = 0;
    QueueNode* pTemp = pQueue->pFront;
    while(pTemp != NULL){
        length++;
        pTemp = pTemp->pNext;
    }
    return length;
}

// 加入队列
Status QueueEnter(Queue* pQueue, void *pElem) {
    if (QueueIsFull(pQueue)) return ERROR;
    QueueNode* pNode = QueueNodeConstruct(pElem, pQueue->elemSize);
    if (pNode == NULL) return ERROR;
    if (pQueue->pFront == NULL) {
        pQueue->pFront = pNode;
        pQueue->pRear = pNode;
    } else {
        pQueue->pRear->pNext = pNode;
        pQueue->pRear = pNode;
    }
    return OK;
}


// 离开队列
Status QueueLeave(Queue* pQueue, void* pElem) {
    if (QueueIsEmpty(pQueue)) return ERROR;
    QueueNode* pTemp = pQueue->pFront;
    memcpy(pElem, pTemp->pElem, pQueue->elemSize);
    pQueue->pFront = pTemp->pNext;
    if (pQueue->pFront == NULL) pQueue->pRear = NULL;
    QueueNodeDestruct(pTemp);
    return OK;
}

//遍历队列
Status QueueTraverse(Queue* pQueue, Status(*visit)()){
    if(pQueue == NULL || visit == NULL) return ERROR;
    QueueNode* pTemp = pQueue->pFront;
    while(pTemp != NULL){
        if(visit(pTemp->pElem) != OK) return ERROR;
        pTemp = pTemp->pNext;
    }
    return OK;
}

//打印整数
Status PrintInt(void *pElem) {
    printf("%d ", *(int *)pElem);
    return OK;
}

//测试函数
void TestQueue() {
    Queue* pQueue = QueueConstruct(10, sizeof(int));
    if( pQueue == NULL){
        printf("Queue construct failed.\n");
        return ;
    }
    int i;
    for(i =  0; i < 10; i++){
        if(QueueEnter(pQueue, &i) != OK){
            printf("Queue enter failed at %d..\n", i);
        }
    }
    printf("Queue after entering elements:");
    QueueTraverse(pQueue, PrintInt);
    printf("\n");
    QueueClear(pQueue);
    printf("Queue after clearing:");
    QueueTraverse(pQueue, PrintInt);
    printf("\n");
    QueueDestruct(pQueue);
    printf("Queue after destructing:");
}

int main(){
    TestQueue();
    return 0;
}