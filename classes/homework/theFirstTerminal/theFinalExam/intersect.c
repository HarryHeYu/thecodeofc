#include <stdio.h>
// 你的代码写在 TODO和END OF TODO之间。
// 请勿修改这对标签外的任何代码！！！ 
/**
 * @brief 求集合的交集
 * 
 * @param A 输入集合
 * @param B 输入集合
 * @param C A和B的交集
 */
void intersect(int *A, int *B, int *C) {
    // TODO
    int index =  0;
    for(int i = 0; A[i] != -1; i++ ){
        for(int j = 0; B[j] != -1; j++){
            if(A[i] == B[j]){
                C[index] = B[j];
                index++;
                break;//其实不需要break，因为给的集合是不含重复元素的
            }
        }
    }
    C[index] = -1;
    // END OF TODO
}
/**
 * @brief 输入以-1结尾的正整数集合（数组）
 * 
 * @param S 集合
 */
void input_set(int *S) {
    int i = 0, v;
    do {
        scanf("%d", &v);
        S[i++] = v;
    } while (v != -1);
}
// 集合最大元素个数
#define MAXN    20
int main() {
    int A[MAXN], B[MAXN], C[MAXN];
    input_set(A);
    input_set(B);
    intersect(A, B, C);
    for (int i = 0; C[i] != -1; ++i)
        printf("%d ", C[i]);
    printf("\n");
    return 0;
}