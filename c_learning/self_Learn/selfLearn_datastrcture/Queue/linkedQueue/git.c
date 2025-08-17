#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // 引入stdbool.h以使用bool类型

typedef int QElemType; // 元素类型
typedef enum { ERROR = 0, OK = 1 } Status; // 状态类型

/* 队列结点 */
typedef struct QNode {
    QElemType data;
    struct QNode *next;
} QNode, *QueuePtr;

/* 链式队列 */
typedef struct {
    QueuePtr front, rear; // 队头和队尾指针
} LinkQueue;

// 创建队列
LinkQueue* CreateQueue() {
    LinkQueue *Q = (LinkQueue *)malloc(sizeof(LinkQueue));
    if (Q == NULL) {
        return NULL;
    }
    Q->front = (QueuePtr)malloc(sizeof(QNode));
    if (Q->front == NULL) {
        free(Q);
        return NULL;
    }
    Q->front->next = NULL;
    Q->rear = Q->front;
    return Q;
}

// 初始化队列
Status InitQueue(LinkQueue *Q) {
    Q->front = (QueuePtr)malloc(sizeof(QNode));
    if (Q->front == NULL) {
        return ERROR;
    }
    Q->front->next = NULL;
    Q->rear = Q->front;
    return OK;
}

// 销毁队列
Status DestroyQueue(LinkQueue *Q) {
    while (Q->front) {
        Q->rear = Q->front->next;
        free(Q->front);
        Q->front = Q->rear;
    }
    free(Q);
    return OK;
}

// 清空队列
Status ClearQueue(LinkQueue *Q) {
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

// 检查队列是否为空
bool QueueEmpty(LinkQueue Q) {
    return Q.front == Q.rear;
}

// 获取队列的长度
int QueueLength(LinkQueue Q) {
    int length = 0;
    QueuePtr p = Q.front->next;
    while (p) {
        length++;
        p = p->next;
    }
    return length;
}

// 获取队头元素但不移除
Status GetHead(LinkQueue Q, QElemType *e) {
    if (Q.front == Q.rear) {
        return ERROR;
    }
    *e = Q.front->next->data;
    return OK;
}

// 入队
Status EnQueue(LinkQueue *Q, QElemType e) {
    QueuePtr s = (QueuePtr)malloc(sizeof(QNode));
    if (!s) {
        return ERROR;
    }
    s->data = e;
    s->next = NULL;
    Q->rear->next = s;
    Q->rear = s;
    return OK;
}

// 出队
Status DeQueue(LinkQueue *Q, QElemType *e) {
    if (Q->front == Q->rear) {
        return ERROR;
    }
    QueuePtr p = Q->front->next;
    *e = p->data;
    Q->front->next = p->next;
    if (Q->rear == p) {
        Q->rear = Q->front;
    }
    free(p);
    return OK;
}

// 遍历队列，使用函数指针处理每个元素
Status QueueTraverse(LinkQueue Q, void (*func)(QElemType)) {
    QueuePtr p = Q.front->next;
    while (p) {
        func(p->data);
        p = p->next;
    }
    printf("\n");
    return OK;
}

// 任务调度应用函数
void TaskScheduling(LinkQueue *Q) {
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

void BrowsingHistory(LinkQueue *Q) {
    QElemType page;
    printf("页面浏览历史记录:\n");
    QueueTraverse(*Q, printElement);
}

// 打印任务队列应用函数
void PrintJobQueue(LinkQueue *Q) {
    QElemType job;
    printf("打印任务队列:\n");
    while (!QueueEmpty(*Q)) {
        DeQueue(Q, &job);
        printf("打印任务: %d\n", job);
    }
}

// 测试链式队列的主函数
int main() {
    LinkQueue *Q = CreateQueue();
    if (Q == NULL) {
        printf("队列创建失败\n");
        return -1;
    }

    QElemType e;
    Status status;

    status = InitQueue(Q);
    if (status == ERROR) {
        printf("队列初始化失败\n");
        return -1;
    }

    printf("初始化队列。是否为空：%d\n", QueueEmpty(*Q));

    EnQueue(Q, 1);
    EnQueue(Q, 2);
    EnQueue(Q, 3);
    printf("入队1, 2, 3后：");
    QueueTraverse(*Q, printElement);

    printf("队列长度：%d\n", QueueLength(*Q));

    GetHead(*Q, &e);
    printf("队头元素：%d\n", e);

    DeQueue(Q, &e);
    printf("出队后：");
    QueueTraverse(*Q, printElement);
    printf("出队元素：%d\n", e);

    printf("应用函数示例:\n");
    TaskScheduling(Q);
    BrowsingHistory(Q);
    PrintJobQueue(Q);

    ClearQueue(Q);
    printf("队列已清空。\n");

    DestroyQueue(Q);
    printf("队列已销毁。\n");

    return 0;
}