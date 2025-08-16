#include <stdio.h>
#include "graph.h" //请勿删除，否则检查不通过

bool insert_vertex(ListGraph *G, VertexType v){
    if(G->vexnum >= MAX_VERTEX_NUM) return false;//图已满
    if(locate_vertex(G,v) != -1) return false;//顶点已存在
    //插入新顶点
    G->vertex[G->vexnum].data = v;
    G->vertex[G->vexnum].firstarc = NULL;
    G->vexnum++;
    return true;
}


bool insert_arc(ListGraph *G, VertexType v, VertexType w){
    int vi = locate_vertex(G,v);
    int wi = locate_vertex(G,w);
    if(vi == -1 || wi == -1) return false;//端点不存在

    //检查边是否已存在
    ArcNode *p = G->vertex[vi].firstarc;
    while(p){
        if(p->adjvex == wi) return false;
        p = p->nextarc;
    }

    //插入边v->w
    ArcNode* newArc = (ArcNode*)malloc(sizeof(ArcNode));
    if(!newArc) return false;
    newArc->adjvex = wi;
    newArc->info = NULL;
    newArc->nextarc = G->vertex[vi].firstarc;
    G->vertex[vi].firstarc = newArc;
    G->arcnum++;

    //若为无向图，还需插入w->v
    if(G->type == UDG){
        p = G->vertex[wi].firstarc;
        while(p){
            if(p->adjvex == vi) return false;
            p = p->nextarc;
        }
        ArcNode* newArc2 = (ArcNode*)malloc(sizeof(ArcNode));
        if(!newArc2) return false;
        newArc2->adjvex = vi;
        newArc2->nextarc = G->vertex[wi].firstarc;
        G->vertex[wi].firstarc = newArc2;
    }
    return true;
}


bool del_vertex(ListGraph *G, VertexType v){
    int i;
    int idx = locate_vertex(G,v);
    if(idx == -1) return false;//顶点不存在
    //1.删除所有指向该顶点的边
    for(i = 0; i < G->vexnum; ++i){
        if(i == idx) continue;
        ArcNode *prev = NULL, *p = G->vertex[i].firstarc;
        while(p){
            if(p->adjvex == idx){
                //删除该边
                if(prev) prev->nextarc = p->nextarc;
                else G->vertex[i].firstarc = p->nextarc;
                ArcNode *tmp = p;
                p = p->nextarc;
                free(tmp);
                G->arcnum--;
                //不return，继续查找后续边
            } else{
                prev = p;
                p = p->nextarc;
            }
        }
    }

    //2.删除该顶点出发的所有边
    ArcNode* p = G->vertex[idx].firstarc;
    while(p){
        ArcNode* tmp = p;
        p = p->nextarc;
        free(tmp);
        G->arcnum--;
    }

    //3.删除该顶点,数组后面的顶点前移
    for(i = idx +1; i < G->vexnum; ++i){
        G->vertex[i-1] = G->vertex[i]; 
    }
    G->vexnum--;

    //4.更新所有边的adjvex(因数组下标变化，可能需要-1)
    for(i = 0; i < G->vexnum; ++i){
        ArcNode* p = G->vertex[i].firstarc;
        while(p){
            if(p->adjvex > idx) p->adjvex--;
            p = p->nextarc;
        }
    }
    return true;
}

bool matrix_insert_vertex(MatrixGraph *G, VertexType v){
    if(!G) return false;
    if(matrix_locate_vertex(G,v) != -1) return false; //顶点已存在
    if(G->vexnum >= MAX_VERTEX_NUM ) return false; //图已满
    //将顶点放到数组末尾
    G->vertex[G->vexnum] = v;
    //初始化与该顶点相关的邻接矩阵的行、列
    for(int i = 0; i <= G->vexnum; ++i){
        G->arcs[i][G->vexnum] = 0; //新顶点的列
        G->arcs[G->vexnum][i] = 0; //新顶点的行
    }
    //更新顶点数
    G->vexnum++;
    return true;
}

bool matrix_insert_arc(MatrixGraph *G, VertexType v, VertexType w){
    if(!G) return false;
    int vi = matrix_locate_vertex(G,v);
    int wi = matrix_locate_vertex(G,w);
    //任意顶点不存在则错误
    if(vi == -1 || wi == -1) return false;
    //检查边是否已存在
    if(G->arcs[vi][wi] == 1) return false; //边已存在

    //对有向图在arc[i][j]标记边，对无向图还需要对称标记
    G->arcs[vi][wi] = 1;
    if(G->type == UDG){
        G->arcs[wi][vi] = 1; //无向图对称标记
    }
    //更新边数
    G->arcnum++;  
    return true;
}