/*
    根据扩展序列创建二叉树
*/
#include <stdio.h>
#include "Tree.h"

#define EMPTY_NODE '#' // Define EMPTY_NODE with a placeholder value
typedef enum{
    ERROR = 0,
    OK = 1
} Status;

Status CreateBinaryTree(BinaryTree* pBinaryTree,
                        char* preorderSequence){
    static int i= 0;
    char rootData;//设每个节点保存的是字符
    BinaryNode* pRoot;
    rootData = preorderSequence[i];
    i++;
    if(rootData == '\0' || rootData == EMPTY_NODE){
        *pBinaryTree = nullptr;
        return OK;
    }
    pRoot = (BinaryNode*)malloc(sizeof(BinaryNode));
    if(pRoot == nullptr)return ERROR;
    pRoot->data = rootData;
    *pBinaryTree = pRoot;
    if(CreateBinaryTree(&(pRoot->pLeft), preorderSequence) == ERROR){
        return ERROR;
    }
    if(CreateBinaryTree(&(pRoot->pRight), preorderSequence) == ERROR){
        return ERROR;
    }
    return OK;
}
//根据序列组合创建二叉树
/*n：当前子树的节点数量。
前序数组的作用：
    确定根节点值
    划分左右子树的前序部分：
        左子树的前序序列：紧跟根节点的若干元素（长度由中序数组确定）。
        右子树的前序序列：剩余元素。(例如，若左子树包含 i 个节点，则前序数组中 [1..i] 属于左子树，[i+1..n-1] 属于右子树。)
    i 的含义：中序序列中，前 i 个元素属于左子树，剩余 n-i-1 个元素属于右子树。
中序数组的作用：
    定位根节点位置
中序数组的作用：
    定位根节点位置
    确定左右子树的节点数量
        左子树的节点数直接由 i 给出，右子树的节点数为 n-i-1。这决定了前序数组中左右子树的划分位置。*/
Status CreateBinaryTreeByMixed(BinaryTree* pBinaryTree, char preorder[],char inorder[],int n){
    if(n <= 0){
        *pBinaryTree = nullptr;
        return OK;
    }
    BinaryNode* pRoot = 
        (BinaryNode*)malloc(sizeof(BinaryNode));
    if(pRoot == nullptr) return ERROR;
    pRoot->data = preorder[0];
    *pBinaryTree = pRoot;
    int i = Locate(inorder, preorder[0], n);//返回根的中序位置
//i 的含义：中序序列中，前 i 个元素属于左子树，剩余 n-i-1 个元素属于右子树。
    if (i < 0) return ERROR;
    if(CreateBinaryTreeByMixed(&(pRoot->pLeft), preorder+1, inorder, i) == ERROR){
        return ERROR;
    }
    if(CreateBinaryTreeByMixed(&(pRoot->pRight), preorder+i+1,inorder+i+1,n-i-1) == ERROR){
        return ERROR;
    }
    return OK;
}
// 在中序序列中查找目标值的位置
int Locate(char inorder[], char target, int n) {
    for (int i = 0; i < n; i++) {
        if (inorder[i] == target) {
            return i; // 找到目标值，返回索引
        }
    }
    return -1; // 如果未找到目标值，返回 -1
}