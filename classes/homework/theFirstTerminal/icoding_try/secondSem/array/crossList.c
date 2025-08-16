/*将一个十字链表存储在一个数组里*/

#include <stdio.h>
#include <stdlib.h>
#include "crosslist.h"

int init_cross_list(PCrossList L, const ElemType *A, int m,int n){
    if(!L || !A || m <= 0 || n <= 0) return 0; // 如果传入指针为空或行列非正，直接返回0
    L->rows = m; L->cols = n; L->nums = 0; //记录矩阵行数、列数。非零元素个数
    OLNode **rowtail = (OLNode**)malloc(sizeof(OLNode*)*(m + 1));
    OLNode **coltail = (OLNode**)malloc(sizeof(OLNode*)*(n + 1));//下标从1开始
    if(!rowtail || !coltail){
        free(rowtail);
        free(coltail);
        return 0; // 如果内存分配失败，释放已分配的内存并返回0
    }
    //初始化辅助尾指针数组全部为NULL(0号不用)
    for(int i = 0; i <= m; i++) rowtail[i] = NULL;
    for(int i = 0; i <= n; i++) coltail[i] = NULL;
    //遍历数组A，将非零元素存入十字链表
    for(int idx = 0; idx < m*n; idx++){
        if(A[idx] == 0) continue; // 如果元素为0，跳过,只处理非零元
        int i = idx / n + 1, j = idx % n + 1; // 计算行列号
        OLNode * node = (OLNode*)malloc(sizeof(OLNode));//为当前非零元分配一个新结点
        node->row = i; node->col = j; node->value = A[idx];
        node->right = NULL; node->down = NULL; //填写行号、列号、值并初始化指针
        if(!L->rowhead[i]) L->rowhead[i] = node;
            else rowtail[i]->right = node;
        rowtail[i] = node;//行链表尾插法：如果当前行还没有头结点，设为头结点，否则加到尾部，然后更新尾指针。
        if(!L->colhead[j])L->colhead[j] = node;
            else coltail[j]->down = node;
        coltail[j] = node;//列链表尾插法：如果当前列还没有头结点，设为头结点，否则加到尾部，然后更新尾指针。
        L->nums++; //非零元素个数加1
    }
    free(rowtail);
    free(coltail);
    return L->nums;// 返回非零元素个数
}


int del_cross_list(PCrossList L, ElemType k){
    if(!L || k == 0) return 0; // 链表未初始化或k为0，直接返回0
    int count = 0; // 统计删除的节点数
    for(int i = 1; i <= L->rows; ++i){
        OLNode *prev = NULL, *curr = L->rowhead[i];
        while(curr){
            if(curr->value == k){// 外层遍历每一行，内层遍历该行所有结点，查找值为k的结点
                OLNode *cp = NULL, *cc = L->colhead[curr->col];// cp是前驱指针，cc是当前指针
                while(cc && cc != curr) cp = cc; cc = cc->down; // 在列链表中找到当前结点
                if(cc == curr){
                    if(!cp) L->colhead[curr->col] = cc->down;
                    else cp->down = cc->down;// 如果是列头，则更新头指针，否则将前驱结点的下一个指针指向当前结点的下一个结点(断开down链)
                }
                if(!prev) L->rowhead[i] = curr->right;
                    else prev->right = curr->right; //如果是行头，则更新头指针，否则将前驱结点的右指针指向当前结点的右结点(断开right链)
                OLNode *to_free = curr;
                curr = curr->right;
                free(to_free);// 释放当前结点内存，并将curr移到下一个结点
                L->nums--; count++;// 非零元计数器和删除计数器均减一/加一
            } else {
                prev = curr;
                curr = curr->right;
            }
        }
        return count; // 如果当前值不是k，则正常遍历到下一个结点。最终返回删除的结点数量。
    }
}