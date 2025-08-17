#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int ElemType;//元素类型
typedef enum { ERROR = 0, OK = 1 }Status;

#define MAXSIZE 20 //循环队列的最大容量

typedef struct  {
    ElemType data[MAXSIZE];
    int front; // 队头指针
    int rear; //队尾指针
}Queue;

//初始化队列
Status initQueue(Queue* q){
    q->front = 0;
    q->rear = 0;
    return OK;
}

//清空队列
Status clearQueue(Queue* q){
    q->front = 0;
    q->rear = 0;
    return OK;
}

//检查队列是否为空
bool isQueueEmpty(Queue* q){
    return q->front == q->rear;
}

//检查队列是否已满
bool isQueueFull(Queue* q){
    return (q->rear + 1) % MAXSIZE == q->front;
}
/*说明队列已满（此时队列中有 MAXSIZE - 1 个元素，牺牲一个单元避免与空队列混淆）。*/
/*如果不牺牲单元，空队列和满队列的判断条件会冲突（front == rear）。通过牺牲一个单元：
空队列：front == rear。
满队列：(rear + 1) % MAXSIZE == front。
队列长度：(rear - front + MAXSIZE) % MAXSIZE。*/
/*当循环队列满时，rear指针的下一个位置（(rear+1) % MAXSIZE）恰好等于front指针的位置。
此时，front所指向的单元不存放有效数据，仅作为队列满的标志。*/
/*循环队列中，front通常指向队头元素的前一个位置，这种设计便于入队 / 出队操作时指针的移动。
牺牲头指针位置后，判断队列满的条件简化为(rear+1) % MAXSIZE == front，无需额外变量记录队列长度。*/

//获取队列的长度
Status getHead(Queue* q, ElemType* e){
    if(isQueueEmpty(q))return ERROR;
    *e = q->data[q->front];
    return OK;
}

//入队
Status enterQueue(Queue* q, ElemType* e){
    if(isQueueFull(q)) return ERROR;
    q->data[q->rear] = *e;
    q->rear = (q->rear+1) % MAXSIZE;
    return OK;
}

//出队
Status leaveQueue(Queue* q, ElemType* e){
    if(isQueueEmpty(q)) return ERROR;
    *e = q->data[q->front];
    q->front = (q->front + 1) % MAXSIZE;
    return OK;
}
/*循环队列必须要先对头或尾指针指向的进行元素的操作然后再进行对头/尾指针的操作*/

//遍历队列
Status queueTraverse(Queue* q, void(*visit)(ElemType)){
    int i = q->front;
    while(i != q->rear){
        visit(q->data[i]);
        i = (i + 1) % MAXSIZE;
    }
    printf("\n");
    return OK;
}

//任务调度应用函数
void taskScheduling(Queue *q){
    ElemType task;
    printf("任务调度开始...\n");
    while (!isQueueEmpty(q))
    {
        leaveQueue(q, &task);
        printf("执行任务:%d\n",task);
    }
    printf("任务调度完成。\n");    
}

//页面浏览历史记录
void printElement(ElemType e){
    printf("%d",e);
}

void browsingHistory(Queue* q){
    printf("页面浏览历史记录：\n");
    queueTraverse(q, printElement);
}

//打印任务队列应用函数
void printJobQueue(Queue* q){
    ElemType job;
    printf("打印任务队列:\n");
    while(!isQueueEmpty(q)){
        leaveQueue(q,&job);
        printf("打印任务:%d\n",job);
    }
}

void testCircularQueue() {
    Queue q;
    ElemType e;
    Status status;

    // 测试初始化队列
    printf("测试初始化队列:\n");
    status = initQueue(&q);
    printf("初始化队列，状态：%s\n", status == OK ? "OK" : "ERROR");
    printf("队列是否为空：%d\n", isQueueEmpty(&q));

    // 测试入队操作
    printf("\n测试入队操作:\n");
    for (int i = 1; i <= 5; i++) {
        e = i;
        status = enterQueue(&q, &e);
        printf("入队元素 %d，状态：%s\n", e, status == OK ? "OK" : "ERROR");
    }

    // 测试队列是否已满
    printf("队列是否已满：%d\n", isQueueFull(&q));

    // 测试遍历队列
    printf("\n测试遍历队列:\n");
    printf("队列元素：");
    queueTraverse(&q, printElement);

    // 测试获取队头元素
    printf("\n测试获取队头元素:\n");
    status = getHead(&q, &e);
    if (status == OK) {
        printf("队头元素：%d\n", e);
    } else {
        printf("获取队头元素失败，队列为空。\n");
    }

    // 测试出队操作
    printf("\n测试出队操作:\n");
    for (int i = 0; i < 3; i++) {
        status = leaveQueue(&q, &e);
        if (status == OK) {
            printf("出队元素：%d\n", e);
        } else {
            printf("出队失败，队列为空。\n");
        }
    }

    // 测试队列长度
    printf("\n测试队列长度:\n");
    printf("当前队列长度：%d\n", (q.rear - q.front + MAXSIZE) % MAXSIZE);

    // 测试任务调度
    printf("\n测试任务调度:\n");
    taskScheduling(&q);

    // 测试浏览历史记录
    printf("\n测试浏览历史记录:\n");
    for (int i = 6; i <= 10; i++) {
        e = i;
        enterQueue(&q, &e);
    }
    browsingHistory(&q);

    // 测试打印任务队列
    printf("\n测试打印任务队列:\n");
    printJobQueue(&q);

    // 测试清空队列
    printf("\n测试清空队列:\n");
    status = clearQueue(&q);
    printf("清空队列，状态：%s\n", status == OK ? "OK" : "ERROR");
    printf("队列是否为空：%d\n", isQueueEmpty(&q));
}

int main() {
    printf("开始测试循环队列功能...\n");
    testCircularQueue();
    printf("循环队列功能测试完成。\n");
    return 0;
}