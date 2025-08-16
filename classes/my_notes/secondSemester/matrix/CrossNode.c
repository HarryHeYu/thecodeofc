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
// typedef struct{
//     int row;
//     int column;
//     Elem value;
// }Triplet;
// typedef struct{
//     Triplet *triplets;
//     int rows;
//     int columns;
//     int nonzeros;//nonzeros：矩阵中非零元素的数量。
// }SparseMatrix;
typedef struct CrossNode {
    int row;
    int column;
    Elem value;
    struct CrossNode *pRight;
    struct CrossNode *pDown;
}CrossNode;
typedef struct {
    CrossNode** rowHeads;
    CrossNode** columnHeads;
    int rows;
    int columns;
    int nonzeros;
}SparseMatrix;

SparseMatrix* SparseMatrixConstruct(int rows,int columns);
//创建一个稀疏矩阵
void SparseMatrixDestruct(SparseMatrix *pMatrix);
//释放稀疏矩阵
Status SparseMatrixAssignElem(SparseMatrix *pMatrix,
                              int row,int column,Elem value);
//给稀疏矩阵指定元素
void SparseMatrixDisplay(SparseMatrix *pMatrix);
//显示稀疏矩阵
void SparseMatrixShowTriplets(SparseMatrix *pMatrix);
//显示稀疏矩阵的三元组
void SparseMatrixTranspose(SparseMatrix *pMatrix);
//转置稀疏矩阵

SparseMatrix* SparseMatrixConstruct(int rows, int columns){
    SparseMatrix* pMatrix = (SparseMatrix*)malloc(sizeof(SparseMatrix));
    if(pMatrix==nullptr) return nullptr;
    pMatrix->rowHeads = (CrossNode**)malloc(rows * sizeof(CrossNode*));
    pMatrix->columnHeads = (CrossNode**)malloc(columns * sizeof(CrossNode*));
    if (pMatrix->rowHeads == nullptr || pMatrix->columnHeads == nullptr) {
        free(pMatrix->rowHeads);
        free(pMatrix->columnHeads);
        free(pMatrix);
        return nullptr;
    }
    // 初始化行头和列头为空头结点
    for (int i = 0; i < rows; i++) {
        pMatrix->rowHeads[i] = (CrossNode*)malloc(sizeof(CrossNode));
        pMatrix->rowHeads[i]->row = i;
        pMatrix->rowHeads[i]->pRight = nullptr;
    }
    for (int j = 0; j < columns; j++) {
        pMatrix->columnHeads[j] = (CrossNode*)malloc(sizeof(CrossNode));
        pMatrix->columnHeads[j]->column = j;
        pMatrix->columnHeads[j]->pDown = nullptr;
    }
    pMatrix->rows = rows;
    pMatrix->columns = columns;
    pMatrix->nonzeros = 0;
    return pMatrix;
}

void SparseMatrixDestruct(SparseMatrix *pMatrix){
    if(pMatrix == nullptr) return;
    // 释放每一行的链表节点
    for (int i = 0; i < pMatrix->rows; i++) {
        CrossNode* current = pMatrix->rowHeads[i];
        while (current != nullptr) {
            CrossNode* temp = current;
            current = current->pRight;
            free(temp);
        }
    }
    // // 释放每一列的链表节点
    // for (int j = 0; j < pMatrix->columns; j++) {
    //     CrossNode* current = pMatrix->columnHeads[j];
    //     while (current != nullptr) {
    //         CrossNode* temp = current;
    //         current = current->pDown;
    //         free(temp);
    //     }
    // }
    free(pMatrix->rowHeads);
    free(pMatrix->columnHeads);
    free(pMatrix);
}

Status SparseMatrixAssignElem(SparseMatrix* pMatrix, int row, int column, Elem value){
    if(row < 0 || row >= pMatrix->rows || column < 0 || column >= pMatrix->columns) {
        return ERROR;
    }
    CrossNode* prev = nullptr;
    CrossNode* curr = pMatrix->rowHeads[row];
    //插入到行链表中
    CrossNode* rowHead = pMatrix->rowHeads[row];
    CrossNode* prev = rowHead;
    CrossNode* curr = rowHead->pRight;
    while(curr != nullptr && curr->column < column){
        prev = curr;
        curr = curr->pRight;
    }
    if(curr != nullptr && curr->column == column) {
        if(value == 0) { // 删除节点
            if(prev) prev->pRight = curr->pRight;
            else pMatrix->rowHeads[row] = curr->pRight;
            free(curr);
            pMatrix->nonzeros--;
/*如果用户试图将某个位置的值设为 0，应该直接忽略操作或从矩阵中移除该节点，而不是新增一个节点。
稀疏矩阵的特性是只存储非零元素。
如果原节点不存在就不进行其他操作*/
        } else {
            curr->value = value;
        }
        return OK;
    }
    if(value == 0) return OK; // 零值不插入
    CrossNode* newNode = (CrossNode*)malloc(sizeof(CrossNode));
    if(newNode == nullptr) return ERROR;
    newNode->row = row;
    newNode->column = column;
    newNode->value = value;
    newNode->pRight = curr;
    if(prev) prev->pRight = newNode;
    else pMatrix->rowHeads[row] = newNode;

    // 插入列链表
    prev = nullptr;
    curr = pMatrix->columnHeads[column];
    while(curr != nullptr && curr->row < row) {
        prev = curr;
        curr = curr->pDown;
    }
    newNode->pDown = curr;
    if(prev) prev->pDown = newNode;
    else pMatrix->columnHeads[column] = newNode;

    pMatrix->nonzeros++;
    return OK;
}

