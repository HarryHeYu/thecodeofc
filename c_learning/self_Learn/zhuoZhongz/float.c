#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>  // 添加stdint.h头文件以支持uint32_t类型

// 分解半精度浮点数
static void unpack_float16(uint16_t val, int *sign, int *exp, uint16_t *mantissa) {
    *sign = (val >> 15) & 0x1;
    *exp = (val >> 10) & 0x1F;
    *mantissa = val & 0x3FF;
}

// 将半精度浮点数 (16 位) 转换为单精度浮点数 (32 位)
static float float16_to_float32(uint16_t h) {
    uint16_t sign = (h >> 15) & 0x1;
    uint16_t exponent = (h >> 10) & 0x1F;
    uint16_t fraction = h & 0x3FF;
    uint32_t result;

    if (exponent == 0) { // 非规格化数或零
        if (fraction == 0) {
            result = sign << 31; // 返回正零或负零
        } else {
            // 转换为规格化数
            while ((fraction & 0x0400) == 0) {
                fraction <<= 1;
                exponent--;
            }
            fraction &= ~0x0400; // 去掉隐含的1
            exponent++;          // 调整指数
            exponent += 127 - 15; // 偏移调整
            result = (sign << 31) | (exponent << 23) | (fraction << 13);
        }
    } else if (exponent == 31) { // 无穷大或 NaN
        result = (sign << 31) | (0xFF << 23) | (fraction << 13);
    } else {
        // 规格化数
        exponent += 127 - 15; // 指数偏移调整
        result = (sign << 31) | (exponent << 23) | (fraction << 13);
    }

    return *(float *)&result;
}

// 将单精度浮点数 (32 位) 转换为半精度浮点数 (16 位)
static uint16_t float32_to_float16(float f) {
    uint32_t bits = *(uint32_t *)&f;
    uint16_t sign = (bits >> 31) & 0x1;
    int32_t exponent = ((bits >> 23) & 0xFF) - 127 + 15;
    uint32_t fraction = bits & 0x7FFFFF;

    if ((bits & 0x7FFFFFFF) == 0) { // 零
        return sign << 15;
    }
    if (exponent <= 0) { // 非规格化数或下溢
        if (exponent < -10) {
            return sign << 15; // 太小，返回零
        }
        fraction = (fraction | 0x800000) >> (1 - exponent);
        return (sign << 15) | (fraction >> 13);
    } else if (exponent >= 31) { // 溢出，返回无穷
        return (sign << 15) | (0x1F << 10);
    }

    // 规格化数
    return (sign << 15) | (exponent << 10) | (fraction >> 13);
}

// 半精度浮点数加法
static short float16_add(short a, short b) {
    float fa = float16_to_float32((uint16_t)a);
    float fb = float16_to_float32((uint16_t)b);
    float result = fa + fb;
    return (short)float32_to_float16(result);
}

// 半精度浮点数减法
static short float16_sub(short a, short b) {
    float fa = float16_to_float32((uint16_t)a);
    float fb = float16_to_float32((uint16_t)b);
    float result = fa - fb;
    return (short)float32_to_float16(result);
}

// 半精度浮点数乘法
static short float16_mul(short a, short b) {
    float fa = float16_to_float32((uint16_t)a);
    float fb = float16_to_float32((uint16_t)b);
    float result = fa * fb;
    return (short)float32_to_float16(result);
}