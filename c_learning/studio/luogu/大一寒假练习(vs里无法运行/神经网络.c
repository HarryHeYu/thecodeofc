#include<stdio.h>
#include<stdlib.h>
#define MAX_N 105
//定义边的结构
typedef struct Edge{
    int to;
    int weight;
    struct Edge* next;//这是一个指向 Edge 结构体的指针，用于构建邻接表。在邻接表中，
    //每个神经元对应一个链表，next 指针指向下一条从该神经元出发的边，通过这种方式可以方便地存储和遍历一个神经元的所有出边。
}Edge;
//定义神经元结构体
typedef struct Neuron{
    int C;//当前状态
    int U;//阈值
    int in_degree;//入度
/*指的是该神经元的入度，即有多少条边指向这个神经元。在拓扑排序中，入度用于确定神经元处理的顺序，入度为 0 的神经元是输入层神经元，首先被处理。*/
    int is_output;//是否为输出层神经元: 不是输出层神经元，将 is_output 标记为 0；否则标记为 1。
    Edge* head;//邻接表表头
}Neuron;

Neuron neurons[MAX_N];
int n, p;
//加边
void addEdge(int from, int to, int weight){
    Edge* newEdge = (Edge*)malloc(sizeof(Edge));
    newEdge->to = to;
    newEdge->weight = weight;
    newEdge->next = neurons[from].head;
    neurons[from].head = newEdge;
}
//拓扑排序模拟神经网络运作
void simulate(){
    int queue[MAX_N];
    int front = 0, rear = 0;//front表示队首，rear表示队尾
/*rear 主要用于控制元素入队的位置。当有新元素需要加入队列时，将元素存储在 queue[rear] 位置，然后 rear 的值加 1，表示队尾向后移动一位，为下一个元素入队做好准备。
front 用于控制元素出队的位置。当需要从队列中取出元素进行处理时，取出 queue[front] 位置的元素，然后 front 的值加 1，表示队首向后移动一位，将下一个元素作为新的队首。*/
    //初始化队列，将输入层神经元加入队列
    for(int i = 1; i <= n; i++){//遍历所有神经元，神经元编号从 1 到 n
        if(neurons[i].in_degree == 0)queue[rear++] = i;
    }
    while(front < rear){
        int current = queue[front++];
        if(neurons[current].C <= 0)continue;//平静状态不传递信号
        Edge* edge = neurons[current].head;
        while(edge != NULL){
            // 获取当前边指向的目标神经元编号
            int next = edge->to;
            // 根据公式更新目标神经元的状态 C
            // 目标神经元的状态加上当前神经元的状态乘以边的权值
            neurons[next].C += edge->weight * neurons[current].C;
            neurons[next].in_degree--;// 目标神经元的入度减 1，表示它已经收到了当前神经元的信号
            if(neurons[next].in_degree == 0){// 如果目标神经元的入度变为 0，说明它的所有输入信号都已处理完毕
                neurons[next].C-= neurons[next].U;// 根据公式，减去目标神经元的阈值 U
                queue[rear++] = next;// 将目标神经元的编号加入队列尾部，等待后续处理
/*虽然 next 可能已经在之前的某个时刻被考虑过，但只有当它的入度变为 0 时，才真正具备了进行状态更新和传递信号的条件。将其加入队列的目的是：
状态更新：在加入队列之前，next 神经元的状态只是部分更新（累加了部分输入信号）。
当入度变为 0 时，需要减去其阈值 U 来完成最终的状态更新，代码 neurons[next].C -= neurons[next].U; 就是做这个操作的。
信号传递：更新状态后，如果 next 处于兴奋状态（C > 0），它会继续向其相邻的神经元传递信号。
通过将其加入队列，后续可以继续处理它的出边，更新相邻神经元的状态，从而实现信息在神经网络中的逐层传递。*/
            }
            edge = edge->next;// 移动到下一条边
        }
    }
}
int main(){
    scanf("%d %d", &n, &p);
     // 初始化神经元
     for (int i = 1; i <= n; i++) {
        scanf("%d %d", &neurons[i].C, &neurons[i].U);
        neurons[i].in_degree = 0;
        neurons[i].is_output = 1;
        neurons[i].head = NULL;
     }
     // 读取边信息
    for (int i = 0; i < p; i++) {
        int from, to, weight;
        scanf("%d %d %d", &from, &to, &weight);
        addEdge(from, to, weight);
        neurons[to].in_degree++;
        neurons[from].is_output = 0;  // 有出边的不是输出层
    }
    // 模拟神经网络运作
    simulate();
    // 输出结果
    int has_output = 0;
    for (int i = 1; i <= n; i++) {
        // 如果第 i 个神经元是输出层神经元，并且其状态 C 大于 0（处于兴奋状态）
        if (neurons[i].is_output && neurons[i].C > 0) {
            printf("%d %d\n", i, neurons[i].C);
            has_output = 1;
        }
    }
    if (!has_output) {
        printf("NULL\n");
    }
    // 释放内存
    for (int i = 1; i <= n; i++) {
        Edge* edge = neurons[i].head;
        while (edge != NULL) {
            Edge* temp = edge;
            edge = edge->next;
            free(temp);
        }
    }

    return 0;
}
/*# P1038 [NOIP 2003 提高组] 神经网络
## 题目背景
人工神经网络（Artificial Neural Network）是一种新兴的具有自我学习能力的计算系统，在模式识别、函数逼近及贷款风险评估等诸多领域有广泛的应用。
对神经网络的研究一直是当今的热门方向，兰兰同学在自学了一本神经网络的入门书籍后，提出了一个简化模型，他希望你能帮助他用程序检验这个神经网络模型的实用性。
## 题目描述
在兰兰的模型中，神经网络就是一张有向图，图中的节点称为神经元，而且两个神经元之间至多有一条边相连，下图是一个神经元的例子：
![](https://cdn.luogu.com.cn/upload/image_hosting/61qm40kj.png)
神经元（编号为 $i$）
图中，$X_1 \sim X_3$ 是信息输入渠道，$Y_1 \sim Y_2$ 是信息输出渠道，$C_i$ 表示神经元目前的状态，$U_i$ 是阈值，可视为神经元的一个内在参数。
神经元按一定的顺序排列，构成整个神经网络。在兰兰的模型之中，神经网络中的神经元分为几层；
称为输入层、输出层，和若干个中间层。每层神经元只向下一层的神经元输出信息，只从上一层神经元接受信息。下图是一个简单的三层神经网络的例子。
![](https://cdn.luogu.com.cn/upload/image_hosting/4xd7f8yz.png)
兰兰规定，$C_i$ 服从公式：（其中 $n$ 是网络中所有神经元的数目）
$$C_i=\left(\sum\limits_{(j,i) \in E} W_{ji}C_{j}\right)-U_{i}$$
公式中的 $W_{ji}$（可能为负值）表示连接 $j$ 号神经元和 $i$ 号神经元的边的权值。当 $C_i$ 大于 $0$ 时，
该神经元处于兴奋状态，否则就处于平静状态。当神经元处于兴奋状态时，下一秒它会向其他神经元传送信号，信号的强度为 $C_i$。
如此．在输入层神经元被激发之后，整个网络系统就在信息传输的推动下进行运作。现在，给定一个神经网络，
及当前输入层神经元的状态（$C_i$），要求你的程序运算出最后网络输出层的状态。
## 输入格式
输入文件第一行是两个整数 $n$（$1 \le n \le 100$）和 $p$。接下来 $n$ 行，每行 $2$ 个整数，第 $i+1$ 行是神经元 $i$ 最初状态和其阈值（$U_i$），
非输入层的神经元开始时状态必然为 $0$。再下面 $p$ 行，每行有两个整数 $i,j$ 及一个整数 $W_{ij}$，表示连接神经元 $i,j$ 的边权值为 $W_{ij}$。
## 输出格式
输出文件包含若干行，每行有 $2$ 个整数，分别对应一个神经元的编号，及其最后的状态，$2$ 个整数间以空格分隔。
仅输出最后状态大于 $0$ 的输出层神经元状态，并且按照编号由小到大顺序输出。
若输出层的神经元最后状态均小于等于 $0$，则输出 `NULL`。
## 输入输出样例 #1
### 输入 #1
```
5 6
1 0
1 0
0 1
0 1
0 1
1 3 1
1 4 1
1 5 1
2 3 1
2 4 1
2 5 1
```
### 输出 #1
```
3 1
4 1
5 1
```*/