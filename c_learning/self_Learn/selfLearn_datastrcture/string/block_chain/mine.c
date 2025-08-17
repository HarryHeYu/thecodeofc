/***************************
 *  <name：块链串
 *  <author: He YU Hao
 *  <date:2025.4.2
*****************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BLOCK_SIZE 4 //块的大小
typedef enum {ERROR = 0, OK = 1} Status;

typedef struct BlockNode {
    char ch[BLOCK_SIZE]; // 块中的字符数组
    struct BlockNode *next;//指向下一个块的指针
}BlockNode;

typedef struct {
    BlockNode *head, *tail; //链表的头和尾指针
    int Length; //字符串的长度
}String;

//初始化串S
Status StrInit(String* S){
    S->head = S->tail = NULL;
    S->Length = 0;
    return OK;
}

//生成一个值等于chars的串S
Status StrAssign(String* T, char* chars){
    int i, len = strlen(chars);
    BlockNode *p, *q;
    p = q = NULL;
    StrInit(T);
    for(int i = 0; i < len; i++){
        if(i % BLOCK_SIZE == 0){
            q = (BlockNode*)malloc(sizeof(BlockNode));
            if(!q) return ERROR;
            q->next = NULL;
            if(!T->head){
                T->head = q;
            } else{
                p->next = q;
            }
            p = q;
            // 初始化块中未使用的字符
            for (int j = 0; j < BLOCK_SIZE; j++) {
                q->ch[j] = '\0';
            }
        }
        q->ch[i % BLOCK_SIZE] = chars[i];
    }
    T->tail = q;
    T->Length = len;
    return OK;
}

//由串S复值得串T
Status StrCopy(String *T, String S) {
    BlockNode *p, *q, *newNode;
    p = S.head;
    q = NULL;
    StrInit(T); // 初始化目标串 T
    int blocks = (S.Length + BLOCK_SIZE - 1) / BLOCK_SIZE; // 计算源串 S 所需的块数
    int currentBlock = 0;   
    while(p) { // 遍历源串 S 的块链表
        newNode = (BlockNode*) malloc(sizeof(BlockNode)); // 分配一个新块
        if(!newNode) return ERROR; // 分配失败返回错误状态
        for(int i = 0; i < BLOCK_SIZE; i++) {
            // 如果当前块不是最后一个块，或者是最后一个块但当前字符位置在已使用范围内
            if (p->next != NULL || i < S.Length % BLOCK_SIZE && currentBlock < blocks - 1) {
                newNode->ch[i] = p->ch[i]; // 复制字符
            } else {
                newNode->ch[i] = '\0'; // 初始化未使用的字符为 '\0'
            }
        }
        newNode->next = NULL;
        if(!T->head) {
            T->head = newNode; // 设置目标串 T 的头指针
        } else {
            q->next = newNode; // 链接到目标串 T 的前一个块
        }
        q = newNode; // 更新目标串 T 的尾指针
        p = p->next; // 移动到源串 S 的下一个块
        currentBlock++; // 更新当前块计数
    }
    T->tail = q; // 设置目标串 T 的尾指针
    T->Length = S.Length; // 设置目标串 T 的长度
    return OK;
}

//检查串S是否为空串
bool StrEmpty(String S){
    return S.Length == 0;
}

//比较串S和T，若S>T，则返回值大于零，若相等则返回零，若S<T，则返回值小于0
int StrCompare(String S, String T){
    BlockNode *p = S.head, *q = T.head;
    int i = 0, j = 0;

    while (p && q) {
        if(p->ch[i] != q->ch[i]) {
            return p->ch[i] - q->ch[i];//返回字符串差值
        }
        i++;
        j++;
        if(i == BLOCK_SIZE){
            p = p->next;
            i = 0;
        }
        if(j == BLOCK_SIZE){
            q = q->next;
            j = 0;
        }
    }
    return S.Length - T.Length;
}

//返回串S的长度，即S中的字符个数
int StrLength(String S){
    return S.Length;
}

//将S清空为空串
Status StrClear(String *S){
    BlockNode *p, *q;
    p = S->head;
    while(p){
        q = p->next;
        free(p);
        p = q;
    }
    S->head = S->tail = NULL;
    S->Length = 0;
    return OK;
}

//将串T的值连接在串S的后面
Status StrCat(String *S, String T){
    BlockNode *p = S->tail;
    BlockNode *q = T.head;
    int i = 0;
    //如果S的最后一个块有剩余空间，先利用这些空间
    if(p && p->ch[S->Length % BLOCK_SIZE] == '\0'){
//检查 p 是否为空，以及 p 的最后一个字符位置是否为空（'\0'）。
        while(p && q && i < BLOCK_SIZE && p->ch[i % BLOCK_SIZE] == '\0'
        && q->ch[i % BLOCK_SIZE] != '\0'){
            p->ch[S->Length % BLOCK_SIZE] = q->ch[i % BLOCK_SIZE];
            i++;
            S->Length++;
            if(i % BLOCK_SIZE == 0) {
                q = q->next;
                i = 0;
            } 
        }
    }
    //处理剩余的块
    while(q){
        BlockNode *newNode = (BlockNode*)malloc(sizeof(BlockNode));
        if(!newNode) return ERROR;
        for(int j = 0; j < BLOCK_SIZE; j++){
            if(q->ch[j] != '\0') {
                newNode->ch[j] = q->ch[j];
            }else {
                newNode->ch[j] = '\0';
            }
        }
        newNode->next = NULL;
        if(S->tail) {
            S->tail->next = newNode;
        }else{
            S->head = newNode;
        }
        S->tail = newNode;
        q = q->next;
        S->Length += BLOCK_SIZE;
    }
    return OK;
}

//用Sub返回串S的第pos个字符起长度为len的子串
Status SubString(String *Sub, String S, int pos, int len){
    int i, j = 0, k = 0;
    BlockNode *p, *q, *newNode;
    p = S.head;
    q = NULL;
    StrInit(Sub);//初始化子串
    if(pos <= 0 || pos > S.Length || len < 0 || len > S.Length - pos + 1){
        return ERROR;//检查参数有效性
    }
    for(int i = 0 ; i < pos - 1; i++){
        j++;
        if(j == BLOCK_SIZE){
            p = p->next;
            j = 0;
        }
    }
/*从 S.head 开始，遍历 S 的块链表，找到包含起始位置 pos 的块以及块内的具体位置。
使用变量 j 记录块内字符的位置，每遍历一个字符，j 加 1。如果 j 达到 BLOCK_SIZE，则移动到下一个块并将 j 设为 0。*/
    for(i = 0 ; i < len; i++){
        if(k % BLOCK_SIZE == 0){
            newNode = (BlockNode*)malloc(sizeof(BlockNode));
            if(!newNode) return ERROR;
            newNode->next = NULL;
            if(!Sub->head){
                Sub->head = newNode;// 设置头指针
            }else {
                q->next = newNode;// 链接到前一个块
/*如果 Sub 的头指针不为空，即 Sub 已经有一个或多个块。
将 newNode 链接到子串 Sub 的前一个块 (q) 的 next 指针上。这意味着 newNode 是 q 块的下一个块。
如果子串 Sub 还没有任何块（即 Sub->head 为 NULL），则将新创建的块 newNode 设置为 Sub 的头块 (Sub->head)。
否则，将新创建的块 newNode 链接到子串 Sub 的前一个块 q 的 next 指针上。
*/
            }
            q = newNode;
            //初始化块串中未使用的字符
            for(int l = 0; l < BLOCK_SIZE; l++){
                newNode->ch[l] = '\0';
            }
        }
        newNode->ch[k % BLOCK_SIZE] = p->ch[j];
        k++;
        j++;
        if(j == BLOCK_SIZE){
            p = p->next;
            j = 0;
        }
    }
    Sub->tail = q;//设置尾指针
    Sub->Length = len;
    return OK;
}

