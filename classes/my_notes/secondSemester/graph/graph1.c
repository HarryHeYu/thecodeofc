/*  ***********邻接表
    ***********邻接矩阵
    ***********bfs、dfs
*/


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

#define _GRAPH_TYPE 1 // 使用邻接矩阵图

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


//图的遍历的实现
Status TraverseGraph(Graph* pGraph, void (*visit)()){
    /*用动态数组记录图中的节点是否被访问过*/
    bool* pVisited = (bool*)malloc(pGraph->numOfVertexes * sizeof(bool));
    if(pVisited == nullptr){
        return ERROR;
    }
    int i;
    for(i = 0; i < pGraph->numOfVertexes; i++){
        pVisited[i] = NO;/*初始化，所有节点未访问*/
    }
    for(i = 0; i < pGraph->numOfVertexes; i++){
        if(pVisited[i] == NO){
            /*调用深度优先遍历*/
            DepthFirstSearch(pGraph, i, pVisited, visit);
            /*或调用广度优先遍历BreadthFirstSearch(pGraph, i, pVisited, visit);*/
        }
    }
    free(pVisited);
    return OK;
}

//深度优先遍历
void DepthFirstTraverse(Graph* pGraph, int i, bool pVisited[],void(*visit)()){
    visit(pGraph->vertex[i]); // 访问当前顶点
    pVisited[i] = YES; // 标记为已访问
    int j = FirstAdjacentVertex(pGraph, i); // 获取第一个邻接顶点
    while(j >= 0){/*j >= 0 表示存邻接点*/
        if(pVisited[j] == NO){
            DepthFirstTraverse(pGraph, j, pVisited, visit); // 递归访问
        }
        j = NextAdjacentVertex(pGraph, i, j); // 获取下一个邻接顶点
    }
}

//广度优先遍历
void BreadthFirstTraverse(Graph* pGraph, int i, bool pVisited[], void(*visit)()){
    int j, k;
    Queue queue;
    visit(pGraph->vertex[i]); // 访问当前顶点
    pVisited[i] = YES; // 标记为已访问
    initQueue(&queue); // 初始化队列
    enterQueue(&queue, i); // 入队当前顶点
    /*队列中的定点的邻接点尚未遍历*/
    while(!isQueueEmpty(&queue)){
        leaveQueue(&queue, &j); // 出队
        k = FirstAdjacentVertex(pGraph, j); // 获取第一个邻接顶点
        while(k >= 0){/*k >= 0 表示存邻接点*/
            if(pVisited[k] == NO){
                visit(pGraph->vertex[k]); // 访问当前顶点
                pVisited[k] = YES; // 标记为已访问
                enterQueue(&queue, k); // 入队当前顶点,k的邻接点尚未遍历
            }
            k = NextAdjacentVertex(pGraph, j, k); // 获取下一个邻接顶点
        }/*j的邻接点遍历完毕*/
    }/*队列中的所有顶点遍历完毕*/
}

