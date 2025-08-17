#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int ElemType;
typedef enum {ERROR = 0, OK = 1} Status;

//队列节点
typedef struct Node {
    ElemType data;
    struct Node *next;
} qNode, *QueuePtr;

//链式队列
typedef struct {
    QueuePtr front, rear; //队头和队尾指针
} LinkQueue;

//创建队列
LinkQueue* createQueue() {
    LinkQueue* q = (LinkQueue*)malloc(sizeof(LinkQueue));
    if (q == NULL) return NULL;
    q->front = NULL;
    q->rear = NULL;
    return q;
}

//初始化队列
Status initQueue(LinkQueue* q) {
    q->front = (QueuePtr)malloc(sizeof(qNode));
    q->rear = q->front;
    if (!q->front) return ERROR;
    q->front->next = NULL;
    return OK;
}

//销毁队列
Status destroyQueue(LinkQueue* q) {
    if (q == NULL) return ERROR;
    while (q->front) {
        q->rear = q->front->next;
        free(q->front);
        q->front = q->rear;
    }
    free(q);
    return OK;
}

//清空队列
Status clearQueue(LinkQueue *Q) {
    QueuePtr p, q;
    Q->rear = Q->front;
    p = Q->front->next;
    Q->front->next = NULL;
    while (p) {
        q = p;
        p = p->next;
        free(q);
    }
    return OK;
}

//检查队列是否为空
bool queueIsEmpty(LinkQueue *q) {
    return q->front == q->rear;
}

//获取队列的长度
int queueLength(LinkQueue *q) {
    int length = 0;
    QueuePtr p = q->front;
    while (p) {
        length++;
        p = p->next;
    }
    return length;
}

//获取队列的头元素
Status getHead(LinkQueue* q, ElemType *e) {
    if (queueIsEmpty(q)) return ERROR;
    *e = q->front->data;
    return OK;
}

//入队
Status enQueue(LinkQueue *q, ElemType *e) {
    QueuePtr s = (QueuePtr)malloc(sizeof(qNode));
    if (!s) return ERROR;
    s->data = *e;
    s->next = NULL;
    if (queueIsEmpty(q)) {
        q->front = s;
        q->rear = s;
    } else {
        q->rear->next = s;
        q->rear = s;
    }
    return OK;
}

//出队
Status leaveQueue(LinkQueue* q, ElemType *e) {
    if (queueIsEmpty(q)) return ERROR;
    QueuePtr p = q->front;
    *e = p->data;
    q->front = p->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(p);
    return OK;
}

//遍历队列
Status queueTraverse(LinkQueue* q, void(*visit)(ElemType)) {
    QueuePtr p = q->front;
    while (p) {
        visit(p->data);
        p = p->next;
    }
    printf("\n");
    return OK;
}

// 任务调度应用函数
void TaskScheduling(LinkQueue* q) {
    ElemType task;
    printf("任务调度开始...\n");
    while (!queueIsEmpty(q)) {
        leaveQueue(q, &task);
        printf("执行任务: %d\n", task);
    }
    printf("任务调度完成。\n");
}

// 页面浏览历史记录应用函数
void printElement(ElemType e) {
    printf("%d ", e);
}

void BrowsingHistory(LinkQueue* q) {
    printf("页面浏览历史记录:\n");
    queueTraverse(q, printElement);
}

// 打印任务队列应用函数
void PrintJobQueue(LinkQueue* q) {
    ElemType job;
    printf("打印任务队列:\n");
    while (!queueIsEmpty(q)) {
        leaveQueue(q, &job);
        printf("打印任务: %d\n", job);
    }
}

// 测试链式队列的主函数
int main() {
    LinkQueue* q = createQueue();
    if (q == NULL) {
        printf("队列创建失败\n");
        return -1;
    }

    ElemType e;
    Status status;

    printf("初始化队列。是否为空：%d\n", queueIsEmpty(q));

    ElemType elem1 = 1, elem2 = 2, elem3 = 3;
    enQueue(q, &elem1);
    enQueue(q, &elem2);
    enQueue(q, &elem3);
    printf("入队1, 2, 3后：");
    queueTraverse(q, printElement);

    printf("队列长度：%d\n", queueLength(q));

    getHead(q, &e);
    printf("队头元素：%d\n", e);

    leaveQueue(q, &e);
    printf("出队后：");
    queueTraverse(q, printElement);
    printf("出队元素：%d\n", e);

    printf("应用函数示例:\n");
    TaskScheduling(q);
    BrowsingHistory(q);
    PrintJobQueue(q);

    clearQueue(q);
    printf("队列已清空。\n");

    destroyQueue(q);
    printf("队列已销毁。\n");

    return 0;
}