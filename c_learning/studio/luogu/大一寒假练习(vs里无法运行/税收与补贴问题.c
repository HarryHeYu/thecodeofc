#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define INF 1000000000//INF 被定义为一个很大的整数，用于表示无穷大，在后续代码中会用于初始化边界值。
// 最大数据点个数（包括成本这一点）
#define MAX_POINTS 105
// 全局数组存储已知价格点及对应销量
int pts[MAX_POINTS], salesArr[MAX_POINTS];
int n; // 数据点个数（第0个点为成本点）
int D; // 超出最高价后，每升高一元减少的销量
// 计算给定售价P的销量 f(P)
int getSales(int P) {
    // P 必须 >=成本 pts[0]
    if(P < pts[0]) return 0;
    if(P <= pts[n-1]) {// P小于等于最高已知价格pts[n-1]，遍历已知价格区间
    /*更多时候，给定的价格可能位于两个已知价格之间。题目假设相邻价位间销量的变化是线性的，
    所以在这种情况下可以使用线性插值的方法来估算该价格对应的销量。*/
        // 在已知区间内，找出所在区间
        // 注意：如果 P 恰好等于某个已知价格，则直接返回对应销量
        for (int i = 0; i < n - 1; i++) {
            if(P == pts[i]) return salesArr[i];
            if(P > pts[i] && P < pts[i+1]) {
                // 线性插值：销量 = salesArr[i] + (salesArr[i+1]-salesArr[i])*(P-pts[i])/(pts[i+1]-pts[i])
                //就是利用线性变化算斜率来算未知值
                int diffPrice = pts[i+1] - pts[i];
                int diffSales = salesArr[i+1] - salesArr[i];
                int fP = salesArr[i] + (diffSales * (P - pts[i])) / diffPrice;
                return fP;
            }
            if(P == pts[i+1]) return salesArr[i+1];
        }
        // 若没有匹配（理论上不应出现），返回最后一个点
        return salesArr[n-1];
    } else {
        // P > 最高给定价格 pts[n-1]
        int fP = salesArr[n-1] - D * (P - pts[n-1]);
        if(fP < 0) fP = 0;
        return fP;
    }
}
// 使用 math.h 的 ceil 与 floor 实现整除向上/向下取整
int ceil_div(int a, int b) {
    double res = ceil((double)a / (double)b);
    //ceil 函数用来计算大于或等于给定浮点数的最小整数。
    //将浮点数 x 向上取整，即返回大于或等于 x 的最小整数。
    return (int)res;
}
int floor_div(int a, int b) {
    double res = floor((double)a / (double)b);
    //将浮点数 x 向下取整，即返回小于或等于 x 的最大整数。
    return (int)res;
}
/*将整数a和b转换为double类型进行除法运算然后使用ceil或floor函数进行取整最后转换回int类型。
这是因为在后续计算补贴或税收的上下界时需要进行精确的取整操作。*/
int main(){
    int T; // 政府预期价格
    int cost, S0; // 成本与以成本价销售时的销量
    // 输入预期价格 T
    scanf("%d", &T);
    // 输入成本与以成本价销售时的销量
    scanf("%d %d", &cost, &S0);
    // 将第一个数据点设置为 (cost, S0)
    pts[0] = cost;
    salesArr[0] = S0;
    n = 1;
    // 读取后续价格及销量数据，直到输入 -1 -1
    while(1) {
        int p, s;
        scanf("%d %d", &p, &s);
        if(p == -1 && s == -1)
            break;
        pts[n] = p;
        salesArr[n] = s;
        n++;
    }
    // 最后一行输入为固定销量递减量 D
    scanf("%d", &D);
    // 题目保证产品不低于成本销售，因此 T >= cost
    if(T < cost) {
        printf("NO SOLUTION\n");
        return 0;
    }
    // 计算政府预期价格 T 时的销量
    int fT = getSales(T);
    if(fT <= 0) {
        // 如果T时销量为0则利润必为0显然不可能为最大利润（假设其他价位能获得正利润）
        printf("NO SOLUTION\n");
        return 0;
    }
    // 确定价格搜索上界：对于价格超过最高给定价格 pts[n-1]，销量 f(P)= salesArr[n-1]- D*(P-pts[n-1]) 应大于0
    int Pmax = pts[n-1] + (salesArr[n-1] - 1) / D; // 使得 f(P) >= 1
    // 用于记录 A 的约束
    int L_bound = -INF, U_bound = INF;
    //初始化补贴或税收A的的下界为无穷大
    int noSolutionFlag = 0;
    //初始化无解标志为0
    // 对 P 从成本到 Pmax 枚举所有可能售价（整数）
    for (int P = cost; P <= Pmax; P++) {
        if(P == T) continue;
        /*如果当前遍历到的价格 P 等于政府预期价格 T，则跳过本次循环，继续下一次循环。
        因为我们要比较的是除预期价格外的其他价格与预期价格下的利润情况。*/
        int fP = getSales(P);
        if(fP <= 0) continue; // 销量为0，不影响最大利润的比较
        // 若 f(T) == f(P)，则不等式变为 (T-cost)*f(T) >= (P-cost)*f(P)
        if(fT == fP) {
            if(P > T) { // 则 (P-cost) > (T-cost)，不等式无法成立
                noSolutionFlag = 1;
                break;
        /*如果当前价格P大于政府预期价格T由于fT == fP那么(P - cost)*fP必然大于
        (T - cost)*fT，即无法满足在预期价格T下获得最大利润的条件*/
            }
            // 如果 P < T，则 (P-cost) < (T-cost)，不等式总成立，无约束
            continue;
        }
        int delta = fT - fP; // 分母
        //delta 表示预期价格销量与当前价格销量的差值
        int R = (P - cost) * fP - (T - cost) * fT;
        //R 用于后续推导补贴或税收 A 的取值范围。
        if(delta > 0) {
            // 得到 A >= R/delta
            int L_i = ceil_div(R, delta);
            if(L_i > L_bound) L_bound = L_i;
        } else { // delta < 0
            int U_i = floor_div(R, delta);
            if(U_i < U_bound) U_bound = U_i;
        }
    }
    if(noSolutionFlag || L_bound > U_bound) {
        //如果无解标志 noSolutionFlag 为 1，或者补贴或税收 A 的下界 L_bound 大于上界 U_bound，说明不存在满足条件的补贴或税收
        printf("NO SOLUTION\n");
        return 0;
    }
    // 在 [L_bound, U_bound] 内找一个使 |A| 最小的整数
    int ans;
    if(L_bound <= 0 && U_bound >= 0) {
        ans = 0;
    } else if(U_bound < 0) {
        // 区间全为负数，则取较大的（绝对值较小）的 U_bound
        ans = U_bound;
    } else { // L_bound > 0
        ans = L_bound;
    }
    printf("%d\n", ans);
    return 0;
}
/*# [NOIP 2000 普及组] 税收与补贴问题
## 题目背景
每样商品的价格越低，其销量就会相应增大。现已知某种商品的成本及其在若干价位上的销量（产品不会低于成本销售），
并假设相邻价位间销量的变化是线性的且在价格高于给定的最高价位后，销量以某固定数值递减。（我们假设价格及销售量都是整数）
对于某些特殊商品，不可能完全由市场去调节其价格。这时候就需要政府以税收或补贴的方式来控制。（所谓税收或补贴就是对于每个产品收取或给予生产厂家固定金额的货币）
## 题目描述
你是某家咨询公司的项目经理，现在你已经知道政府对某种商品的预期价格，以及在各种价位上的销售情况。
要求你确定政府对此商品是应收税还是补贴的最少金额（也为整数），才能使商家在这样一种政府预期的价格上，获取相对其他价位上的最大总利润。
- 总利润 $=$ 单位商品利润 $\times$ 销量
- 单位商品利润 $=$ 单位商品价格 $-$ 单位商品成本（减去税金 或者 加上补贴）
## 输入格式
输入的第一行为政府对某种商品的预期价；
 第二行有两个整数，第一个整数为商品成本，第二个整数为以成本价销售时的销售量；

 接下来若干行，每行都有两个整数，第一个为某价位时的单价，第二个为此时的销量，以一行 `-1 -1` 表示所有已知价位及对应的销量输入完毕；

 输入的最后一行为一个单独的整数表示在已知的最高单价外每升高一块钱将减少的销量。
## 输出格式
输出有两种情况：若在政府预期价上能得到最大总利润，则输出一个单独的整数，数的正负表示是补贴还是收税，数的大小表示补贴或收税的金额最小值。若有多解，取绝对值最小的输出。
如在政府预期价上不能得到最大总利润，则输出 `NO SOLUTION`。
## 样例 #1
### 样例输入 #1
```
31
28 130
30 120
31 110
-1  -1
15
```
### 样例输出 #1
```
4
```
## 提示
### 数据范围及约定
保证输入的所有数字均小于 $10^5$。
### 样例解释（2023/6/22 更新）
如下图所示是输入样例所对应的价格变化图，横轴表示销售价格，纵轴表示销量。
![](https://cdn.luogu.com.cn/upload/image_hosting/21mhtm5i.png)
根据题意，$28$ 元是商品的成本。销售价格不应该低于 $28$ 元；当销售价格大于给出的价格的最大值 $31$ 元后，
按照售价每提高一元，销量降低 $15$ 计算，例如当售价为 $33$ 元时，销量为 $110-15\times (33-31)=80$。在给出来的价位之间，销量呈线性变化。
当政府给该商品补贴 $4$ 元后，企业将该商品定价为 $31$ 元时，取得的利润为 $31-28+4=7$ 元，销量为 $110$ 件，
总利润为 $7\times 110=770$ 元，是企业在所有定价下能够取得的最大的总利润。此时企业的售价为政府的期望售价，因此是一个合法方案。*/