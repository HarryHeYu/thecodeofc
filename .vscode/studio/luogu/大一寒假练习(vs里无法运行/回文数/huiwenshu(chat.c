#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// 判断是否是回文数
int isPalindrome(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        if (str[i] != str[len - i - 1]) {
            return 0;  // 不是回文数
        }
    }
    return 1;  // 是回文数
}
// 字符串反转
void reverseString(char *str, char *reversedStr) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        reversedStr[i] = str[len - i - 1];
    }
    reversedStr[len] = '\0';  // 确保字符串结束
}
// 进制加法
void addNumbers(char *a, char *b, char *result, int base) {
    int len1 = strlen(a);
    int len2 = strlen(b);
    int i = len1 - 1;
    int j = len2 - 1;
    int carry = 0;  // 进位
    int k = 0;      // 结果字符串的索引
    // 从低位到高位逐位相加
    while (i >= 0 || j >= 0 || carry > 0) {
        int sum = carry;
        // 处理a的当前位
        if (i >= 0) {
            if (a[i] >= '0' && a[i] <= '9') {
                sum += a[i] - '0';//将ASCII转换为int类型 
            } else {
                sum += 10 + (a[i] - 'A');  // 处理十六进制字母,//将ASCII转换为int类型 
            }
            i--;
        }
        // 处理b的当前位
        if (j >= 0) {
            if (b[j] >= '0' && b[j] <= '9') {
                sum += b[j] - '0';
            } else {
                sum += 10 + (b[j] - 'A');  // 处理十六进制字母
            }
            j--;
        }
        // 计算当前位的值和进位
        result[k++] = (sum % base) + '0';//转换为ASCII
        if (sum % base > 9) {
            result[k - 1] = (sum % base - 10) + 'A'; // 处理大于9的情况,先在前面一步将数值存入，再在此判断中对大于9的情况进行修正
        }
        carry = sum / base;//用于对下一位的进位
    }
    result[k] = '\0';  // 添加字符串结束符
}
// 将输入转换为指定进制的字符串
void convertToBase(char *input, int base, char *output) {
    int len = strlen(input);
    for (int i = 0; i < len; i++) {
        if (input[i] >= '0' && input[i] <= '9') {
            output[i] = input[i];
        } else {
            output[i] = input[i];
        }
    }
    output[len] = '\0';
}
int main() {
    int N;  // 进制
    char M[101];  // 输入的数（最多100位）
    char num[101];  // 当前处理的数
    char reversedNum[101];  // 反转后的数
    char stepResult[203];  // 每一步的加法结果
    int steps = 0;  // 步数
    // 输入N和M
    scanf("%d", &N);
    scanf("%s", M);
    // 将输入转换为指定进制的字符串
    convertToBase(M, N, num);
    // 检查初始数是否已经是回文数
    if (isPalindrome(num)) {
        printf("STEP=%d\n", steps);
        return 0;
    }
    // 最多进行30次循环
    for (steps = 1; steps <= 30; steps++) {
        // 反转当前数
        reverseString(num, reversedNum);
        // 计算当前数加反转后的数
        addNumbers(num, reversedNum, stepResult, N);
        // 检查结果是否为回文数
        if (isPalindrome(stepResult)) {
            printf("STEP=%d\n", steps);
            return 0;
        }
        // 更新当前数为加法结果
        strcpy(num, stepResult);
    }
    // 如果30步内无法生成回文数
    printf("Impossible!\n");
    return 0;
}
