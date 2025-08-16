/*
    ***********和前面的相比结构体有所更改*****
    ***********AOE网络关键路径计算(张学老师给的代码是用的邻接表)
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "Queue.c"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // for bool type
#include <string.h> // for memcpy
#include <time.h>

#include "Queue.c"

#define C24_VERSION 202311L
#if __STDC_VERSION__ < C24_VERSION
    #define nullptr NULL
    // Removed redefinition of bool to avoid conflict with <stdbool.h>
#else
    #include <stdbool.h>
#endif // __STDC_VERSION

typedef enum{ERROR=0,OK=1}Status;
typedef enum{NO=0,YES=1}Flag; // 0表示否，1表示是

#define NUM_OF_VERTEXES 10 // 顶点数
#define Vertex int // 顶点类型
#define Edge int // 边类型
#define Weight int // 权重类型
#define VertexInof int // 顶点信息类型
#define QUEUE_INIT_CAPACITY 10

typedef struct Queue{
    int* data; // 队列数据
    int front; // 队头指针
    int rear; // 队尾指针
    int capacity; // 队列容量
} Queue;

//邻接矩阵的一种定义
typedef struct {
    Vertex vertex[NUM_OF_VERTEXES]; // 顶点数组
    Edge matrix[NUM_OF_VERTEXES][NUM_OF_VERTEXES]; // 邻接矩阵
    int numOfVertexes; // 顶点数
}AdjacencyMatrixGraph; // 邻接矩阵图结构体


//邻接表的一种定义
typedef struct EEdge {
    int adjacentToVertex;  // 邻接顶点编号
    Weight weight;         // 边的权重
    struct EEdge* pNextEdge;  // 指向下一条边的指针
    bool isCritical; //新增：标记是否为关键边
} EEdge;
typedef struct VVertex{
    VertexInof info;
    EEdge* pFirstEdge;
} VVertex;
/*邻接表使用链表存储从某个顶点出发的所有边。对于顶点 v：
pFirstEdge 指向第一条出边（EEdge 类型的节点）。
每条 EEdge 包含目标顶点 adjacentToVertex、边的权重 weight，以及指向下一条边的指针 pNextEdge。
通过 pNextEdge 可以遍历该顶点的所有出边，形成一个单向链表。*/
typedef struct{
    VVertex vertex[NUM_OF_VERTEXES]; // 顶点数组
    int numOfVertexes; // 顶点数
    /*也可以有
    *int numOfEdges;
    *GraphType graphType;
    *其实通用的图效率不高，一般根据具体的应用来定义。
    */
}AdjacencyListGraph; // 邻接表图结构体

#define _GRAPH_TYPE 2 // 使用邻接矩阵图

#if _GRAPH_TYPE == 1
typedef AdjacencyMatrixGraph Graph; // 邻接矩阵图
//获取第一个邻接顶点
int FirstAdjacentVertex(Graph* pGraph, int i){
    if(i < 0 || i >= pGraph->numOfVertexes){
        return -1; // 错误
    }
    for(int j = 0; j < pGraph->numOfVertexes; j++){
        if(pGraph->matrix[i][j] != 0){ // 有边
            return j;
        }
    }
    return -1; // 没有邻接顶点
}

//获取下一个邻接顶点
int NextAdjacentVertex(Graph* pGraph, int i, int j){
    if(i < 0 || i >= pGraph->numOfVertexes || j < 0 || j >= pGraph->numOfVertexes){
        return -1; // 错误
    }
    for(int k = j + 1; k < pGraph->numOfVertexes; k++){
        if(pGraph->matrix[i][k] != 0){ // 有边
            return k;
        }
    }
    return -1; // 没有邻接顶点
}

#elif _GRAPH_TYPE == 2
typedef AdjacencyListGraph Graph; // 邻接表图
//获取第一个邻接顶点
int FirstAdjacentVertex(Graph* pGraph, int i){
    if(i < 0 || i >= pGraph->numOfVertexes){
        return -1; // 错误
    }
    EEdge* pEdge = pGraph->vertex[i].pFirstEdge;
    if(pEdge != nullptr){
        return pEdge->adjacentToVertex; // 返回第一个邻接顶点
    }
    return -1; // 没有邻接顶点
}
//获取下一个邻接顶点
int NextAdjacentVertex(Graph* pGraph, int i, int j){
    if(i < 0 || i >= pGraph->numOfVertexes || j < 0 || j >= pGraph->numOfVertexes){
        return -1; // 错误
    }
    EEdge* pEdge = pGraph->vertex[i].pFirstEdge;
    while(pEdge != nullptr){
        /*遍历邻接链表，检查每条边的目标顶点编号 pEdge->adjacentToVertex 是否等于 j。
如果找到目标顶点 j，进入下一步处理。*/
        if(pEdge->adjacentToVertex == j){
            if(pEdge->pNextEdge != nullptr){
                return pEdge->pNextEdge->adjacentToVertex; // 返回下一个邻接顶点
            }
            break;
        }
        pEdge = pEdge->pNextEdge;
    }
    return -1; // 没有邻接顶点
}
#endif // _GRAPH_TYPE

