/*  ***********Prim 算法
    ***********Topo sort
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "Queue.c"
#include "graph1.c"

// 定义 INFINITY 用于表示无穷大
#define INFINITY 99999

typedef struct {
    int u;         // U 中与 i 距离最近的点 u
    int distance;  // i 到 U 的距离，即 (u, i) 的权值
} DistanceToU;

typedef struct miniTreeEdge {
    int u;       // U 中与 v 距离最近的点
    int v;       // 当前加入 MST 的点
    int weight;  // 边 (u, v) 的权值
} MiniTreeEdge;

// Prim 算法
Status MiniSpanTreeByPrim(Graph* pGraph, int root, MiniTreeEdge miniTreeEdge[]) {
    int i, k, u, n, weight;
    n = pGraph->numOfVertexes;
    // 动态分配 distanceToU 数组
    DistanceToU* distanceToU = (DistanceToU*)malloc(n * sizeof(DistanceToU));
    /*distanceToU[i]表示顶点i到U的距离，即i到U中的所有点的
    最小权值。当i在U中时，其distance用0表示*/
    if (distanceToU == NULL) {
        printf("内存分配失败。\n");
        return ERROR;
    }
    u = root; // 初始点 root 加入 U 集合
    for (i = 0; i < n; i++) {
        distanceToU[i].u = u;
        distanceToU[i].distance = pGraph->matrix[u][i];
    }
    distanceToU[u].distance = 0; // 标记 root 已加入 U
    for (k = 0; k < n - 1; k++) {
        int v = NearestVertex(distanceToU, n); // 找到距离 U 最近的点 v
        u = distanceToU[v].u;
        miniTreeEdge[k].u = u;
        miniTreeEdge[k].v = v;
        miniTreeEdge[k].weight = pGraph->matrix[u][v];

        distanceToU[v].distance = 0; // 将 v 加入 U
        for (i = 0; i < n; i++) { //更新distanceToU
            weight = pGraph->matrix[v][i];
            if (weight != INFINITY && weight < distanceToU[i].distance) {
                distanceToU[i].u = v;
                distanceToU[i].distance = weight;
            }
        }
    }
    free(distanceToU); // 释放动态分配的内存
    return OK;
}

int NearestVertex(DistanceToU* distanceToU, int n) {
    int i, min = INFINITY, v = -1;
    for (i = 0; i < n; i++) {
        if (distanceToU[i].distance != 0 && distanceToU[i].distance < min) {
            /*表示顶点i尚未加入集合 U（因为已加入的顶点distance被标记为 0）。
            distanceToU[i].distance < min：找到距离集合 U 最近的顶点（即权值最小的边）。*/
            min = distanceToU[i].distance;
            v = i;
        }
    }
    return v;
}

// 计算每个顶点的入度  是有向图的入度
void CaculateIndegree(Graph* pGraph, int indegree[]) {
    int i, j;
    for (i = 0; i < pGraph->numOfVertexes; i++) {
        indegree[i] = 0;
    }
    for (i = 0; i < pGraph->numOfVertexes; i++) {
        for (j = 0; j < pGraph->numOfVertexes; j++) {
            if (pGraph->matrix[i][j] != INFINITY) {
                indegree[j]++;
            }
        }
    }
}

// 拓扑排序函数
Status TopoSort(Graph* pGraph, int topoSequence[]) {
    int i, j = 0, u, v;
    int n = pGraph->numOfVertexes;
    int* indegree = (int*)malloc(n * sizeof(int)); // 动态分配入度数组
    if (indegree == NULL) {
        return ERROR; // 内存分配失败
    }
    // 计算每个顶点的入度
    CaculateIndegree(pGraph, indegree);
    Queue queue;
    if (initQueue(&queue) == ERROR) { // 初始化队列
        free(indegree);
        return ERROR; // 队列初始化失败
    }
    // 将所有入度为 0 的顶点加入队列
    for (i = 0; i < n; i++) {
        if (indegree[i] == 0) {
            enterQueue(&queue, i);
        }
    }
    while (!isQueueEmpty(&queue)) {
        leaveQueue(&queue, &u);       // 出队一个入度为 0 的顶点
        topoSequence[j++] = u;       // 将顶点加入拓扑序列

        for (v = 0; v < n; v++) {
            if (pGraph->matrix[u][v] != INFINITY) {
                indegree[v]--;           // 邻接顶点的入度减 1
                if (indegree[v] == 0) {  // 如果邻接顶点的入度变为 0
                    enterQueue(&queue, v); // 入队该顶点
                }
            }
        }
    }
    // 检查是否所有顶点都加入了拓扑序列
    if (j != n) {
        free(indegree);
        freeQueue(&queue);
        return ERROR; // 图中存在环
    }
    free(indegree); // 释放动态分配的内存
    freeQueue(&queue); // 释放队列
    return OK;
}