void SparseMatrixTranspose(SparseMatrix* pMatrix) {
    // 创建一个新的稀疏矩阵用于存储转置后的结果
    SparseMatrix* transposed = SparseMatrixConstruct(pMatrix->columns, pMatrix->rows);

    // 初始化列的非零元素计数数组
    int* colCount = (int*)calloc(pMatrix->columns, sizeof(int));
    if (colCount == nullptr) return; // 如果内存分配失败，直接返回

    // 遍历原矩阵的行链表，统计每列的非零元素数量
    for (int i = 0; i < pMatrix->rows; i++) {
        CrossNode* current = pMatrix->rowHeads[i];
        while (current != nullptr) {
            colCount[current->column]++;
            current = current->pRight;
        }
    }

    // 初始化列链表头节点指针数组
    CrossNode** colTail = (CrossNode**)malloc(pMatrix->columns * sizeof(CrossNode*));
    if (colTail == nullptr) {
        free(colCount);
        return;
    }
    for (int i = 0; i < pMatrix->columns; i++) {
        colTail[i] = transposed->columnHeads[i]; // 初始化为列头节点
    }

    // 遍历原矩阵，按列插入到新的稀疏矩阵中
    for (int i = 0; i < pMatrix->rows; i++) {
        CrossNode* current = pMatrix->rowHeads[i];
        while (current != nullptr) {
            // 创建新的节点
            CrossNode* newNode = (CrossNode*)malloc(sizeof(CrossNode));
            if (newNode == nullptr) {
                free(colCount);
                free(colTail);
                return;
            }
            newNode->row = current->column; // 转置后的行是原列
            newNode->column = current->row; // 转置后的列是原行
            newNode->value = current->value;
            newNode->pRight = nullptr;
            newNode->pDown = nullptr;

            // 插入到转置矩阵的列链表中
            colTail[current->column]->pDown = newNode;
            colTail[current->column] = newNode;

            // 更新转置矩阵的行链表
            if (transposed->rowHeads[newNode->row] == nullptr) {
                transposed->rowHeads[newNode->row] = newNode;
            } else {
                CrossNode* rowCurrent = transposed->rowHeads[newNode->row];
                while (rowCurrent->pRight != nullptr) {
                    rowCurrent = rowCurrent->pRight;
                }
                rowCurrent->pRight = newNode;
            }

            current = current->pRight;
        }
    }

    // 释放原矩阵的内存
    SparseMatrixDestruct(pMatrix);

    // 将转置矩阵赋值给原矩阵
    pMatrix->rowHeads = transposed->rowHeads;
    pMatrix->columnHeads = transposed->columnHeads;
    pMatrix->rows = transposed->rows;
    pMatrix->columns = transposed->columns;
    pMatrix->nonzeros = transposed->nonzeros;

    // 释放临时列尾指针数组和列计数数组
    free(colTail);
    free(colCount);

    // 释放转置矩阵的包装结构（行头和列头已转移给原矩阵）
    free(transposed);
}
// int main(){
//     SparseMatrix *pMatrix;
//     pMatrix=SparseMatrixConstruct(M,N);
//     if(pMatrix==nullptr) return 1;
//     printf("This is the initial matrix:\n");
//     SparseMatrixAssignElem(pMatrix,0,1,2);
//     SparseMatrixAssignElem(pMatrix,3,0,7);
//     SparseMatrixAssignElem(pMatrix,3,5,3);
//     SparseMatrixAssignElem(pMatrix,4,4,5);
//     SparseMatrixDisplay(pMatrix);
//     printf("The triplets (row,column,value) are:\n");
//     SparseMatrixShowTriplets(pMatrix);
//     // 测试转置函数
//     printf("\nTransposing the matrix...\n");
//     SparseMatrixTranspose(pMatrix);

//     // 显示转置后的矩阵
//     printf("This is the transposed matrix:\n");
//     SparseMatrixDisplay(pMatrix);
//     printf("The triplets (row, column, value) after transposing are:\n");
//     SparseMatrixShowTriplets(pMatrix);

//     // 销毁矩阵
//     SparseMatrixDestruct(pMatrix);

//     return 0;
// }