// 计算各顶点入度（邻接表版本）
void CaculateIndegree(Graph* pGraph, int indegree[]) {
    memset(indegree, 0, sizeof(int) * pGraph->numOfVertexes);
    for (int i = 0; i < pGraph->numOfVertexes; i++) {
        EEdge* pEdge = pGraph->vertex[i].pFirstEdge;
        while (pEdge != nullptr) {
            indegree[pEdge->adjacentToVertex]++; // 邻接顶点入度+1
            pEdge = pEdge->pNextEdge;
        }
    }
}
Status CriticalPath(Graph* pGraph){
    int n = pGraph->numOfVertexes;
    int inverseToSeq[n], earliestTime[n], latestTime[n];
    /*下面计算逆拓扑序列和最早时间，过程与拓扑排序相似*/
    int indegree[n];
    CaculateIndegree(pGraph,indegree);
    Queue queue;
    if(initQueue(&queue) == ERROR) return ERROR;
    for(int i = 0; i < n; i++){
        if(indegree[i] == 0) enterQueue(&queue, i);//入度为0的顶点已全部进入队列
        earliestTime[i] = 0;//最早发生时间初始化
    }
    int j = n - 1;//拓扑序列的下降，倒序
    int u, v;//存顶点
    EEdge* pEdge;
    while(isQueueEmpty(&queue) != true){
        leaveQueue(&queue,&u);
        inverseToSeq[j] = u;//输出逆拓扑序列
        j--;
        pEdge = pGraph->vertex[u].pFirstEdge;//取邻接表
        while(pEdge != nullptr){
            v = pEdge->adjacentToVertex;
            indegree[v]--;
            if(indegree[v] == 0) enterQueue(&queue, v);
            /*下面计算最早发生时间*/
            if(earliestTime[u] + pEdge->weight > earliestTime[v]){
                earliestTime[v] = earliestTime[u] + pEdge->weight;
            }
            pEdge = pEdge->pNextEdge;
        }//u邻接到的点处理完毕
    }//外循环结束，计算完毕
    if(j >= 0) return ERROR; //检错
    freeQueue(&queue);
    /*以上对最早发生时间和拓扑序列的计算与拓扑序列相似*/
    //下面计算最晚发生时间
    for(int i = 0; i < n; i++){
        latestTime[i] = earliestTime[n-1];
    }//最晚发生时间，0是源点，n-1是汇聚点
    for(j = 1; j < n; j++){//在逆拓扑序列中，下标为0的是汇点
        u = inverseToSeq[j];
        pEdge = pGraph->vertex[u].pFirstEdge;
        while(pEdge != nullptr){
            v = pEdge->adjacentToVertex;
            if(latestTime[v] - pEdge->weight < latestTime[u]){
                latestTime[u] = latestTime[v] - pEdge->weight;
            }
            pEdge = pEdge->pNextEdge;
        }//u的最晚发生时间计算完毕
    }//所有顶点的最晚发生时间计算完毕
    /*下面标记关键路径
     *注意：为边做标记，因为边表示活动，而顶点不表示活动
      *两顶点u,v在关键路径上不能证明(u,v)也在关键路径上*/
     for(u = 0; u < n; u++){//依次处理每个顶点
        pEdge = pGraph->vertex[u].pFirstEdge;
        while(pEdge != nullptr){//依次标记由u引出的边
            v = pEdge->adjacentToVertex;//取u的一个邻接点
            if(earliestTime[u] == latestTime[u] &&
               earliestTime[v] == latestTime[v] &&
               earliestTime[u] == latestTime[v] - pEdge->weight){
                pEdge->isCritical = true;
               }
               pEdge = pEdge->pNextEdge;
        }//从u引出的所有边标记完毕
    }//全部标记完毕
    return OK;
}