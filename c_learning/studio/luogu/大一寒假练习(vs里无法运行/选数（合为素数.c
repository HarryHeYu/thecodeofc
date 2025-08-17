#include <stdio.h>
#include <stdbool.h>

#define MAXN 20

int n, k;
int nums[MAXN];
int count = 0;

// 判断一个数是否为素数
bool isPrime(int num) {
    if (num < 2) return false;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return false;
    }
    return true;
}
/*然后从 2 开始到  进行遍历，如果 num 能被其中任何一个数整除，则返回 false，否则返回 true。*/

// 深度优先搜索函数
void dfs(int start, int selected, int sum) {
    if (selected == k) {
        if (isPrime(sum)) {
            count++;
        }
        return;
    }
    for (int i = start; i < n; i++) {
        dfs(i + 1, selected + 1, sum + nums[i]);
    }
}
/*int start：表示当前可以选择的数的起始索引。在递归过程中，为了避免重复的组合，每次选择的数要从 start 开始往后选。
int selected：表示已经选择的数的个数。当 selected 等于 k 时，说明已经选够了 k 个数，此时可以进行和是否为素数的判断。
int sum：表示已经选择的数的累加和。随着递归的进行，每次选择一个新的数后，都会将其加入到 sum 中。
当 selected 等于 k 时，意味着已经从 n 个数中选出了 k 个数。此时调用 isPrime 函数来判断这 k 个数的和 sum 是否为素数。
如果 sum 是素数，则将全局变量 count 加 1，count 用于记录和为素数的组合的数量。
最后使用 return 语句终止当前递归调用，返回上一层。
使用 for 循环从 start 开始到 n - 1 进行遍历，i 表示当前要选择的数的索引。
在每次循环中，递归调用 dfs 函数：
i + 1：作为下一次递归调用的 start 参数，确保下次选择的数从当前数的下一个开始，避免出现重复的组合。例如，已经选了第 i 个数，下次就从第 i + 1 个数开始选。
selected + 1：表示已经选择的数的个数增加了 1。
sum + nums[i]：将当前选择的数 nums[i] 加入到 sum 中，更新累加和。*/
int main() {
    // 读取输入
    scanf("%d %d", &n, &k);
    for (int i = 0; i < n; i++) {
        scanf("%d", &nums[i]);
    }

    // 调用深度优先搜索函数
    dfs(0, 0, 0);

    // 输出结果
    printf("%d\n", count);

    return 0;
}
/*# P1036 [NOIP 2002 普及组] 选数
## 题目描述
已知 $n$ 个整数 $x_1,x_2,\cdots,x_n$，以及 $1$ 个整数 $k$（$k<n$）。
从 $n$ 个整数中任选 $k$ 个整数相加，可分别得到一系列的和。例如当 $n=4$，$k=3$，$4$ 个整数分别为 $3,7,12,19$ 时，可得全部的组合与它们的和为：
$3+7+12=22$
$3+7+19=29$
$7+12+19=38$
$3+12+19=34$
现在，要求你计算出和为素数共有多少种。
例如上例，只有一种的和为素数：$3+7+19=29$。
## 输入格式
第一行两个空格隔开的整数 $n,k$（$1 \le n \le 20$，$k<n$）。
第二行 $n$ 个整数，分别为 $x_1,x_2,\cdots,x_n$（$1 \le x_i \le 5\times 10^6$）。
## 输出格式
输出一个整数，表示种类数。
## 输入输出样例 #1
### 输入 #1
```
4 3
3 7 12 19
```
### 输出 #1
```
1
```*/