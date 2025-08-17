/*非众数字符串：只要这个字符串中出现的次数最多的字符的出现次数没有超过字符串长度的一半。
即：n(times) <= n/2 || n(times) <= (n-1)/2
非空子串：必须要是连续的*/
#include<stdio.h>
#include<string.h>

#define MAX 501
#define SIZE 26

int main(){
    char s[MAX];
    scanf("%s",s);
    int n = strlen(s);//计算输入的字符串的长度
    int countOfSubstr = 0;//初始化非众数字符串的个数为0
    for(int i = 0; i <=  n; i++){//遍历所有可能的字符串长度
        for(int j = 0; j <= n; j++){//遍历所有可能的字符串起始位置
            int count[SIZE] = {0};//初始化每个字符出现的次数为0
            int maxCount = 0;//初始化出现次数最多的字符出现的次数为0
            for( int k = i; k < j + i; k++){//遍历每个位置的字符
                count[s[k] - 'a']++;//统计每个字符出现的次数
                if(count[s[k]- 'a'] > maxCount){//更新出现次数最多的字符出现的次数
                    maxCount = count[s[k] - 'a'];
                }
            }
            if(maxCount <= (i/2)){
                countOfSubstr++;//非众数字符串的个数加1
            }
        }
    }
    printf("%d\n",countOfSubstr);//输出非众数字符串的个数
    return 0;
}