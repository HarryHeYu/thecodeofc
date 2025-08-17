/***************************
 *  <name:下三角矩阵
 *  <author: He YU Hao
 *  <date:2025.4.19
*****************************/
#include <stdio.h>
#include <stdlib.h>
//特殊矩阵的压缩状态（下三角矩阵示例）
void lowerTriangularMatrixCompression(){
    int n, i, j, k = 0;
    printf("请输入下三角矩阵的大小(n x n):");
    scanf("%d",&n);
    int *compressed = (int)malloc((n * (n + 1) / 2) * sizeof(int));
    printf("请输入下三角矩阵的元素: \n");
    for (i = 0; i < n; i++) {
        for (j = 0; j <= i; j++) {
            scanf("%d", &compressed[k++]);
        }
    }

    printf("压缩存储的数组为: ");
    for (i = 0; i < n * (n + 1) / 2; i++) {
        printf("%d ", compressed[i]);
    }
    printf("\n");
    free(compressed);
}

int main() {
    printf("特殊矩阵压缩存储示例：\n");
    lowerTriangularMatrixCompression();
    return 0;
}