//在串S的第pos个字符之前插入串T
Status StrInsert(String *S, int pos, String T){
    if(pos < 1 || pos > S->Length + 1) return ERROR;
    int i = 0, j = 0, k = 0;//在代码中，j 是一个计数器，用于跟踪在当前块中已经遍历的字符位置
    BlockNode *p, *q, *r, *newNode;
    p = S->head;//p 指向目标串 S 的头块
    q = NULL;//q 用于构建新块链表
    newNode = NULL;//newNode 用于分配新块
    //找到插入位置的前一个块和字符位置
    for(i = 0; i < pos - 1; i++){
        j++;
        if(j == BLOCK_SIZE){
            p = p->next;
            j = 0;
        }
    }
    //插入T的块到S中
    BlockNode* t = T.head;
    while(t){
        newNode = (BlockNode*)malloc(sizeof(BlockNode));
        if(!newNode) return ERROR;
        for(k = 0; k < BLOCK_SIZE; k++){
            newNode->ch[k] = t->ch[k];
        }
        newNode->next = NULL;
        if(!q){
            q = newNode;
        }else{
            q->next = newNode;
        }
        q = newNode;
        t = t->next;
    }
    //处理插入块之间的链接
    if(j == 0){
        newNode->next = p;//将新块链的末尾连接到当前块 p 上。
        if(p == S->head){
            S->head = newNode;
        }else{
            BlockNode* prev = S->head;
            while(prev->next != p){
                prev = prev->next;
            }
            prev->next = newNode;
/*如果 j == 0，表示插入位置正好在当前块的起始位置，也就是块的边界上。
将新块链的末尾连接到当前块 p 上。
如果当前块 p 是头块，更新头块指针 S->head 指向新块链。
否则，找到当前块 p 的前一个块 prev 并更新其 next 指针连接到新块链。*/
        }
    }else{
        // 创建一个新的临时块 tmp
        BlockNode* tmp = (BlockNode*)malloc(sizeof(BlockNode));
        if(!tmp) return ERROR;
        for(k = 0; k < BLOCK_SIZE; k++){
            if(k < j){
                tmp->ch[k] = p->ch[k];
            }else{
                tmp->ch[k] = '\0';
            }
        }
        tmp->next = newNode; // 将新块 tmp 的 next 指针指向新块链的头
        q->next = p; // 将新块链的末尾连接回当前块 p
        if(p == S->head){
            S->head = tmp; // 更新头块指针 S->head 指向新块 tmp
        }else{
            BlockNode* prev = S->head;
            while(prev->next != p){
                prev = prev->next;
            }
            prev->next = tmp; // 将前一个块的 next 指针连接到新块 tmp
        }
    
        // 处理插入位置之后的字符
        BlockNode* remain = (BlockNode*)malloc(sizeof(BlockNode));
        if(!remain) return ERROR;
        for(k = 0; k < BLOCK_SIZE - j; k++){
            remain->ch[k] = p->ch[j + k];
            p->ch[j + k] = '\0';
        }   
        remain->next = p->next;
        p->next = remain;
/*如果 j != 0，表示插入位置在当前块的中间。
创建一个新的临时块 tmp，将当前块 p 的前 j 个字符拷贝到 tmp 中，剩余部分填充为空字符 '\0'。
将新块 tmp 的 next 指针指向新块链的头。
将新块链的末尾连接回当前块 p。
如果当前块 p 是头块，更新头块指针 S->head 指向新块 tmp。
否则，找到当前块 p 的前一个块 prev 并更新其 next 指针连接到新块 tmp。
处理插入位置之后的字符，将剩余部分保存在新的块 remain 中，并连接到原来的块链。*/
     }
     S->Length += T.Length;
     return OK;
 }

 //从串S中删除第pos个字符起长度为len的子串
