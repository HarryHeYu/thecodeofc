#include <stdio.h>
#include <stdlib.h>
#include "tsmatrix.h"

bool add_matrix(const TSMatrix *pM, const TSMatrix *pN, TSMatrix *pQ){
    if(!pM || !pN || pM->m != pN->m || pM->n != pN->n ){
            return false;
        }
    pQ->m = pM->m;
    pQ->n = pM->n;
    pQ->len = 0; // 初始化非零元个数为 0
    int i = 0, j = 0, k = 0;
    while(i < pM->len && j < pN->len){
        int rowM = pM->data[i].i;
        int colM = pM->data[i].j;
        int rowN = pN->data[j].i;
        int colN = pN->data[j].j;
        //合并相同位置的元素
        if(rowM < rowN || (rowM == rowN && colM < colN)){
            if(k >= MAXSIZE) return false;
            pQ->data[k++] = pM->data[i++];
        } else if(rowM > rowN || (rowM == rowN && colM > colN)){
            if(k >= MAXSIZE) return false;
            pQ->data[k++] = pN->data[j++]; 
        } else if(rowM == rowN && colM == colN){
            ElemType sum = pM->data[i].e + pN->data[j].e;
            if(sum != 0){
                if(k >= MAXSIZE) return false;
                pQ->data[k].i = rowM;
                pQ->data[k].j = colM;
                pQ->data[k++].e = sum;
            }
            i++;
            j++;
        }
    }
    //处理剩余部分
    while(i < pM->len){
        if(k >= MAXSIZE) return false;
        pQ->data[k++] = pM->data[i++];
    }
    while(j < pN->len){
        if(k >= MAXSIZE) return false;
        pQ->data[k++] = pN->data[j++];
    }
    pQ->m = pM->m;
    pQ->n = pM->n;
    pQ->len = k;
    return true;
}

