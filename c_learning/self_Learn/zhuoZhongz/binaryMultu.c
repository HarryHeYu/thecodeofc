#include <stdlib.h>
#include <string.h>

typedef struct _Bigint{
    int mem_len;
    int digit_len;
    unsigned char* p;
} Bigint;

// 获取指定位置的十进制数字
int get_digit(Bigint num, int pos) {
    int byte_pos = pos / 2;
    if (pos % 2 == 0) {
        // 偶数位置，高4位
        return (num.p[byte_pos] >> 4) & 0xF;
    } else {
        // 奇数位置，低4位
        return num.p[byte_pos] & 0xF;
    }
}

// 设置结果的指定位置为给定的十进制数字
void set_digit(Bigint* result, int pos, int digit) {
    int byte_pos = pos / 2;
    if (pos % 2 == 0) {
        // 偶数位置，设置高4位
        result->p[byte_pos] = (result->p[byte_pos] & 0x0F) | ((digit & 0xF) << 4);
    } else {
        // 奇数位置，设置低4位
        result->p[byte_pos] = (result->p[byte_pos] & 0xF0) | (digit & 0xF);
    }
}

// 大整数乘法
Bigint multi(Bigint m, Bigint k) {
    // 结果最大位数是两个数字位数之和
    int max_digit_len = m.digit_len + k.digit_len;
    int* temp = (int*)calloc(max_digit_len, sizeof(int));

    // 执行乘法计算（按低位索引）
    for (int i = 0; i < m.digit_len; i++) {
        int m_pos = m.digit_len - 1 - i;
        int m_digit = get_digit(m, m_pos);
        for (int j = 0; j < k.digit_len; j++) {
            int k_pos = k.digit_len - 1 - j;
            int k_digit = get_digit(k, k_pos);
            temp[i + j] += m_digit * k_digit;
        }
    }

    // 处理进位
    for (int i = 0; i < max_digit_len - 1; i++) {
        temp[i + 1] += temp[i] / 10;
        temp[i] %= 10;
    }

    // 去除前导零，确定实际位数
    int actual_digit_len = max_digit_len;
    while (actual_digit_len > 1 && temp[actual_digit_len - 1] == 0) {
        actual_digit_len--;
    }

    // 修正：按位数计算字节数（至少一字节）
    int mem_len = actual_digit_len / 2 + 1;

    // 创建结果
    Bigint result;
    result.digit_len = actual_digit_len;
    result.mem_len = mem_len;
    result.p = (unsigned char*)calloc(mem_len, sizeof(unsigned char));

    // 从临时结果填充到 Bigint（从高位到低位）
    for (int i = 0; i < actual_digit_len; i++) {
        set_digit(&result, actual_digit_len - 1 - i, temp[i]);
    }

    free(temp);
    return result;
}