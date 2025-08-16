#include <stdio.h>
 
// 你的代码写在 TODO和END OF TODO之间。
// 请勿修改这对标签外的任何代码！！！
 
 
/**
 * @brief 输入一个字符串S，将S中出现的O(大写字母O)替换为P(大写字母P)后重新输出字符串。
 *        本题不能使用C库函数，如果使用库函数，则视为0分！
 * @param s 字符串
 */
void puts_replace(char *s) {
    // 此函数要用到字符输出
    // TODO
    while(*s){
        if(*s == 'O'){
            *s = 'P';
        }
        putchar(*s);
        s++;
    }
    // END OF TODO
}
 
char S[1000];
int main() {
    scanf("%s", S); // 输入时，用~代替空格
    puts_replace(S);
    
    return 0;
}