Status StrDelete(String* S, int pos, int len){
    int i, j = 0, k = 0;
    BlockNode *p, *q, *r;
    p = S->head;
    q = NULL;
    if(pos < 1 || pos > S->Length - len + 1 || len < 0) return ERROR;
    for(i = 0; i < pos - 1; i++){
        j++;
        if(j == BLOCK_SIZE){
            p = p->next;
            j = 0;
        }
    }
    r = p;
    for(i = 0; i < len; i++){
        k++;
        if(k % BLOCK_SIZE == 0){
            q = r->next;
            r->next = q->next;
            free(q);
        }
    }
    S->Length -= len;
}

//打印字符串
void StrPrint(String T){
    BlockNode *p = T.head;
    int i = 0;
    while(p) {
        for(i = 0; i < BLOCK_SIZE; i++){
            if(p->ch[i] != '\0'){//只打印有效字符
                printf("%c",p->ch[i]);
            }
        }
        p = p->next;
    }
    printf("\n");
}

//用V替换串S中出现的所有与T相等的不重复的子串
void StrReplace(String *S, String T, String V){
    int i = 0;
    String Sub;
    if(StrEmpty(T)) return ;
    while(i <= S->Length - StrLength(T)){
        SubString(&Sub,*S,i,StrLength(T));
        if(StrCompare(Sub,T) == 0){
            StrDelete(S,i+1,StrLength(T));
            StrInsert(S,i+1,V);
            i += StrLength(V);
        }else{
            i++;
        }
    }
}

