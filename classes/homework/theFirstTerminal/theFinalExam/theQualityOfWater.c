#include <stdio.h>
 
// 你的代码写在 TODO和END OF TODO之间。
// 请勿修改这对标签外的任何代码！！！
 
/**
 * @brief 循环输入n个正整数表示的水质，输入阈值，统计大于等于阈值的个数
 * 
 * @param n 数值的个数
 * @return int 统计值
 */
int qualified(int n) {
    // 在此函数中，需要用到整数输入
    // TODO
    int T;
    scanf("%d",&T);
    int value[100];
    for(int i = 0; i < n; i++){
        scanf("%d",&value[i]);
    }
    int count = 0;
    for(int i = 0; i < n; i++){
        if(value[i] >= T){
            count++;
        }
    }
    return count;
    // END OF TODO
}
 
int main() {
    int N;
    scanf("%d", &N);
 
    printf("%d\n", qualified(N));
    return 0;
}