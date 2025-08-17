#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // 引入stdbool.h以使用bool类型

typedef int QElemType; // 元素类型
typedef enum { ERROR = 0, OK = 1 } Status; // 状态类型

#define MAXSIZE 20 // 循环队列的最大容量

/* 循环队列 */
typedef struct {
    QElemType data[MAXSIZE]; // 数组存储元素
    int front; // 队头指针
    int rear; // 队尾指针
} SqQueue;

// 初始化队列
Status InitQueue(SqQueue *Q) {
    Q->front = 0;
    Q->rear = 0;
    return OK;
}

// 清空队列
Status ClearQueue(SqQueue *Q) {
    Q->front = 0;
    Q->rear = 0;
    return OK;
}

// 检查队列是否为空
bool QueueEmpty(SqQueue Q) {
    return Q.front == Q.rear;
}

// 检查队列是否已满
bool QueueFull(SqQueue Q) {
    return (Q.rear + 1) % MAXSIZE == Q.front;
}

// 获取队列的长度
int QueueLength(SqQueue Q) {
    return (Q.rear - Q.front + MAXSIZE) % MAXSIZE;
}

// 获取队头元素但不移除
Status GetHead(SqQueue Q, QElemType *e) {
    if (Q.front == Q.rear) {
        return ERROR;
    }
    *e = Q.data[Q.front];
    return OK;
}

// 入队
Status EnQueue(SqQueue *Q, QElemType e) {
    if (QueueFull(*Q)) {
        return ERROR;
    }
    Q->data[Q->rear] = e;
    Q->rear = (Q->rear + 1) % MAXSIZE;
    return OK;
}

// 出队
Status DeQueue(SqQueue *Q, QElemType *e) {
    if (QueueEmpty(*Q)) {
        return ERROR;
    }
    *e = Q->data[Q->front];
    Q->front = (Q->front + 1) % MAXSIZE;
    return OK;
}

// 遍历队列，使用函数指针处理每个元素
Status QueueTraverse(SqQueue Q, void (*func)(QElemType)) {
    int i = Q.front;
    while (i != Q.rear) {
        func(Q.data[i]);
        i = (i + 1) % MAXSIZE;
    }
    printf("\n");
    return OK;
}

// 任务调度应用函数
void TaskScheduling(SqQueue *Q) {
    QElemType task;
    printf("任务调度开始...\n");
    while (!QueueEmpty(*Q)) {
        DeQueue(Q, &task);
        printf("执行任务: %d\n", task);
    }
    printf("任务调度完成。\n");
}

// 页面浏览历史记录应用函数
void printElement(QElemType e) {
    printf("%d ", e);
}

void BrowsingHistory(SqQueue *Q) {
    printf("页面浏览历史记录:\n");
    QueueTraverse(*Q, printElement);
}

// 打印任务队列应用函数
void PrintJobQueue(SqQueue *Q) {
    QElemType job;
    printf("打印任务队列:\n");
    while (!QueueEmpty(*Q)) {
        DeQueue(Q, &job);
        printf("打印任务: %d\n", job);
    }
}

// 测试循环队列的主函数
int main() {
    SqQueue Q;
    QElemType e;
    Status status;

    InitQueue(&Q);
    printf("初始化队列。是否为空：%d\n", QueueEmpty(Q));

    EnQueue(&Q, 1);
    EnQueue(&Q, 2);
    EnQueue(&Q, 3);
    printf("入队1, 2, 3后：");
    QueueTraverse(Q, printElement);

    printf("队列长度：%d\n", QueueLength(Q));

    GetHead(Q, &e);
    printf("队头元素：%d\n", e);

    DeQueue(&Q, &e);
    printf("出队后：");
    QueueTraverse(Q, printElement);
    printf("出队元素：%d\n", e);

    printf("应用函数示例:\n");
    TaskScheduling(&Q);
    BrowsingHistory(&Q);
    PrintJobQueue(&Q);

    ClearQueue(&Q);
    printf("队列已清空。\n");

    return 0;
}