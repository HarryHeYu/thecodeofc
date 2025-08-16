#include<stdio.h>
#include<math.h>
double f(double a, double b, double c, double d, double x){
    return a * x * x * x + b * x * x + c * x + d;
}
//定义查找函数，用于在[left,right]上找方程的根（二分法）
double binarySearch(double a, double b, double c, double d
    ,double left, double right){
        double mid;
        while(right - left >= 1e-6){
/*使用 right - left > 1e-6 的原因
精度控制：如前面所讲，由于浮点数在计算机中的存储和计算存在误差，我们不能要求找到的根是绝对精确的。
设置 1e - 6 这样一个精度阈值，意味着当区间长度小于这个值时，我们认为区间内的任意值都可以近似看作方程的根，
并且这个精度足以保证最终输出的结果精确到小数点后 2 位。避免无限循环：如果区间长度已经非常小，
继续进行二分查找不会显著提高结果的精度，反而会增加不必要的计算量。使用 right - left > 1e-6 可以在区间长度达到一定精度后及时终止循环。
3. 使用 right - left >= 0 会产生的问题
无限循环：当 right 和 left 非常接近但还未相等时，right - left >= 0 始终成立，二分查找会一直进行下去，
无法终止。因为在浮点数运算中，即使两个数在数学意义上已经非常接近，但由于舍入误差，它们可能永远不会严格相等，这就会导致程序陷入无限循环。
精度失控：即使最终 right 和 left 相等，也可能会进行过多不必要的迭代，而且无法保证结果的精度。因为没有精度控制，
程序会不断地将区间二等分，可能会进行大量的计算而没有实际意义。*/
            mid = (left + right) / 2;
            if(f(a,b,c,d,mid) * f(a,b,c,d,left) <= 0){
                right = mid;
            }else{
                left = mid;
            }
        }
        return left;//此时既可返回left也可以返回right，因为此时两值已经非常接近了，<=1e-6
    }
int main(){
    double a, b, c, d;
    scanf("%lf %lf %lf %lf",&a, &b, &c, &d);
    int count = 0;
    for(double i = -100; i < 100; i++){
        double left = i, right = i + 1;
        double y_1 = f(a,b,c,d,left), y_2 = f(a,b,c,d,right);
        if(y_1 == 0){//如果了left是根
            printf("%.2lf ",left);
            count++;
        }else if(y_1 * y_2 < 0){
            double root = binarySearch(a,b,c,d,left,right);
            printf("%.2lf ",root);
            count++;
        }
        if(count == 3)break;
    }
    if(f(a,b,c,d,100) == 0)printf("%.2lf",100.00);//检查右端点是否为根
    return 0;
}
/*# [NOIP 2001 提高组] 一元三次方程求解
## 题目描述
有形如：$a x^3 + b x^2 + c x + d = 0$  这样的一个一元三次方程。给出该方程中各项的系数（$a,b,c,d$ 均为实数），并约定该方程存在三个不同实根
（根的范围在 $-100$ 至 $100$ 之间），且根与根之差的绝对值 $\ge 1$。要求由小到大依次在同一行输出这三个实根(根与根之间留有空格)，并精确到小数点后 $2$ 位。
提示：记方程 $f(x) = 0$，若存在 $2$ 个数 $x_1$ 和 $x_2$，且 $x_1 < x_2$，$f(x_1) \times f(x_2) < 0$，则在 $(x_1, x_2)$ 之间一定有一个根。
## 输入格式
一行，$4$ 个实数 $a, b, c, d$。
## 输出格式
一行，$3$ 个实根，从小到大输出，并精确到小数点后 $2$ 位。
## 样例 #1
### 样例输入 #1
```
1 -5 -4 20
```
### 样例输出 #1
```
-2.00 2.00 5.00
```
## 提示
**【题目来源】**
NOIP 2001 提高组第一题*/