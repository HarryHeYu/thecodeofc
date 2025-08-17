#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int ElemType;
typedef enum { ERROR = 0, OK = 1}Status;

#define MAXSIZE 100 //基于数组的队列的最大容量

typedef struct {
    ElemType data [MAXSIZE];//数组存储元素
    int front; //队头指针
    int rear; //队尾指针
}ArrayQueue;

//初始化队列
Status initQueue(ArrayQueue* q){
    q->front = 0;
    q->rear = 0;
    return OK;
}

//清空队列
Status clearQueue(ArrayQueue* q){
    q->front = 0;
    q->rear = 0;
    return OK;
}

//判断队列是否为空
bool isQueueEmpty(ArrayQueue* q){
    return q->front == q->rear;
}

//判断队列是否已满
bool isQueueFull(ArrayQueue* q){
    return q->rear == MAXSIZE;
}

//获取队列长度
int queueLength(ArrayQueue* q){
    return q->rear - q->front;
}

//获取队头元素
Status getHead(ArrayQueue* q, ElemType* item){
    if(isQueueEmpty(q)) return ERROR;
    *item = q->data[q->front];
    return OK;
}

//入队
Status enterQueue(ArrayQueue* q, ElemType* item){
    if(isQueueFull(q)) {
        printf("The queue is full,it can't enter.\n");
        return ERROR;
    }
    q->data[q->rear++] = *item;
    return OK;
}

//出队
Status leaveQueue(ArrayQueue* q, ElemType* item){
    if(isQueueEmpty(q)) {
        printf("The queue is empty, it can't leave.\n");
        return ERROR;
    }
    *item = q->data[q->front++];
    return OK;
}

//遍历队列
Status queueTraverse(ArrayQueue* q, void(*visit)(ElemType)){
    int i = q->front;
    while(i != q->rear) visit(q->data[i++]);
    printf("\n");
    return OK;
}

//任务调度应用函数
void taskScheduling(ArrayQueue* q){
    ElemType task;
    printf("任务调度开始...：\n");
    while(!isQueueEmpty(q)){
        leaveQueue(q, &task);
        printf("执行任务：%d\n",task);
    }
    printf("任务调度完成。\n");
}

//页面浏览历史记录应用函数
void printElement(ElemType e){
    printf("%d",e);
}

void browsingHistory(ArrayQueue* q){
    printf("页面浏览历史记录：\n");
    queueTraverse(q, printElement);
}

//打印任务队列应用函数
void printJobQueue(ArrayQueue* q){
    ElemType job;
    printf("打印任务队列：\n");
    while(!isQueueEmpty(q)){
        leaveQueue(q,&job);
        printf("打印任务：%d\n",job);
    }
}

// 测试基于数组的队列的主函数
int main() {
    ArrayQueue q;
    ElemType e;
    Status status;

    // 初始化队列
    initQueue(&q);
    printf("初始化队列，是否为空：%d\n", isQueueEmpty(&q));

    // 入队操作
    e = 1;
    enterQueue(&q, &e);
    e = 2;
    enterQueue(&q, &e);
    e = 3;
    enterQueue(&q, &e);
    printf("入队 1, 2, 3 后：");
    queueTraverse(&q, printElement);

    // 队列长度
    printf("队列长度：%d\n", queueLength(&q));

    // 获取队头元素
    getHead(&q, &e);
    printf("队头元素：%d\n", e);

    // 出队操作
    leaveQueue(&q, &e);
    printf("出队后：");
    queueTraverse(&q, printElement);
    printf("出队元素：%d\n", e);

    // 应用函数示例
    printf("应用函数示例:\n");
    taskScheduling(&q);
    browsingHistory(&q);
    printJobQueue(&q);

    // 清空队列
    clearQueue(&q);
    printf("队列已清空。\n");

    return 0;
}