#include <stdio.h>
 
// 你的代码写在 TODO和END OF TODO之间。
// 请勿修改这对标签外的任何代码！！
 
 
/**
 * @brief 求x的所有真因子之和
 * 
 * @param x 正整数
 * @return int 真因子之和
 */
int sum_of_factor(int x) {
    // TODO
    int sum = 0;
    for(int i = 1; i < x; i++){
        if(x % i == 0){
            sum += i;
        }
    }
    return sum;
    // END OF TODO
}
 
/**
 * @brief 判断是否是亲密数对
 * 
 * @param m 数1
 * @param n 数2
 */
void is_intimate(int m, int n) {
    // TODO
    // 这个程序要用到输出
    int sum_m = sum_of_factor(m);
    int sum_n = sum_of_factor(n);
    if(sum_m == n && sum_n == m){
        printf("%d %d\n",n,m);
    }else{
        printf("not\n");
    }
    // END OF TODO
}
 
 
int main() {
    int m, n;
    scanf("%d%d", &m, &n);
 
    is_intimate(m, n);
    
    return 0;
}