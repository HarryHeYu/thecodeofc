#include <stdio.h>
 
// 你的代码写在 TODO和END OF TODO之间。
// 请勿修改这对标签外的任何代码！！！
 
/**
 * @brief 统计字符c在s中出现的次数
 * 
 * @param s 字符串
 * @param c 字符
 * @return int 字符c的出现字数
 */
int repeat_char(char *s, char c) {
    // TODO
    int count = 0;
    while(*s){
        if(*s == c){
            count++;
        }
        s++;
    }
    return count;
    // END OF TODO
}
 
char S[1000];
int main() {
    char c;
    c = getchar();
    scanf("%s", S);
    printf("%d\n", repeat_char(S, c));
    
    return 0;
}