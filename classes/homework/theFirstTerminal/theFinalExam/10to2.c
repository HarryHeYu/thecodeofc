#include <stdio.h>
 
// 你的代码写在 TODO和END OF TODO之间。
// 请勿修改这对标签外的任何代码！！！
 
/**
 * @brief 将十进制整数转换为二进制数，并显示
 * 
 * @param d 十进制正整数
 */
void dtob(int d) {
    // 此函数要用到输出
    // TODO
    int a[1000];
    int b = 0;
    if(d == 0){
        printf("0");
        return;
    }
    while(d > 0){
        a[b] = d % 2;
        d = d / 2;
        b++;
    }
    for(int i = b - 1; i >= 0; i--){
        printf("%d",a[i]);
    }
    // END OF TODO
}
 
int main() {
    int d;
    scanf("%d", &d);
 
    dtob(d);
 
    return 0;
}