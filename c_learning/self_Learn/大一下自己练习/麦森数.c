#include<stdio.h>
#include<math.h>
#include<string.h>
//定义最大位数，501是为了处理可能产生的进位
#define MAX_DIGITS 501
//将大整数v[]清零
void bigClear(int v[]){
    //使用memset函数将数组v的所有元素清零
    memset(v,0,sizeof(int)*MAX_DIGITS);
/*void *memset(void *s, int c, size_t n);
s：指向要填充的内存块的指针，即目标内存区域的起始地址。
c：要设置的值，该值会被转换为 unsigned char 类型。
n：要填充的字节数，即从起始地址开始，需要填充的内存区域的大小。*/
}
//用整数val初始化大整数v[]
//将一个普通整数 val 转换为大整数并存储在数组 v 中。
void bigFromInt(int v[],int val){
    //先将数组v清零
    bigClear(v);
    int i = 0;
    //将整数val逐位拆分存储到数组v中，低位在前
    while(val > 0 && i < MAX_DIGITS){
        v[i++] = val % 10;
        val /= 10;
    }
}
//计算(vA * vB) mod 10^500，结果放入vRes
void bigMul(const int vA[], const int vB[], int vRes[]){
    //暂存结果到一个临时数组里，长度最多2 * MAX_DIGITS位，但只需保留500+几位
    static int tmp[2 * MAX_DIGITS];
    //将临时数组清零
    memset(tmp,0,sizeof(tmp));
    //逐位计算乘积，结果存入tmp
    for(int i = 0; i < MAX_DIGITS; i++){
        if(vA[i] == 0) continue;
        int carry = 0;
        for(int j = 0; j < MAX_DIGITS; j++){
            //如果vB[j]为0且没有进位，则跳过本次乘法
            if(vB[j] == 0 && carry == 0) continue;
            //计算当前位的乘积和进位
            int sum = tmp[i + j] + vA[i] * vB[j] + carry;
//这里的 tmp[i + j] 一开始是 0，vA[i] * vB[j] 是当前位的乘积，carry 是上一次计算产生的进位。
            tmp[i + j] = sum % 10;
//把 sum 对 10 取模，得到当前位的结果，然后存储在 tmp[i + j] 中。
            carry = sum / 10;
//把 sum 除以 10，得到进位，供下一次计算使用。
/*在后续的内层循环中，tmp[i + j] 可能已经被之前的计算更新过了
。也就是说，tmp[i + j] 存储的是之前部分乘积累加的结果。
在每次计算 sum 时，都会把 tmp[i + j] 考虑进去，从而实现多位乘法结果的累加。*/
        }
    }
    //只保留后500位
    for(int i = 0; i < MAX_DIGITS; i++){
        vRes[i] = tmp[i];
    }
}
//拷贝大整数scr到dst
void bigCopy(int dst[], const int src[]){
    //逐位将src数组的元素复制到dst数组中
    for(int i = 0; i < MAX_DIGITS; i++){
        dst[i] = src[i];
    }
}
//快速幂：计算2^p mod 10^500，保存在result中
void bigPower2(int p, int result[]){
    //result初始化为1
    bigFromInt(result,1);
    //base初始化为2
    static int base[MAX_DIGITS];
    bigFromInt(base,2);
/*这里将 result 数组初始化为表示整数 1 的大整数，
将 base 数组初始化为表示整数 2 的大整数。因为任何数的 0 次幂为 1，
所以结果初始化为 1，而底数为 2。*/
    while(p > 0){
        if(p & 1){
//用于判断 p 的二进制表示的最低位是否为 1。如果结果为 1，则说明 p 是奇数。
            //按位与运算符 & 是一个二元运算符，用于对两个操作数的对应二进制位进行逻辑与运算。
            //逻辑与运算的规则是：只有当两个对应位都为 1 时，结果位才为 1，否则为 0。
            //当 p 为奇数时，需要将当前结果 result 乘以底数 base。
            static int temp[MAX_DIGITS];
            bigClear(temp);
            bigMul(result,base,temp);
            bigCopy(result,temp);
        }
        //无论 p 是奇数还是偶数，每次循环都要将底数 base 平方。
        static int tempB[MAX_DIGITS];
        bigClear(tempB);
        bigMul(base,base,tempB);
        bigCopy(base,tempB);
        //P右移一位,相当于除以2
        //这是快速幂算法的核心步骤之一，通过不断将指数减半来减少计算次数。
        p >>= 1;
    }
}
//大整数v减去1（v>0)
void bigDec(int v[]){
//低位在前，高位在后。例如，数字 123 会存储为 v[0] = 3，v[1] = 2，v[2] = 1。
    int i = 0;
    while(i < MAX_DIGITS){
        if(v[i] == 0){
            //如果当前位为0，借位并将该位设为9
            v[i] = 9;
            i++;
        }else{
            //当前位不为0，直接减1
            v[i]--;
            break;
        }
    }
}
//计算位数：digits = floor(p * log10(2)) + 1
int countDigits(int p){
    //因为 p > 1000.可直接使用double计算位数
    double d = p * log10(2);
    //是使用10^n 来夹逼后求对在取整的结果
    //这么算是对的，不懂的话再查一下
    return (int)d + 1;
}
int main(){
    int p;
    scanf("%d",&p);
    //计算2^p - 1的位数
    int digits = countDigits(p);
    //计算(2^p) mod 10^500
    static int v[MAX_DIGITS];
    bigClear(v);
    bigPower2(p,v);
    //将结果减去1
    bigDec(v);
    //输出位数
    printf("%d\n",digits);
    //输出最后500位(含不足时的前置补0)
    //v[0]是最低位，v[499]是最高位(在500范围内)
    //总共打印10行，每行50个数字
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 50; j++){
            int index = 499 - (i * 50 + j);
            if(index < 0){
                //超过已有位数，用'0'补
                putchar('0');
            }else{
                //输出对应位的数字
                putchar(v[index] + '0');
            }
        }
        //每行输出结束后换行
        putchar('\n');
    }
    return 0;
}
/*# P1045 [NOIP 2003 普及组] 麦森数
## 题目描述
形如 $2^{P}-1$ 的素数称为麦森数，这时 $P$ 一定也是个素数。但反过来不一定，即如果 $P$ 是个素数，$2^{P}-1$ 不一定也是素数
。到 1998 年底，人们已找到了 37 个麦森数。最大的一个是 $P=3021377$，它有 909526 位。麦森数有许多重要应用，它与完全数密切相关。
任务：输入 $P(1000<P<3100000)$，计算 $2^{P}-1$ 的位数和最后 $500$ 位数字（用十进制高精度数表示）
## 输入格式
文件中只包含一个整数 $P(1000<P<3100000)$
## 输出格式
第一行：十进制高精度数 $2^{P}-1$ 的位数。
第 $2\sim 11$ 行：十进制高精度数 $2^{P}-1$ 的最后 $500$ 位数字。（每行输出 $50$ 位，共输出 $10$ 行，不足 $500$ 位时高位补 $0$）
不必验证 $2^{P}-1$ 与 $P$ 是否为素数。
## 输入输出样例 #1
### 输入 #1
```
1279
```
### 输出 #1
```
386
00000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000
00000000000000104079321946643990819252403273640855
38615262247266704805319112350403608059673360298012
23944173232418484242161395428100779138356624832346
49081399066056773207629241295093892203457731833496
61583550472959420547689811211693677147548478866962
50138443826029173234888531116082853841658502825560
46662248318909188018470682222031405210266984354887
32958028878050869736186900714720710555703168729087
```
*/