#include <stdio.h>
 
// 你的代码写在 TODO和END OF TODO之间。
// 请勿修改这对标签外的任何代码！！！
 
 
/**
 * @brief 给定两个正整数a,b 求出区间[a,b]内的质数之和
 * 
 * @param a 区间下界
 * @param b 区间上界
 * @return int 区间内的质数之和
 */
int prime_sum(int a,int b) {
    // TODO
    int sum = 0;
    for(int i = a; i <= b; i++){
        if(i <= 1)continue;//1和0不是质数
        if(i == 2){
        sum += 2;//2是质数
        continue;
        }
        if(i % 2 == 0)continue;//说明i是偶数
        int is_prime = 1;//假设i是质数
        for(int j = 3; j * j <= i; j += 2){
            if(i % j == 0){
                is_prime = 0;//如果i能被j整除，则i不是质数
                break;
            }
        }
        if(is_prime){
            sum +=i;//i是质数就加到sum中
        }
    }
    return sum;
    // END OF TODO
}
 
 
int main() {
    int a,b;
    scanf("%d", &a);
    scanf("%d", &b);
    
    printf("%d\n", prime_sum(a,b));
 
    return 0;
}