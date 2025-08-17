#include<stdio.h>
#include<math.h>
#define eps 0.0001
int main(){
    double h, s, v, l, k;
    int n;
    scanf("%lf %lf %lf %lf %lf %d", &h, &s, &v, &l, &k, &n);
    int ans = 0;//记录能落到车上的小球的数量
    double abletime_st, abletime_fi, carset_r, carset_l;
/*abletime_st:最早能落到车上的时间(h-k); abletime_fi:能落到车上的最晚时间(h);
carset_r:车的右边界; carset_l:车的左边界*/
    abletime_fi = sqrt(2 * (h - eps) / 10);// 计算能落到车上的最晚时间
    if(h == k){// 计算能落到车上的最早时间
        abletime_st = 0;
    }else{
        abletime_st = sqrt(2 * (h - k -eps) / 10);
    }
    carset_r = s - (abletime_st * v) + l; // 计算车右边界的位置
    carset_l = s - (abletime_fi * v);// 计算车左边界的位置
    //遍历每一辆车，判断是否能够落在车上
    for(int i = 0; i <= n - 1; i++){
        if(carset_l - eps <= i && i <= carset_r + eps)ans++;
    }//
    printf("%d",ans);
    return 0;
}
/*# P1033 [NOIP 2002 提高组] 自由落体
## 题目描述
在高为 $H$ 的天花板上有 $n$ 个小球，体积不计，位置分别为 $0,1,2,\cdots,n-1$。在地面上有一个小车（长为 $L$，高为 $K$，距原点距离为 $S_1$）。
已知小球下落距离计算公式为 $d=0.5 \times g \times (t^2)$，其中 $g=10$，$t$ 为下落时间。地面上的小车以速度 $V$ 前进。
如下图：
![](https://cdn.luogu.com.cn/upload/image_hosting/1d177dhg.png)
小车与所有小球同时开始运动，当小球距小车的距离 $\le  0.0001$ (感谢 Silver_N 修正) 时，即认为小球被小车接受（小球落到地面后不能被接受）。
请你计算出小车能接受到多少个小球。
## 输入格式
$H,S_1,V,L,K,n$（$1 \le H,S_1,V,L,K,n \le 100000$）
## 输出格式
小车能接受到的小球个数。
## 输入输出样例 #1
### 输入 #1
```
5.0 9.0 5.0 2.5 1.8 5
```
### 输出 #1
```
1
```
## 说明/提示
当球落入车的尾部时，算作落入车内。*/