/*Huffman Tree*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define C24_VERSION 202311L
#if __STDC_VERSION__ < C24_VERSION
    #define nullptr NULL
    typedef enum{false=0,true=1}bool;
#endif // __STDC_VERSION

#define ELem int;
#define MAX_CODE_LENGTH 100

typedef enum{ERROR=0,OK=1}Status;

typedef struct  HuffmanNode{
    int weight; // 权值
    int parent; // 父节点
    int leftChild; // 左孩子
    int rightChild; // 右孩子
} HuffmanNode , *HuffmanTree;

//选择两个权值最小的节点
Status SelectTwoHuffmanNode(HuffmanTree huffmanTree, int n, int* pa, int* pb){
    int i, t, a, b;
    for(i = 0; i < n; i++){
        if(huffmanTree[i].parent < 0) break;
    }
    if(i >= n) return ERROR;
    a = i;
    for(i = a + 1; i < n; i++){
        if(huffmanTree[i].parent < 0) break;
    }
    if(i >= n) return ERROR;
    b = i;
    if(huffmanTree[a].weight > huffmanTree[b].weight){
        t = a;
        a = b;
        b = t;
    }
    i = a > b ? a : b;
    for(i = i + 1; i < n; i++){
        if(huffmanTree[i].parent < 0 && huffmanTree[i].weight < huffmanTree[b].weight){
            b = i;
        }
        if(huffmanTree[a].weight > huffmanTree[b].weight){
            t = a;
            a = b;
            b = t;
        }
    }
    *pa = a;
    *pb = b;
    return OK;
}

//创建一个哈夫曼树
HuffmanTree ConstructHuffmanTree(int weight[],int n){
    Status s;
    int m = 2 * n - 1;
    HuffmanTree huffmanTree = 
        (HuffmanTree)malloc(m * sizeof(HuffmanNode));
    if(huffmanTree == nullptr) return nullptr;
    int i = 0;
    for(i = 0; i < n ;i++){
        huffmanTree[i].weight = weight[i];
        huffmanTree[i].parent = -1;
        huffmanTree[i].leftChild = -1;
        huffmanTree[i].rightChild = -1;
    }
    for(i = n; i < m; i++){
        huffmanTree[i].weight = 0;
        huffmanTree[i].parent = -1;
        huffmanTree[i].leftChild = -1;
        huffmanTree[i].rightChild = -1;
    }
    int a, b;
    for(i = n; i < m; i++){
        s = SelectTwoHuffmanNode(huffmanTree, i, &a, &b);
        if(s == ERROR) return nullptr;
        huffmanTree[i].weight = huffmanTree[a].weight + huffmanTree[b].weight;
        huffmanTree[i].leftChild = a;
        huffmanTree[i].rightChild = b;
        huffmanTree[a].parent = i;
        huffmanTree[b].parent = i;
    }
    return huffmanTree;    
}
void PrintHuffmanTree(HuffmanTree huffmanTree, int n) {
    printf("Index\tWeight\tParent\tLeftChild\tRightChild\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\n", 
               i, 
               huffmanTree[i].weight, 
               huffmanTree[i].parent, 
               huffmanTree[i].leftChild, 
               huffmanTree[i].rightChild);
    }
}

//哈夫曼树的销毁
void DestructHuffmanTree(HuffmanTree huffmanTree){
    free(huffmanTree);
    huffmanTree = nullptr;
}


//打印哈夫曼编码
Status PrintHuffmanCode(HuffmanTree huffmanTree, int numOfLeaves){
    int i, j, p, k;
    char reverseCode[MAX_CODE_LENGTH];
    if(huffmanTree == nullptr || numOfLeaves <= 2){
        printf("Errror\n");
        return ERROR;
    }
    for(i = 0; i < numOfLeaves; i++){
        j = i;/*为第i个叶子编码，从叶子到根，倒着编码*/
        k = 0;/*编码数组的下标回零*/
        while(huffmanTree[j].parent > 0){/*从叶子到根*/
            p = huffmanTree[j].parent;/*父节点*/
            if(huffmanTree[p].leftChild == j){
                reverseCode[k] = '0';/*左分支用0编码*/
            }else{
                reverseCode[k] = '1';/*右分支用1编码*/
            }
            k++;
            j = p;/*上移到父节点*/
        }/*while结束，得到第i个叶子的倒序编码*/
        printf("第%d个叶子节点的哈夫曼编码为：", i);
        k = k - 1;//指向编码的最后一个字符
        while(k >= 0){
            putchar(reverseCode[k]);/*从后向前输出编码*/
            k--;
        }/*while结束，第i个叶子的编码打印结束*/
        printf("\n");
    }/*for结束，全部编码打印结束*/
    return OK;
}/*在实际应用中，一般把编码打印到文件*/

void TestConstructHuffmanTree() {
    // 测试权值数组
    int weights[] = {5, 9, 12, 13, 16, 45};
    int n = sizeof(weights) / sizeof(weights[0]);

    // 构造哈夫曼树
    HuffmanTree huffmanTree = ConstructHuffmanTree(weights, n);
    if (huffmanTree == nullptr) {
        printf("Failed to construct Huffman Tree.\n");
        return;
    }

    // 打印哈夫曼树
    int m = 2 * n - 1; // 哈夫曼树的节点总数
    printf("Huffman Tree constructed successfully:\n");
    PrintHuffmanTree(huffmanTree, m);

    // 释放内存
    free(huffmanTree);
}

int main() {
    TestConstructHuffmanTree();
    return 0;
}