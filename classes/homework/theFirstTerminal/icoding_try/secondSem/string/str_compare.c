#include <stdio.h>
#include <stdlib.h>
#include "dsstring.h" //请不要删除，否则检查不通过

/*该操作当比较的两个字符是都是字母，且两个字符互为大小写（如a和A、e和E）时认为两个字符相同，否则不同，其比较结果按这两个字符的原值确定。函数的返回值规定如下：
返回值 < 0：第一个不匹配的字符在 ptr1 中的值低于 ptr2 中的值
返回值 == 0：两个字符串的内容相等
返回值 > 0：第一个不匹配的字符在 ptr1 中的值大于在 ptr2 中的值*/

int str_compare(const char* ptr1, const char* ptr2){
    if(ptr1 == NULL || ptr2 == NULL){
        return 0; 
    }
    if(ptr1 == NULL){
        return -1;
    }
    if(ptr2 == NULL){
        return 1;
    }
    while(*ptr1 != '\0' && *ptr2 != '\0'){
        //检查是否为字母
        int is_letter1 = ((*ptr1 >= 'A' && *ptr1 <= 'Z') || (*ptr1 >= 'a' && *ptr1 <= 'z'));
        int is_letter2 = ((*ptr2 >= 'A' && *ptr2 <= 'Z') || (*ptr2 >= 'a' && *ptr2 <= 'z'));
        //检查是否互为大小写
        int case_diff = (*ptr1 - *ptr2);
        int is_case_diff = is_letter1 && is_letter2 && (case_diff == 32 || case_diff == -32);

        //检查字符是否相同(相同字符或互为大小写)
        if(*ptr1 != *ptr2 && !is_case_diff){
            //第一个不匹配的字符，返回差值
            return *ptr1 - *ptr2;
        }
        //移动到下一个字符
        ptr1++;
        ptr2++;
    }
    //如果其中一个字符串结束而另一个还未结束
    if(*ptr1 != '\0' && *ptr2 == '\0'){
        return 1; //ptr1大于ptr2
    } else if(*ptr1 == '\0' && *ptr2 != '\0'){
        return -1; //ptr1小于ptr2
    }
    //如果两个字符串都结束，返回0
    return 0;
}
