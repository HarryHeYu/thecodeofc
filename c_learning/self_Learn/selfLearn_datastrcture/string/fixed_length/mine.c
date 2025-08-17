/***************************
 *  <name：定长顺序串
 *  <author: He YU Hao
 *  <date:2025.4.1
*****************************/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAXSIZE 20 //字符串的最大长度

typedef enum { ERROR = 0, OK = 1 }Status;

typedef struct {
    char ch[MAXSIZE]; //用于储存字符串的数组
    int length;//字符串的长度
}String;

//生成一个值等于chars的串S
Status StrAssign(String *T, char *chars){
    if(strlen(chars) > MAXSIZE) {
        return ERROR;
    }else {
        T->length = strlen(chars);
        for(int i = 0; i < T->length; i++) T->ch[i] = chars[i];
        return OK;
    }
}

//在字符串的第pos个字符之前插入串T
Status StrInsert(String* S, int pos, String T){
    if(pos < 1 || pos > S->length + 1) return ERROR;
    int newLength = S->length + T.length;
    if(newLength <= MAXSIZE){
        //空间足够，完整插入
        for(int i = S->length; i >= pos; i--){
            S->ch[i + T.length - 1] = S->ch[i - 1];
        }
        for(int i = pos; i < pos + T.length; i++){
            S->ch[i - 1] = T.ch[i - pos];
        }
        S->length = newLength;
        return OK;
    }else {
        //空间不足，部分插入
        int availableSpace = MAXSIZE - S->length; // 计算可用空间
        int copyLen = (availableSpace > 0) ? availableSpace : 0; //取可用空间的值，确保不为负数
        if (availableSpace == 0) {
            return ERROR; // 没有可用空间，直接返回错误
        }
        //移动原字符串以腾出空间
        for(int i = S->length - 1; i >= pos - 1; i--){
            if(i + copyLen < MAXSIZE){
                S->ch[i + copyLen] = S->ch[i];
            }
        }
        //插入新字符串的部分内容
        for(int i = 0; i < copyLen; i++){
            S->ch[pos - 1 + i] = T.ch[i];
        }
        S->length += copyLen;
        return ERROR; //因空间不足部分插入，返回错误
    }
}

//从串S中删除第pos个字符起长度为len的子串
Status StrDelete(String* S, int pos, int len){
    if(pos < 1 || pos > S->length - len + 1 || len < 0) return ERROR;
    for(int i = pos + len; i <= S->length; i++){
        S->ch[i - len - 1] = S->ch[i - 1];
    }
    S->length -= len;
    return OK;
}

//由串S复制得串T
Status StrCopy(String* T, String *S){
    for(int i = 0; i < S->length; i++){
        T->ch[i] = S->ch[i];
    }
    T->length = S->length;
    return OK;
}

//若串S为空串，则返回true，反之false
bool StrEmpty(String* S){
    return S->length == 0;
}

//比较串S和T，若 S>T，则返回值大于 0；若 S=T，则返回值 0；若 S<T，则返回值小于 0
int StrCompare(String S, String T){
    for(int i = 0; i < S.length && i < T.length; i++){
        if(S.ch[i] != T.ch[i]){
            return S.ch[i] - T.ch[i];
        }
    }
    return S.length - T.length;
}

//返回串S的长度，即串S中的字符个数
int StrLength(String S){
    return S.length;
}

//将串S清空为空串
Status StrClear(String* S){
    S->length = 0;
    return OK;
}
/*更彻底的版本// 将 S 清为空串
Status StrClear(String *S) {
    for (int i = 0; i < S->length; i++) {
        S->ch[i] = '\0';
    }
    S->length = 0;
    return OK;
}*/

//将串T的值连接在串S的后面
Status StrCat(String* S, String *T){
    if(S->length + T->length <= MAXSIZE){
        for(int i = 0; i < T->length; i++){
            S->ch[S->length + i] = T->ch[i];
        }
        S->length += T->length;
        return OK;
    }else{
        for(int i = 0; i < MAXSIZE - S->length; i++){
            S->ch[S->length + i] = T->ch[i];
        }
        S->length = MAXSIZE;
        return ERROR;
    }
}

