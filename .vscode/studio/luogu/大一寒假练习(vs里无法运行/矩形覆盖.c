#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
typedef struct{
    int x;
    int y;
}Point;

typedef struct{
    int x_min, x_max;
    int y_min, y_max;
    int count;
}Group;
/*Group 结构体：用于表示一个矩形组，其中 x_min 和 x_max 分别表示矩形在 x 轴方向的最小和最大坐标，
y_min 和 y_max 分别表示矩形在 y 轴方向的最小和最大坐标，count 表示该矩形组内包含的点的数量。*/
Point points[50];
int n, k;
int min_area = INT_MAX;

int compare(const void* a, const void *b){
    Point *p1 = (Point *)a;
    Point *p2 = (Point *)b;
    return(p1->x + p1->y) - (p2->x + p2->y);
}
//主要是为了对 points 数组中的点进行排序，优先处理外围的点
int max(int a, int b){return a > b ? a : b;}
int min(int a, int b){return a < b ? a : b;}

void backtrack(int idx, Group groups[]){
    if(idx == n){
        int total = 0;
        // 计算所有矩形组的总面积
        for(int i = 0; i < k; i++){
            if(groups[i].count == 0)continue;// 如果该组没有点，跳过
            int dx = groups[i].x_max - groups[i].x_min;
            int dy = groups[i].y_max - groups[i].y_min;
            total += dx * dy;
        }
        // 如果当前总面积小于之前记录的最小面积，则更新最小面积
        if(total < min_area)min_area = total;
        return;
    }
/*对于每个点，会尝试将其分配到 k 个矩形组中的一个。在分配时，
会维护一个 new_group 变量来表示将点加入某个矩形组后的新状态，而不会直接修改原有的 groups 数组。
这里只是在 new_group 这个临时变量上进行修改，不会影响到其他矩形组的状态，确保了每个点在每次尝试分配时都是独立处理的。*/
    Point p = points[idx];// 获取当前要处理的点
    // 尝试将当前点分配到 k 个矩形组中的每一个(其目的是通过穷举的方式，探索所有可能的点分配方案)
    for(int i = 0; i < k; i++){
        Group new_group = groups[i]; // 创建一个新的矩形组副本，避免修改原始的矩形组信息
        if(new_group.count == 0){// 如果该矩形组还没有点
            // 将当前点的坐标作为矩形组的边界
            new_group.x_min = new_group.x_max = p.x;
            new_group.y_min = new_group.y_max = p.y;
            new_group.count = 1;
        }else{
            // 更新矩形组的边界，使其能够包含当前点
            new_group.x_min = min(new_group.x_min, p.x);
            new_group.x_max = max(new_group.x_max,p.x);
            new_group.y_min = min(new_group.y_min, p.y);
            new_group.y_max = max(new_group.y_max,p.y);
        }
        //检查与其他组的重叠
        /*虽然重叠检查主要是为了确保矩形组之间不重叠，但也从侧面避免了点的重复纳入。
        因为如果一个点被重复纳入不同的矩形组，很可能会导致矩形组之间出现重叠。*/
        int overlap = 0;
        for(int j = 0; j < k; j++){
            if(j == i || groups[j].count == 0)continue;//跳过自身和空组
            Group g = groups[j];
            // 判断两个矩形是否重叠
            if(!(new_group.x_max < g.x_min || new_group.x_min > g.x_max 
            ||new_group.y_max < g.y_min || new_group.y_min > g.y_max)){
                overlap = 1;
                break;
            }
        }
        // 如果存在重叠，跳过当前分配方案
        if(overlap)continue;
        //计算当前面积和，剪枝
        int current_area = 0;
        for(int j = 0; j < k; j++){
            if(j == i){
                // 计算新矩形组的面积
                int dx = new_group.x_max - new_group.x_min;
                int dy = new_group.y_max - new_group.y_min;
                current_area += dx * dy;
            }else if(groups[j].count > 0){
                // 计算其他非空矩形组的面积
                int dx = groups[j].x_max - groups[j].x_min;
                int dy = groups[j].y_max - groups[j].y_min;
                current_area += dx * dy;
            }
        }
        // 如果当前总面积已经大于等于之前记录的最小面积，进行剪枝，跳过当前分配方案
        if(current_area >= min_area)continue;
        //保存状态递归
/*在递归调用 backtrack 函数处理完一个点的分配后，会进行回溯操作，将矩形组的状态恢复到之前的状态。
通过这种方式，当尝试将一个点分配到某个矩形组并递归处理后续点后，会把该矩形组的状态还原，
避免对后续其他矩形组的分配尝试产生干扰，保证每个点在不同的分配尝试中是相互独立的，不会被重复分配。*/
        Group old_group = groups[i];
        groups[i] = new_group;
        backtrack(idx+1, groups);
        groups[i] = old_group;// 回溯操作，恢复原始的矩形组信息
    }
}

int main(){
    scanf("%d %d", &n, &k);
    for(int i = 0; i < n; i++){
        scanf("%d %d", &points[i].x, &points[i].y);
    }
    //预处理排序，优先处理外围的点
    qsort(points, n, sizeof(Point), compare);
    Group groups[4] = {0};
    for(int i = 0; i < k; i++){
        groups[i].x_min = INT_MAX;
        groups[i].x_max = -1;
        groups[i].y_min = INT_MAX;
        groups[i].y_max = -1;
        groups[i].count = 0;
    }
    backtrack(0, groups);
/*对于每个点 p，都会遍历 k 个矩形组，尝试将其加入其中一个组。如果某个组为空，就以该点为基础初始化这个组；如果组不为空，就更新组的边界以包含该点。
通过这种方式，在回溯的过程中会尝试所有可能的分组情况，从而确保 k 个矩形组能够覆盖所有的点。*/
    printf("%d\n",min_area);
    return 0;
}
/*# P1034 [NOIP 2002 提高组] 矩形覆盖
## 题目描述
在平面上有 $n$ 个点，每个点用一对整数坐标表示。例如：当 $n=4$ 时，$4$ 个点的坐标分别为：$p_1(1,1)$，$p_2(2,2)$，$p_3(3,6)$，$p_4(0,7)$，见图一。
![](https://cdn.luogu.com.cn/upload/image_hosting/dxc1c5k9.png)
这些点可以用 $k$ 个矩形全部覆盖，矩形的边平行于坐标轴。当 $k=2$ 时，可用如图二的两个矩形 $s_1,s_2$ 覆盖，$s_1,s_2$ 面积和为 $4$。
问题是当 $n$ 个点坐标和 $k$ 给出后，怎样才能使得覆盖所有点的 $k$ 个矩形的面积之和为最小呢？  
约定：覆盖一个点的矩形面积为 $0$；覆盖平行于坐标轴直线上点的矩形面积也为 $0$。各个矩形必须完全分开（边线与顶点也都不能重合）。
## 输入格式
第一行共两个整数 $n,k$，含义如题面所示。
接下来 $n$ 行，其中第 $i+1$ 行有两个整数 $x_i,y_i$，表示平面上第 $i$ 个点的坐标。
## 输出格式
共一行一个整数，为满足条件的最小的矩形面积之和。
## 输入输出样例 #1
### 输入 #1
```
4 2
1 1
2 2
3 6
0 7
```
### 输出 #1
```
4
```
## 说明/提示
对于 $100\%$ 数据，满足 $1\le n \le  50$，$1 \le k \le 4$，$0 \le x_i,y_i  \le 500$。*/