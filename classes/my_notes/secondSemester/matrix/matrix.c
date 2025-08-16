#include<stdio.h>
#include<stdlib.h>
#define C24_VERSION 202311L
#if __STDC_VERSION__ < C24_VERSION
    #define nullptr NULL
    typedef enum{false=0,true=1}bool;
#endif // __STDC_VERSION
#define M 6
#define N 6
typedef enum{ERROR=0,OK=1}Status;
typedef int Elem;
typedef struct{
    int row;
    int column;
    Elem value;
}Triplet;
typedef struct{
    Triplet *triplets;
    int rows;
    int columns;
    int nonzeros;//nonzeros：矩阵中非零元素的数量。
}SparseMatrix;
SparseMatrix* SparseMatrixConstruct(int rows,int columns);
void SparseMatrixDestruct(SparseMatrix *pMatrix);
Status SparseMatrixAssignElem(SparseMatrix *pMatrix,
                              int row,int column,Elem value);
void SparseMatrixDisplay(SparseMatrix *pMatrix);
void SparseMatrixShowTriplets(SparseMatrix *pMatrix);
void SparseMatrixTranspose(SparseMatrix *pMatrix);
/* ASSIGNMENT 1 TO DO:
 * Implement SparseMatrixTranspose
 * using the quick transpose algorithm.
 *
 * ASSIGNMENT 2 TO DO:
 * Implement SparseMatrix with the orthogonal linked list.
 * typedef struct CrossNode {
 *     int row;
 *     int column;
 *     Elem value;
 *     struct CrossNode *pRight;
 *     struct CrossNode *pDown;
 * }CrossNode;
 * typedef struct {
 *     CrossNode* rowHeads[];
 *     CrossNode* columnHeads[];
 *     int rows;
 *     int columns;
 *     int nonzeros;
 * }SparseMatrix;
 */

SparseMatrix* SparseMatrixConstruct(int rows,int columns){
    SparseMatrix *pMatrix;
    pMatrix=(SparseMatrix*)malloc(sizeof(SparseMatrix));
    if(pMatrix==nullptr) return nullptr;
    pMatrix->triplets=(Triplet*)
                      malloc(rows*columns*sizeof(Triplet));
    if(pMatrix->triplets==nullptr){
        free(pMatrix);
        return nullptr;
    }
    pMatrix->rows=rows;
    pMatrix->columns=columns;
    pMatrix->nonzeros=0;
    return pMatrix;
}
void SparseMatrixDestruct(SparseMatrix *pMatrix){
    free(pMatrix->triplets);
    free(pMatrix);
}
Status SparseMatrixAssignElem(SparseMatrix *pMatrix,
                              int row,int column,Elem value){
    int i;
    for(i=0;i<pMatrix->nonzeros;i++){
        if(row==pMatrix->triplets[i].row &&
           column==pMatrix->triplets[i].column){
            pMatrix->triplets[i].value=value;
            return OK;
        }
    }
    if(pMatrix->nonzeros>=pMatrix->rows*pMatrix->columns){
        return ERROR;
    }
    i=pMatrix->nonzeros-1;
    while(i>=0){
        if(row<pMatrix->triplets[i].row ||
           (row==pMatrix->triplets[i].row &&
            column<pMatrix->triplets[i].column)){
            pMatrix->triplets[i+1]=pMatrix->triplets[i];
            i--;
        }
        else{
            break;
        }
    }
    pMatrix->triplets[i+1].row=row;
    pMatrix->triplets[i+1].column=column;
    pMatrix->triplets[i+1].value=value;
    pMatrix->nonzeros++;
    return OK;
/*从后向前遍历：
从当前最后一个非零元素的索引 i = nonzeros - 1 开始。
如果当前三元组的 row 大于目标 row，或者 row 相等但 column 大于目标 column，则将当前三元组向后移动一位。
找到插入位置：
如果当前三元组的 row 小于目标 row，或者 row 相等且 column 小于目标 column，则停止移动，插入新三元组。
插入新三元组：
在位置 i + 1 插入新的三元组 (row, column, value)。
更新非零元素计数器：
非零元素数量 nonzeros 增加 1。*/
}
void SparseMatrixDisplay(SparseMatrix *pMatrix){
    int i,j,k;
    k=0;
    for(i=0;i<pMatrix->rows;i++){
        for(j=0;j<pMatrix->columns;j++){
            if(k<pMatrix->nonzeros &&
               i==pMatrix->triplets[k].row &&
               j==pMatrix->triplets[k].column){
                printf("%3d",pMatrix->triplets[k].value);
                k++;
            }
            else{
                printf("  0");
            }
        }
        printf("\n");
    }
}

void SparseMatrixShowTriplets(SparseMatrix *pMatrix){
    int i,n;
    n=pMatrix->nonzeros;
    for(i=0;i<n;i++){
        printf("(%d,%d,%d)\n",
               pMatrix->triplets[i].row,
               pMatrix->triplets[i].column,
               pMatrix->triplets[i].value);
    }
}
void SparseMatrixTranspose(SparseMatrix *pMatrix){
    // ASSIGNMENT TO DO
    Triplet *pTransposed = (Triplet*)malloc(pMatrix->nonzeros
                                        * sizeof(Triplet));
    if(pTransposed == nullptr) return;
    int *colCount = (int*)malloc(pMatrix->columns * sizeof(int));
    int *colStart = (int*)malloc(pMatrix->columns * sizeof(int));
    if(colCount == nullptr || colStart == nullptr) return;
    for (int i = 0; i < pMatrix->columns; i++) {
        colCount[i] = 0;
    }
    for(int i = 0; i < pMatrix->nonzeros; i++){
        colCount[pMatrix->triplets[i].column]++;
    }
    colStart[0] = 0;
    for(int i  = 1; i < pMatrix->columns; i++){
        colStart[i] = colCount[i - 1] + colStart[i - 1]; 
    }
    for(int i = 0; i < pMatrix->nonzeros; i++){
        int col = pMatrix->triplets[i].column;
        int pos = colStart[col];
        pTransposed[pos].row = col;
        pTransposed[pos].column = pMatrix->triplets[i].row;
        pTransposed[pos].value = pMatrix->triplets[i].value;
        colStart[col]++;
    }
    free(pMatrix->triplets);
    pMatrix->triplets = pTransposed;
    int temp = pMatrix->rows;
    pMatrix->rows = pMatrix->columns;
    pMatrix->columns = temp;
    free(colCount);
}

int main(){
    SparseMatrix *pMatrix;
    pMatrix=SparseMatrixConstruct(M,N);
    if(pMatrix==nullptr) return 1;
    printf("This is the initial matrix:\n");
    SparseMatrixAssignElem(pMatrix,0,1,2);
    SparseMatrixAssignElem(pMatrix,3,0,7);
    SparseMatrixAssignElem(pMatrix,3,5,3);
    SparseMatrixAssignElem(pMatrix,4,4,5);
    SparseMatrixDisplay(pMatrix);
    printf("The triplets (row,column,value) are:\n");
    SparseMatrixShowTriplets(pMatrix);
    // 测试转置函数
    printf("\nTransposing the matrix...\n");
    SparseMatrixTranspose(pMatrix);

    // 显示转置后的矩阵
    printf("This is the transposed matrix:\n");
    SparseMatrixDisplay(pMatrix);
    printf("The triplets (row, column, value) after transposing are:\n");
    SparseMatrixShowTriplets(pMatrix);

    // 销毁矩阵
    SparseMatrixDestruct(pMatrix);

    return 0;
}

