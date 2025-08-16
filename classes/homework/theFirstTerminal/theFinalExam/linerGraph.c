#include <stdio.h>
 
// 你的代码写在 TODO和END OF TODO之间。
// 请勿修改这对标签外的任何代码！！！
 
 
// 线性表的最大容量
#define CAPACITY  128
 
typedef struct {
    int len; // 线性表实际存储的元素个数
    int data[CAPACITY]; // 数组形式的存储空间。数据存储在该数组前len个单元中，后面的单元空置
} list;
 
 
/**
 * @brief 判断线性表是否是非递减序列
 * 
 * @param L 线性表指针
 * @return int* 如果是非递减序列，返回NULL；否则，返回第一个违例的前项元素地址
 */
int* is_increasing(list *L) {
    // TODO
    int now = L->data[0];
    for(int i = 0; i < L->len - 1; i++){
        if(L->data[i] > L->data[i+1]){
            return &L->data[i];
        }
    }
    return NULL;
    // END OF TODO
}
 
 
int main() {
    // 定义线性表及其指针
    list l, *L = &l;
 
    // 输入线性表长度
    scanf("%d", &L->len);
    int* p = L->data;
    // 输出线性表元素
    for (int i = 0; i < L->len; ++i, ++p)
        scanf("%d", p);
 
    // 判断
    p = is_increasing(L);
 
    // 输出结果
    if (p == NULL) {
        printf("yes\n");
    } else {
        printf("%d %d\n", *p, *(p+1));
    }
    
    return 0;
}