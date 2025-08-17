/***************************
 *  <name：堆串
 *  <author: He Yu Hao
 *  <date:2025.4.1
*****************************/

//堆串（heap string）通常是指在堆内存中动态分配的字符串。

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum {ERROR = 0, OK = 1}Status;

typedef struct {
    char *ch;//指向存储字符串的指针
    int length;
}String;

//生成一个值等于chars的串
Status StrAssign(String *T, char* chars) {
    int len = strlen(chars);
    T->ch = (char*)malloc((len + 1) * sizeof(char));
    if (!T->ch) return ERROR;
    for (int i = 0; i < len; i++) T->ch[i] = chars[i];
    T->ch[len] = '\0'; // 确保以 NULL 结尾
    T->length = len;    
    return OK;
}

//在串S的第pos个字符之前插入串T
Status StrInsert(String *S, int pos, String* T){
    if(pos < 1 || pos > S->length + 1) return ERROR;
    char* new_ch = (char*)realloc(S->ch, (S->length + T->length + 1) * sizeof(char));
    if (!new_ch) return ERROR;
    S->ch = new_ch;
    // 从后向前移动数据
    for (int i = S->length - 1; i >= pos - 1; i--) {
        S->ch[i + T->length] = S->ch[i];
    }
    // 复制 T
    for (int i = 0; i < T->length; i++) {
        S->ch[pos - 1 + i] = T->ch[i];
    }
    S->length += T->length;
    S->ch[S->length] = '\0';  // 终止符
    return OK;    
}

//从串S中删除第pos个字符起长度为len的子串
Status StrDelete(String* S, int pos, int len){
    if(pos < 1 || pos > S->length - len + 1 || len < 0) return ERROR;
    for(int i = pos + len; i <= S->length; i++){
        S->ch[i - len - 1] = S->ch[i - 1];
    }
/*从 pos + len 开始遍历到 S->length，将每个字符向前移动 len 个位置。
i 表示遍历的当前位置，i - len - 1 表示该字符的新位置。
例如，对于字符串 S = "HelloWorld"，如果 pos = 6 和 len = 3，则 S->ch[8] 将移动到 S->ch[5]。*/
    S->length -= len;
    if (S->length == 0) {
        free(S->ch);
        S->ch = NULL;
/*如果 S->length == 0，realloc 可能返回 NULL，导致后续访问 S->ch 崩溃。
realloc(S->ch, 0) 在某些实现下可能导致 free(S->ch)，
但 S->ch 不会被置为 NULL，之后访问 S->ch 可能会出错。*/
    } else {
        char* new_ch = (char*)realloc(S->ch, (S->length + 1) * sizeof(char));
        if (!new_ch) return ERROR;
        S->ch = new_ch;
        S->ch[S->length] = '\0';
    }
    return OK;
}

// 由串 S 复制得串 T
Status StrCopy(String* T, String* S){
    //如果T原有内存则释放
    free(T->ch);
    T->ch = (char*)malloc(S->length * sizeof(char));
    if(!T->ch) return ERROR;
    for(int i = 0; i < S->length; i++){
        T->ch[i] = S->ch[i];
    }
    T->length = S->length;
    return OK;
}

//检查串是否为空
int StrEmpty(String* S){
    return S->length == 0;
}

//比较串S和T，若S>T,则返回值大于0，相等则等于0，若S<T则返回值小于0
int StrCompare(String* S, String* T) {
    if (S->length == 0 || T->length == 0) {
        return S->length - T->length;
    }
/*如果 S->length == 0 或 T->length == 0，S->ch[i] 可能访问 NULL，导致崩溃。*/
    for (int i = 0; i < S->length && i < T->length; i++) {
        if (S->ch[i] != T->ch[i]) {
            return S->ch[i] - T->ch[i];
        }
    }
    return S->length - T->length;
}

/*函数首先通过循环逐个字符比较两个字符串。
如果字符不相等，返回它们的差值。
如果所有字符都相等，则通过比较它们的长度来确定大小关系。
返回值大于 0 表示 S 大于 T，小于 0 表示 S 小于 T，等于 0 表示 S 等于 T。*/

//返回串S的长度，即串S的字符个数
int StrLength(String* S){
    return S->length;
}

//将S清空为空串
Status StrClear(String* S) {
    if (S->ch != NULL) {
        free(S->ch);
        S->ch = NULL;
    }
    S->length = 0;
    return OK;
}
/*更详细将S清空为空串
Status StrClear(String* S){
    if (S->ch != NULL) {
        memset(S->ch, 0, S->length * sizeof(char)); // 将内存内容清零
        free(S->ch); // 释放动态分配的内存
        S->ch = NULL; // 将指针置为NULL
    }
    S->length = 0; // 重置字符串长度
    return OK;
}*/

//将串T的值连接在串S的后面
Status StrCat(String* S, String* T){
    S->ch = (char*)realloc(S->ch,(T->length + S->length) * sizeof(char));
    if(!S->ch) return ERROR;
    for(int i = 0; i < T->length; i++){
        S->ch[i + S->length] = T->ch[i];
    }
    S->length += T->length;
    return OK;
}

