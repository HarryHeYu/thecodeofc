#include <stdio.h>
 
// 你的代码写在 TODO和END OF TODO之间。
// 请勿修改这对标签外的任何代码！！！
 
/**
 * @brief 循环输入n个非负数，求这n个数中3的倍数之和
 * 
 * @param n 数值的个数
 * @return int 3的倍数之和
 */
int summary(int n) {
    // 在此函数中，需要用到整数输入
    // TODO
    int sum = 0, num;
    for(int i = 0; i < n; i++){
        scanf("%d",&num);
        if(num % 3 ==0){
            sum += num;
        }
    }
    return sum;
    // END OF TODO
}
 
int main() {
    int N;
    scanf("%d", &N);
 
    printf("%d\n", summary(N));
    return 0;
}