//若串S中存在和串T相同的子串，则返回它在串S中第pos个字符之后第一次出现的位置，否则返回0
int StrIndex(String S, String T, int pos){
    if (pos < 1 || pos > S.Length) {
        return 0; // 检查参数有效性
    }
    BlockNode *p = S.head;
    BlockNode *q;
    int i = 1, j = 1;
    int k = pos - 1; // 从第 pos 个字符开始查找
    //找到第pos个字符所在的块和位置
    while (k > 0 && p)
    {
        k--;
        if(k % BLOCK_SIZE == 0 && k != 0)p = p->next;
    }
    //开始查找
    while(p && j <= T.Length) {
        q = T.head;
        i = k % BLOCK_SIZE;
        while(p && q && p->ch[i] == q->ch[j-1]){
            i++;
            j++;
            if(i == BLOCK_SIZE){
                p = p->next;
                i = 0;
            }
            if(j > T.Length){
                return k + 1;//找到匹配的子串，返回起始位置
            }
            q = q->next;
        }
        k++;
        j = 1;
        i = k % BLOCK_SIZE;
        if(i == 0 && p){
            p = p->next;
        }//如果当前字符不匹配，移动到 S 的下一个字符继续查找。
    }
    return 0;
}

//销毁串S
Status StrDestroy(String *S){
    BlockNode *p, *q;
    p = S->head;
    while(p) {
        q = p->next;
        free(p); //释放块
        p = q;
    }
    S->head = S->tail = NULL;
    S->Length = 0;
    return OK;
}

// 应用函数：字符串拼接
void concatenateStrings(String *result, String s1, String s2) {
    StrClear(result);
    StrCopy(result, s1);
    StrCat(result, s2);
}

// 应用函数：查找并替换子串
void findAndReplaceSubstring(String *s, String target, String replacement) {
    StrReplace(s, target, replacement);
}

// 应用函数：查找子串位置
int findSubstringPosition(String s, String target, int startPos) {
    return StrIndex(s, target, startPos);
}

int main() {
    String s1, s2, result, target, replacement;
    int position;

    // 初始化字符串
    StrInit(&s1);
    StrInit(&s2);
    StrInit(&result);
    StrInit(&target);
    StrInit(&replacement);

    // 生成字符串
    StrAssign(&s1, "Hello");
    StrAssign(&s2, "World");
    printf("字符串 s1: ");
    StrPrint(s1);
    printf("字符串 s2: ");
    StrPrint(s2);

    // 字符串拼接
    concatenateStrings(&result, s1, s2);
    printf("拼接后的字符串: ");
    StrPrint(result);

    // 查找并替换子串
    StrAssign(&target, "loWo");
    StrAssign(&replacement, "lo Universe");
    findAndReplaceSubstring(&result, target, replacement);
    printf("替换后的字符串: ");
    StrPrint(result);

    // 查找子串位置
    position = findSubstringPosition(result, replacement, 1);
    printf("子串的位置: %d\n", position);

    // 销毁字符串
    StrDestroy(&s1);
    StrDestroy(&s2);
    StrDestroy(&result);
    StrDestroy(&target);
    StrDestroy(&replacement);

    return 0;
}