//用Sub返回串S的第pos个字符起长度为len的子串
Status SubString(String* Sub, String S, int pos, int len){
    if(pos <= 0 || pos > S.length || len < 0 || len > S.length - pos + 1){
        return ERROR;
    }
    for(int i = 0; i < len; i++){
        Sub->ch[i] = S.ch[pos + i - 1];
    }
    Sub->length = len;
    return OK;
}

//若串S中存在和串T相同的子串，则返回它在串S中第pos个字符之后第一次出现的位置，否则返回0
int StrIndex(String* S, String* T, int pos){
    int i = pos;
    int j = 1;
/*i 用于遍历 S，从 pos 位置开始。
j 用于遍历 T，从第一个字符开始。*/
    while(i <= S->length && j <= T->length){
        if(S->ch[i - 1] == T->ch[j - 1]){
            ++i;
            ++j;
        }else {
            i = i - j + 2;
            j = 1;
        }
/*将 i 退回到上次匹配开始的下一位置 (i = i - j + 2)，
继续查找。将 j 重置为 1。*/
    }
    if(j > T->length){
        return i - T->length;
    }else {
        return 0;
    }
/*如果 j 超过了 T 的长度，说明找到了完整的 T 子串。
返回 i - T->length，即 T 在 S 中第一次出现的位置。*/
}
/*// 查找子串的位置
void findAllSubstringPositions(String* S, String* T, int* positions, int maxPositions) {
    int count = 0;
    int pos = 1;
    while (count < maxPositions) {
        pos = StrIndex(S, T, pos);
        if (pos == 0) break;
        positions[count++] = pos;
        pos++;
    }
    positions[count] = 0; // 以0结尾表示结束
}
    该代码可以通过循环遍历找到所有位置*/

//用V替换串S中出现的所有与T相等的不重叠的子串
void StrReplace(String* S, String* T, String* V){
    int i = 0;
    if(StrEmpty(T)) return;
    i = StrIndex(S,T,i);
    while(i) {
        StrDelete(S,i,StrLength(*T));
        StrInsert(S,i,*V);
        i += StrLength(*V);
        i = StrIndex(S,T,i);
    }
}
/*初始化查找位置 i 为 0。
检查 T 是否为空子串，若为空则直接返回。
查找 T 在 S 中的第一次出现位置，赋值给 i。
进入循环：
在位置 i 删除 T。
在位置 i 插入 V。
更新 i 至刚插入的 V 之后的位置。
继续查找 T 在 S 中的下一个出现位置。
循环继续，直到 T 不再出现在 S 中。*/

//销毁串S
Status StrDestroy(String* S){
    S->length = 0;
    return OK;
}
//实际上定长顺序串不存在所谓的销毁，因为它的内存是由编译器在栈上或静态区分配的

//打印字符串
void StrPrint(String *T){
    for(int i = 0; i < T->length; i++){
        printf("%c",T->ch[i]);
    }
    printf("\n");
}

//应用函数：字符串拼接
void concatenateStrings(String* result, String* s1, String* s2){
    StrClear(result);
    StrCopy(result, s1);
    StrCat(result, s2);
}

//应用函数：查找并替换子串
void findAndReplaceSubstring(String* s, String* target, String* replacement){
    StrReplace(s,target,replacement);
}

//应用函数：查找子串的位置
int findSubstringPosition(String* s, String* target, int startPos){
    return StrIndex(s,target,startPos);
}

//测试主函数
int main() {
    String s1, s2, result, target, replacement;
    int postion;

    // 生成字符串
    StrAssign(&s1, "Hello");
    StrAssign(&s2, "World");
    printf("字符串s1:");
    StrPrint(&s1);
    printf("字符串s2:");
    StrPrint(&s2);

    // 字符串拼接
    concatenateStrings(&result, &s1, &s2);
    printf("拼接后的字符串:");
    StrPrint(&result);

    // 查找并替换子串
    StrAssign(&target, "loWo");
    StrAssign(&replacement, "lo Universe");
    findAndReplaceSubstring(&result, &target, &replacement);
    printf("替换后的字符串:");
    StrPrint(&result);

    // 查找子串的位置
    postion = findSubstringPosition(&result, &replacement, 1);
    printf("子串的位置:%d\n", postion);

    // 销毁字符串（仅清空内容，无需释放内存）
    StrDestroy(&s1);
    StrDestroy(&s2);
    StrDestroy(&result);
    StrDestroy(&target);
    StrDestroy(&replacement);

    return 0;
}