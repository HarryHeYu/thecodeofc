#include<stdio.h>
#include<stdlib.h>
typedef struct{
    double distance;//加油站离出发点的距离
    double price;//该加油站每升汽油的价格
}Station;
int compare(const void* a, const void* b);
int compare(const void* a, const void* b){
    Station *s1 = (Station *)a;
    Station *s2 = (Station *)b;
    if(s1->distance < s2->distance)return -1;
    if(s1->distance > s2->distance)return 1;
    return 0;
}
int main(){
    double D1, C, D2, P;
    int N;
    scanf("%lf %lf %lf %lf %ld",&D1,&C,&D2,&P,&N);
    Station stations[8];//存出加油站信息，N最大为6，再加上终点和起点总共8个
    stations[0].distance = 0;
    stations[0].price = P;
    for(int i = 0; i <= P; i++){
        scanf("%lf %lf",&stations[i].distance,&stations[i].price);
    }
    stations[N+1].distance = D1;
    stations[N+1].price = 0;
    qsort(stations, N+2, sizeof(Station), compare);//按距离对加油站从小到大排序
    //检查是否存在无法到达的情况
    for(int i = 0; i < N + 1; i++){
        if(stations[i+1].distance - stations[i].distance > C * D2){
            printf("No Solution\n");
            return 0;
        }
    }
    int current = 0;//当前所在加油站的索引
    double cost = 0;//总花费
    double gas = 0;//当前油箱中的油量
    while(current < N +1){
        int next = -1;//下一个要去的加油站的索引
        double min_price = 1e9;//用于记录下各加油站的最低油价，初始化为一亿
        //寻找在可行驶范围内油价更低的加油站
        for(int i = current + 1; i <= N + 1; i++){
            if(stations[i].distance - stations[current].distance <= C * D2){
                if(stations[i].price < stations[current].price){
                    next = i;
                    break;
                }
                if(stations[i].price < min_price){
                    min_price = stations[i].price;
                    next = i;
                }
            }else{
                break;
            }
        }
        //计算需要加的油量
        double need = (stations[next].distance - stations[current].distance)/D2 - gas;
        if(stations[next].price < stations[current].price){
            //如果下个加油站油价更低，只加刚好能到下一个加油站的油
            cost += need * stations[current].price;
            gas = 0;
        }else{
            //如果下一个油站油价更高，把油箱加满
            cost += (C - gas) * stations[current].price;
            gas = C - (stations[next].distance - stations[current].distance)/D2;
        }
        current = next;
    }
    printf("%.2lf\n",cost);
    return 0;
}
/*# [NOIP 1999 提高组] 旅行家的预算
## 题目描述
一个旅行家想驾驶汽车以最少的费用从一个城市到另一个城市（假设出发时油箱是空的）。
给定两个城市之间的距离 $D_1$、汽车油箱的容量 $C$（以升为单位）、每升汽油能行驶的距离 $D_2$、
出发点每升汽油价格$P$和沿途油站数 $N$（$N$ 可以为零），油站 $i$ 离出发点的距离 $D_i$、每升汽油价格 $P_i$（$i=1,2,…,N$）。
计算结果四舍五入至小数点后两位。如果无法到达目的地，则输出 `No Solution`。
## 输入格式
第一行，$D_1$，$C$，$D_2$，$P$，$N$。
接下来有 $N$ 行。
第 $i+1$ 行，两个数字，油站 $i$ 离出发点的距离 $D_i$ 和每升汽油价格 $P_i$。
## 输出格式
所需最小费用，计算结果四舍五入至小数点后两位。如果无法到达目的地，则输出 `No Solution`。
## 样例 #1
### 样例输入 #1
```
275.6 11.9 27.4 2.8 2
102.0 2.9
220.0 2.2
```
### 样例输出 #1
```
26.95
```
## 提示
$N \le 6$，其余数字 $ \le 500$。
NOIP1999 普及组第三题、提高组第三题*/