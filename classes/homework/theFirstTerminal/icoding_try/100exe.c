#include <stdio.h>
// 你的代码写在 TODO和END OF TODO之间。
// 请勿修改这对标签外的任何代码！！！

/*
@brief 逆序输出字符串
@
@param s 字符串
*/
void puts_rev(char *s) {
  // 此函数要用到字符输出
  // TODO
  int len = 0;
    for(int i = 0; s[i] != '\0'; i++){
        len++;
    }
    len--;
    for(int i = len; i >= 0; i--){
        printf("%c",s[i]);
    }
  // END OF TODO
}

char S[1000];
int main() {
  scanf("%s", S); // 输入时，用~代替空格
  puts_rev(S);

  return 0;
}