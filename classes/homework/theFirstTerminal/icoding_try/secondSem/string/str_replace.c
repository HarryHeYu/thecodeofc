#include<stdio.h>
#include<stdlib.h>
#include "dsstring.h"

/*in， 原始字符串，保持不变
out, 存放替换结果的字符串
outlen，out空间的大小
oldstr，要替换的旧字符串
newstr，替换成的新字符串
函数返回成功替换的次数，即有多少个子串被成功替换*/

int str_replace(const char *in, char *out, int outlen, 
    const char *oldstr, const char *newstr){
    int count = 0; // 成功替换的次数
    int old_len = 0; // oldstr的长度
    int new_len = 0; // newstr的长度
    int in_pos = 0; // 输入字符串当前位置
    int out_pos = 0; // 输出字符串当前位置
    // 计算oldstr和newstr的长度
    while(oldstr[old_len] != '\0') old_len++;
    while(newstr[new_len] != '\0') new_len++; 
    //处理边界情况
    if(old_len == 0 || outlen <= 1){// new_len可以为0，表示删除原字符
        if(outlen > 0) out[0] = '\0'; // 如果outlen大于0，设置输出字符串为空
        /*字符串必须以空字符 '\0' 结尾，即使是空字符串也需要至少 1 个字节 来存储这个终止符。
        输出缓冲区的大小恰好为 1，此时唯一合法的内容是 空字符串（仅包含 '\0'）。
        */
        return count;
    }
    while(in[in_pos] != '\0'){
        //检查是否匹配oldstr
        int match = 1;
        for(int i = 0; i < old_len; i++){
            if(in[in_pos + i] == '\0' || in[in_pos + i] != oldstr[i]){
                match = 0;
                break;
            }
        }
        //如果匹配成功，检查途欢后是否会超出outlen
        if(match){
            //计算替换后的总长度
            int remaining_len = 0;
            int temp_pos = in_pos + old_len;//假设替换后到达的位置
            //计算剩余输入的长度
            while(in[temp_pos + remaining_len] != '\0'){
                remaining_len++;
            }
            //检查替换后的字符串(包括结束符)是否超出outlen
            if(out_pos + new_len + remaining_len + 1 <= outlen){
                //替换不会超出，进行替换
                for(int i = 0; i < new_len; i++){
                    out[out_pos++] = newstr[i];
                }
                in_pos += old_len; // 移动输入字符串指针
                count++; // 成功替换一次
            } else{
                //替换会超出outlen
                if(count == 0){
                    //如果第一次替换就超出，直接复制原始字符串
                    int i = 0;
                    while(in[i] != '\0' && i < outlen - 1){
                        out[i] = in[i];
                        i++;
                    }
                    out[i] = '\0';
                    return count;
                } else {
                    //已经进行过替换，复制剩余部分
                    while(in[in_pos] != '\0' && out_pos < outlen - 1){
                        out[out_pos++] = in[in_pos++];
                    }
                    break;
                }
            }
        }else{
            //不匹配，复制当前字符
            out[out_pos++] = in[in_pos++];
            //检查是否还有足够的空间
            if(out_pos >= outlen - 1){
                //没有足够的空间，结束复制
                break;
            }
        }
    }
    //确保输出字符串以空字符结尾
    out[out_pos] = '\0';
    return count;
}