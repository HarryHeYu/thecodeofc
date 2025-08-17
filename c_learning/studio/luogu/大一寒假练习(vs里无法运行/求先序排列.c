#include<stdio.h>
#include<string.h>
//根据中序和后序遍历结果来求先序遍历
//inStart表示当前处理的中序遍历序列的起始索引，inEnd是中序遍历序列的结束索引;
//postEnd表示当前处理的后序遍历序列的结束索引
void preOder(char *inoder, char* postoder, int inStart, int inEnd, int postEnd){
    if(inStart > inEnd)return;//表示子树为空
    //后序遍历的最后一个字符是根节点
    char root = postoder[postEnd];
    printf("%c",root);//后序遍历的最后一个字符是根节点，输出该根节点。
    //在中序遍历中找到根节点的位置
    int rootIndex;
    for(rootIndex = inStart; rootIndex <= inEnd; rootIndex++){
        if(inoder[rootIndex] == root)break;
    }
    //递归处理左子树
    preOder(inoder,postoder,inStart,rootIndex - 1,postEnd - (inEnd - rootIndex) - 1);
    //递归处理右子树
    preOder(inoder,postoder,rootIndex + 1,inEnd,postEnd - 1);
}
int main(){
    char inoder[10], postoder[10];
    //读取中序和后序遍历结果
    scanf("%s",inoder);
    scanf("%s",postoder);
    int len = strlen(inoder);
    //调用函数求先序排列
    preOder(inoder,postoder,0,len - 1,len -1);
    return 0;    
}
/*# P1030 [NOIP 2001 普及组] 求先序排列
## 题目描述
给出一棵二叉树的中序与后序排列。求出它的先序排列。（约定树结点用不同的大写字母表示，且二叉树的节点个数 $ \le 8$）。
## 输入格式
共两行，均为大写字母组成的字符串，表示一棵二叉树的中序与后序排列。
## 输出格式
共一行一个字符串，表示一棵二叉树的先序。
## 输入输出样例 #1
### 输入 #1
```
BADC
BDCA
```
### 输出 #1
```
ABCD
```*/