#include <stdio.h>
 
// 你的代码写在 TODO和END OF TODO之间。
// 请勿修改这对标签外的任何代码！！！
 
 
/**
 * @brief 读取字符串中的整数
 * 
 * @param s 字符串
 * @return int 字符串中嵌入的整数。如果没有，则返回0。
 */
int get_int(char *s) {
    // 在此函数中，需要用到字符输出
    // TODO
    int num = 0;
    for(int i = 0; s[i] != '\0'; i++){
        if(s[i] >= '0' && s[i] <= '9'){
            num = (s[i] - '0') + num * 10;//需要写(s[i] - '0')，因为他是字符而不是整数，需要转换一下
        }
    }
    return num;
    // END OF TODO
}
 
char S[1000];
int main() {
    scanf("%s", S);
    printf("%d\n", get_int(S));
    return 0;
}