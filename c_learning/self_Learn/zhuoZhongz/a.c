#include <stdio.h>

int main() {
    //与运算
    int a = 5; // 二进制: 0101
    int b = 3; // 二进制: 0011
    int result_and = a & b; // 二进制: 0001, 十进制: 1
    printf("a & b = %d\n", result_and);

    //或运算
    int a = 5; 
    int b = 3; 
    int result_or = a | b; // 二进制: 0111, 十进制: 7
    printf("a | b = %d\n", result_or);

    //异或运算
    int a = 5; 
    int b = 3; 
    int result_xor = a ^ b; // 二进制: 0110, 十进制: 6
    printf("a ^ b = %d\n", result_xor);

    //取反运算
    int a = 5; 
    int result_not = ~a; // 二进制: 1010, 十进制: -6
    printf("~a = %d\n", result_not);
    
    //左移运算
    int a = 5;
    int result_left_shift = a << 1; // 二进制: 1010, 十进制: 10
    printf("a << 1 = %d\n", result_left_shift);

    //右移运算
    int a = 5; 
    int result_right_shift = a >> 1; // 二进制: 0010, 十进制: 2
    printf("a >> 1 = %d\n", result_right_shift);

    //按位取反
    int a = 5;
    int result_bitwise_not = ~a; // 二进制: 1010, 十进制: -6
    printf("~a = %d\n", result_bitwise_not);

    //按位与
    int a = 5;
    int b = 3; 
    int result_bitwise_and = a & b; // 二进制: 0001, 十进制: 1
    printf("a & b = %d\n", result_bitwise_and);

    //按位或
    int a = 5; 
    int b = 3;
    int result_bitwise_or = a | b; // 二进制: 0111, 十进制: 7
    printf("a | b = %d\n", result_bitwise_or);

    //按位异或
    int a = 5; 
    int b = 3; 
    int result_bitwise_xor = a ^ b; // 二进制: 0110, 十进制: 6
    printf("a ^ b = %d\n", result_bitwise_xor);

    //按位左移
    int a = 5; 
    int result_bitwise_left_shift = a << 1; // 二进制: 1010, 十进制: 10
    printf("a << 1 = %d\n", result_bitwise_left_shift);

    //按位右移
    int a = 5; 
    int result_bitwise_right_shift = a >> 1; // 二进制: 0010, 十进制: 2
    printf("a >> 1 = %d\n", result_bitwise_right_shift);

    return 0;
}