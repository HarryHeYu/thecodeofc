// #include<stdio.h>

// int main(){
//     long long a,b;
//     printf("请输入价格和你有的现金\n");
//     scanf("%lld%lld",&a,&b);
//     if(a > b){
//         long long temp = a - b;
//         if(temp % 100 == 0){
//             printf("需要%lld张100元\n" ,temp/100);
//         }
//         else{
//             printf("需要%lld张100元\n",temp/100 + 1);
//         }
//     }
//     else{
//         printf("不用提款机\n");
//     }
//     return 0;
// }
#include<stdio.h>

int main(){
    long long a, b;
    printf("请输入价格和你有的现金\n");
    scanf("%lld%lld", &a, &b);

    if (a > b) { // 只有当价格大于现金时，才需要取款
        long long temp = a - b;
        if (temp % 100 == 0) {
            printf("需要%lld张100元\n", temp / 100);
        } else {
            printf("需要%lld张100元\n", (temp + 99) / 100); // 向上取整
        }
    } else {
        printf("0\n"); // 如果价格不大于现金，不需要取款
    }
    return 0;
}