//用Sub返回串S的第pos个字符起长度为len的子串
Status SubString(String* S, String* Sub, int pos, int len) {
    if (pos < 1 || len < 0 || pos > S->length || len > S->length - pos + 1) {
        return ERROR;
    }
    Sub->ch = (char*)malloc((len + 1) * sizeof(char));
    if (!Sub->ch) return ERROR;
    for (int i = 0; i < len; i++) {
        Sub->ch[i] = S->ch[i + pos - 1];
    }
    Sub->ch[len] = '\0';
    Sub->length = len;
    return OK;
}


//若串S中存在和串T相同的子串，则返回它在串S中第pos个字符之后第一次出现的位置；否则返回0
int StrIndex(String S, String T, int pos) {
    // pos 从 1 开始，转换为数组下标 pos - 1
    if (pos < 1 || pos > S.length - T.length + 1) return 0;
    for (int i = pos - 1; i <= S.length - T.length; i++) {
        int j;
        // 对 S 中从 i 开始的连续子串与 T 进行匹配
        for (j = 0; j < T.length; j++) {
            if (S.ch[i + j] != T.ch[j])
                break;
        }
        // 如果 j 达到 T.length，说明找到了完整的 T 子串
        if (j == T.length)
            return i + 1;  // 返回的位置按 1 开始计数
    }
    // 如果未找到匹配，返回 0
    return 0;
}
/*while类型的：
int StrIndex(String* S, String* T, int pos) {
    // pos 是 1 基的索引，转换为 0 基下标
    if (pos < 1 || pos > S->length - T->length + 1) {
        return 0;
    }
    int i = pos - 1; // i 为 S 中的起始匹配位置（0 基）
    while (i <= S->length - T->length) {
        int j = 0; // j 用于遍历 T
        while (j < T->length && S->ch[i + j] == T->ch[j]) {
            j++;
        }
        if (j == T->length) {
            // 找到匹配，返回 1 基位置
            return i + 1;
        }
        i++; // 如果当前起始位置不匹配，移动到下一个位置
    }
    // 未找到匹配，返回 0
    return 0;
}
*/



//用V替换串S中出现的所有与T相等的不重叠的子串
void StrReplace(String* S, String T, String V) {
    int i = 1; // 从第一个字符开始查找（1起始）
    int pos;
    while ((pos = StrIndex(*S, T, i)) > 0) {
        if (StrDelete(S, pos, T.length) == ERROR) break;
        if (StrInsert(S, pos, &V) == ERROR) break;
        i = pos + V.length; // 更新下一个查找起点
    }
}


//销毁串S
Status StrDestroy(String* S) {
    if (S->ch != NULL) {
        free(S->ch);
        S->ch = NULL;
    }
    S->length = 0;
    return OK;
}


//打印字符串
void StrPrint(String T){
    for(int i = 0; i < T.length; i++){
        printf("%c",T.ch[i]);
    }
    printf("\n");
}

// 应用函数：字符串拼接
void concatenateStrings(String *result, String *s1, String *s2) {
    StrClear(result);       // 清空结果字符串
    StrCopy(result, s1);    // 复制 s1 到结果字符串
    StrCat(result, s2);     // 将 s2 拼接到结果字符串
}

// 应用函数：查找并替换子串
void findAndReplaceSubstring(String *s, String *target, String *replacement) {
    StrReplace(s, *target, *replacement); // 替换子串
}

// 应用函数：查找子串位置
int findSubstringPosition(String *s, String *target, int startPos) {
    return StrIndex(*s, *target, startPos); // 查找子串位置
}

int main() {
    String s1 = { NULL, 0 };
    String s2 = { NULL, 0 };
    String result = { NULL, 0 };
    String target = { NULL, 0 };
    String replacement = { NULL, 0 };
/*未初始化的指针是随机的，导致 free(S->ch) 可能释放非法内存，造成程序崩溃。
将指针初始化为 NULL 后，free(NULL) 是安全的，不会崩溃。
养成初始化变量的好习惯，尤其是动态内存管理相关的变量，能避免很多 bug。
之前直接就定义了而没有赋值导致在free（T->ch)时报错*/
    int position;

    // 初始化字符串
    if (StrAssign(&s1, "Hello") == ERROR ||
        StrAssign(&s2, "World") == ERROR) {
        printf("StrAssign 错误\n");
        return -1;
    }
    printf("字符串 s1: ");
    StrPrint(s1);
    printf("字符串 s2: ");
    StrPrint(s2);

    // 字符串拼接
    concatenateStrings(&result, &s1, &s2);
    printf("拼接后的字符串: ");
    StrPrint(result);

    // 查找并替换子串
    if (StrAssign(&target, "loWo") == ERROR ||
        StrAssign(&replacement, "lo Universe") == ERROR) {
        printf("StrAssign 错误\n");
        return -1;
    }
    findAndReplaceSubstring(&result, &target, &replacement);
    printf("替换后的字符串: ");
    StrPrint(result);

    // 查找子串位置
    position = findSubstringPosition(&result, &replacement, 1);
    printf("子串的位置: %d\n", position);

    // 销毁字符串
    StrDestroy(&s1);
    StrDestroy(&s2);
    StrDestroy(&result);
    StrDestroy(&target);
    StrDestroy(&replacement);

    return 0;
}
