#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "dsstring.h" // 请不要删除，否则检查不通过

/*src为要查找的字符串
pos为子串开始的下标
len为子串的长度
sub在函数调用运行前指向一个已经初始化好的空串，在函数返回时，sub指向串src从第pos个字符起长度为len的子串
函数查找成功返回true，参数不正确返回 false*/

//用Sub返回串S的第pos个字符起长度为len的子串
bool blstr_substr(BLString src, int pos, int len, BLString *sub){ 
    int i, j = 0, k = 0;
    Block *p, *q, *newNode;  
    // 初始化子串
    blstr_init(sub);   
    // 检查参数有效性
    if(pos < 0 || pos >= src.len || len <= 0){
        return false;
    } 
    // 如果请求的长度超过字符串范围，则截断为可用的最大长度
    if(pos + len > src.len){
        len = src.len - pos;
    }   
    p = src.head;
    q = NULL;  
    // 找到起始位置  是找到第pos个字符所在的块,j是块内的起始位置
    for(i = 0; i < pos; i++){
        j++;
        if(j == BLOCK_SIZE){
            p = p->next;
            j = 0;
        }
    }  
    // 复制子串
    for(i = 0; i < len; i++){
        if(k % BLOCK_SIZE == 0){
            newNode = (Block*)malloc(sizeof(Block));
            if(!newNode) return false;
            newNode->next = NULL;
            if(!sub->head){
                sub->head = newNode;// 设置头指针
            }else {
                q->next = newNode;// 链接到前一个块
            }
            q = newNode;// 更新前一个块指针
            //初始化块串中未使用的字符
            for(int l = 0; l < BLOCK_SIZE; l++){
                newNode->ch[l] = BLS_BLANK;
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
    sub->tail = q;//设置尾指针
    sub->len = len;
    return true;
}