//深度优先找简单路径(并非最优)
Status FindPath(Graph *pGraph, int u, int v, int path[]){
    int *prev = (int*)malloc(pGraph->numOfVertexes * sizeof(int));
    if(prev == nullptr){
        return ERROR;
    }
    for(int i = 0; i < pGraph->numOfVertexes; i++){
        prev[i] = -1; // 初始化前驱数组,用负值表示没有前驱
    }
    prev[u] = u; // 设置起点的前驱为自己
    //表示u已经被遍历过，prev同时充当visited数组
    DepthFirstSearch(pGraph, u, v, prev);
    /*从u开始搜索v，把遍历过的点的前驱记录在prev中*/
    if(TracePathBack(prev, v, u, path, 
        pGraph->numOfVertexes) == ERROR){
        /*根据prev数组，从v反向追溯到u，得到的路径
        *存储在path数组中。prev数组和path数组的长度
        *都是pGraph->numOfVertexes。
        */
        free(prev);
        return ERROR;
    }
    return OK;
}
//深度优先搜索
void DepthFirstSearch(Graph *pGraph, int u, int v, int *prev){
    static bool founded = false;
    if(u == v){//这个if不能与下面的if合并，因为递归
    /*若合并为 if(u == v || founded)，当首次到达 v 时，
    会立即返回，导致 v 的邻接点未被处理。而原代码在标记 founded 后，
    仍会继续执行当前层的递归（处理 v 的邻接点），
    但更深层的递归会因 founded 为 true 而直接返回。*/
        founded = true;
    }
    if(founded == true){
        return; // 找到目标顶点，停止搜索
    }
    int i = FirstAdjacentVertex(pGraph, u);
    while(i >= 0){//对每个邻接点，做下面操作
        if(prev[i] < 0){//prev[i]<0表示顶点i尚未被访问
            prev[i] = u; // 设置前驱
            DepthFirstSearch(pGraph, i, v, prev); // 递归搜索
        }    
        i = NextAdjacentVertex(pGraph, u, i);
    }/*while结束，即u的所有邻接点处理结束*/
}
//追溯路径
Status TracePathBack(int *prev, int v, int u, int path[], int length){
    if(v == u){
        path[length] = u; // 记录起点
        return OK;
    }
    if(prev[v] < 0){
        return ERROR; // 没有路径
    }
    path[length] = v; // 记录当前顶点
    return TracePathBack(prev, prev[v], u, path, length + 1);
}


//bfs找最优路径
void InitializeSearchData(int *prev, bool *visited, int numOfVertexes) {
    for (int i = 0; i < numOfVertexes; i++) {
        prev[i] = -1;       // 初始化前驱数组为 -1，表示没有前驱
        visited[i] = false; // 初始化访问标记为 false
    }
}
void PerformBFS(Graph *pGraph, int u, int v, int *prev, bool *visited) {
    Queue queue;
    initQueue(&queue); // 初始化队列
    // 起点入队并标记为已访问
    enterQueue(&queue, u);
    visited[u] = true;
    while (!isQueueEmpty(&queue)) {
        int current;
        leaveQueue(&queue, &current); // 出队当前顶点
        // 如果找到目标顶点，停止搜索
        if (current == v) break;
        // 遍历当前顶点的所有邻接顶点
        int i = FirstAdjacentVertex(pGraph, current);
        while (i >= 0) {
            if (!visited[i]) { // 如果邻接顶点未访问
                enterQueue(&queue, i); // 邻接顶点入队
                visited[i] = true;     // 标记为已访问
                prev[i] = current;     // 记录前驱
            }
            i = NextAdjacentVertex(pGraph, current, i); // 获取下一个邻接顶点
        }
    }
    freeQueue(&queue); // 释放队列
}
int TracePath(int *prev, int v, int u, int path[]) {
    int tempPath[NUM_OF_VERTEXES];
    int length = 0;
    // 从目标顶点 v 反向追溯到起点 u
    for (int i = v; i != -1; i = prev[i]) {
        tempPath[length++] = i;
    }
    // 如果路径中没有起点 u，说明没有路径
    if (tempPath[length - 1] != u) {
        return -1; // 返回 -1 表示没有路径
    }
    // 反转路径
    for (int i = 0; i < length; i++) {
        path[i] = tempPath[length - i - 1];
    }
    return length; // 返回路径长度
}
Status BFSFindShortestPath(Graph *pGraph, int u, int v, int path[]) {
    if (u < 0 || u >= pGraph->numOfVertexes || v < 0 || v >= pGraph->numOfVertexes) {
        return ERROR; // 输入顶点编号非法
    }
    int prev[NUM_OF_VERTEXES];
    bool visited[NUM_OF_VERTEXES];
    // 初始化辅助数据结构
    InitializeSearchData(prev, visited, pGraph->numOfVertexes);
    // 执行 BFS
    PerformBFS(pGraph, u, v, prev, visited);
    // 追溯路径
    int pathLength = TracePath(prev, v, u, path);
    if (pathLength == -1) {
        return ERROR; // 没有路径
    }
    return pathLength; // 返回路径长度
}