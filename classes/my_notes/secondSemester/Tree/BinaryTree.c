/* Binary Tree */
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define C24_VERSION 202311L
#if __STDC_VERSION__ < C24_VERSION
    #define nullptr NULL
    typedef enum{false=0,true=1}bool;
#endif // __STDC_VERSION
#define MAX_HEIGHT 6
#define MAX_ELEM 99
#define EMPTY -1
typedef enum{ERROR=0,OK=1}Status;
typedef int Elem;
typedef struct BinaryNode{
    Elem elem;
    struct BinaryNode *pLeft;
    struct BinaryNode *pRight;
}BinaryNode,*BinaryTree;
BinaryNode* makeNode(Elem elem);  // private
void freeNode(BinaryNode *pNode); // private
BinaryTree* Construct();
void Destruct(BinaryTree *pTree);
bool IsEmpty(BinaryTree *pTree);
Status Insert(BinaryTree *pTree,Elem elem);
void TraversePreorder(BinaryTree pRoot,void (*visit)(Elem*));
void TraverseInorder(BinaryTree pRoot,void (*visit)(Elem*));
void TraversePostorder(BinaryTree pRoot,void (*visit)(Elem*));
void TraverseInorder_Nonrecursive(BinaryTree pRoot,
                                  void (*visit)(Elem*));
int Height(BinaryTree pRoot);
// Returns the height of a binary tree.
// pRoot points to the root of the tree.

// The following functions are used for visualization.
void PrintInt(int* pInt);
// Prints *pInt on the screen.
void Display(BinaryTree pRoot);
// Displays a binary tree on the screen.
void LayOut(BinaryTree pRoot,int* a,int m,int left,int right);
/* This is a private operation of the Dislay function.
 * It lays out a binary tree for on-screen display.
 * pRoot points to the root of the tree.
 * a[][m] is an output matrix that records the position of
 * the binary tree.
 * left is the leftmost position.
 * right is the rightmost position.
 */

BinaryNode* makeNode(Elem elem){
    BinaryNode *pNode;
    pNode=(BinaryNode*)malloc(sizeof(BinaryNode));
    if(pNode==nullptr) return nullptr;
    pNode->elem=elem;
    pNode->pLeft=nullptr;
    pNode->pRight=nullptr;
    return pNode;
}
void freeNode(BinaryNode *pNode){
    free(pNode);
    return;
}
BinaryTree* Construct(){
    BinaryTree *pTree;
    pTree=(BinaryTree*)malloc(sizeof(BinaryTree));
    if(pTree!=nullptr){
        return nullptr;
    }
    return pTree;
}
void Clear(BinaryTree pRoot){
    if(pRoot==nullptr) return;
    Clear(pRoot->pLeft);
    Clear(pRoot->pRight);
    freeNode(pRoot);
    return;
}
//使用后序遍历递归释放树的节点
void Destruct(BinaryTree *pTree){
    if(pTree==nullptr) return;
    Clear(*pTree);
    free(pTree);
    return;
}
bool IsEmpty(BinaryTree *pTree){
    return *pTree==nullptr?true:false;
}
Status Insert(BinaryTree *pTree,Elem elem){
    BinaryNode *pNode,*pRoot;
    if(*pTree==nullptr){
        pNode=makeNode(elem);
        if(pNode==nullptr) return ERROR;
        *pTree=pNode;
        return OK;
    }
    pRoot=*pTree;
    if(elem==pRoot->elem) return ERROR;
    if(elem<pRoot->elem)  return Insert(&(pRoot->pLeft),elem);
    return Insert(&(pRoot->pRight),elem);
}
void TraversePreorder(BinaryTree pRoot,void (*visit)(Elem*)){
    if(pRoot==nullptr) return;
    visit(&(pRoot->elem));
    TraversePreorder(pRoot->pLeft,visit);
    TraversePreorder(pRoot->pRight,visit);
}
void TraverseInorder(BinaryTree pRoot,void (*visit)(Elem*)){
    // ASSIGNMENT 1 TO DO
    if(pRoot==nullptr) return;
    TraverseInorder(pRoot->pLeft,visit);
    visit(&(pRoot->elem));
    TraverseInorder(pRoot->pRight,visit);
    // The following line is for testing.
    printf("This is assignment 1.");
}
void TraversePostorder(BinaryTree pRoot,void (*visit)(Elem*)){
    // ASSIGNMENT 2 TO DO
    if(pRoot==nullptr) return;
    TraversePostorder(pRoot->pLeft,visit);
    TraversePostorder(pRoot->pRight,visit);
    visit(&(pRoot->elem));
    // The following line is for testing.
    printf("This is assignment 2.");
}
void TraverseInorder_Nonrecursive(BinaryTree pRoot,
                                  void (*visit)(Elem*)){
    // ASSIGNMENT 3 TO DO
    BinaryNode *pNode;
    BinaryNode *pStack[MAX_HEIGHT];
    int top=-1;
    if(pRoot==nullptr) return;
    pNode=pRoot;
    while(pNode!=nullptr || top!=-1){
        while(pNode!=nullptr){
            pStack[++top]=pNode;
            pNode=pNode->pLeft;
        }
        if(top!=-1){
            pNode=pStack[top--];
            visit(&(pNode->elem));
            pNode=pNode->pRight;
        }
    }
    // The following line is for testing.6
    printf("This is assignment 3.\n");
    printf("Basic idea is to use stack. ");
    printf("Please refer to the textbook for details.\n");
}
int Height(BinaryTree pRoot){
    int h1,h2;
    if(pRoot==nullptr) return 0;
    h1=1+Height(pRoot->pLeft);
    h2=1+Height(pRoot->pRight);
    return h1>h2?h1:h2;
}
void Display(BinaryTree pRoot){
    int h,m,left,right,i,j;
    h=Height(pRoot);
    if(h>MAX_HEIGHT){
        printf("The tree is too large to display.\n");
        return;
    }
    m=(1<<h)-1; // m=2^h-1;
    int a[h][m];
    for(i=0;i<h;i++){
        for(j=0;j<m;j++){
            a[i][j]=EMPTY; //初始化矩阵
        }
    }
    left=0;
    right=m-1;
    LayOut(pRoot,(int*)a,m,left,right); //布局树到数组
    printf("\n");
    for(i=0;i<h;i++){
        for(j=0;j<m;j++){
            if(a[i][j]==EMPTY) printf("  ");
            else printf("%2d",a[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
void LayOut(BinaryTree pRoot,int* a,int m,int left,int right){
    static int level=-1;
    if(pRoot==nullptr) return;
    level++; //level 表示当前的层级（二维数组的行号）。
    a[level*m+(left+right)/2]=pRoot->elem; //(left + right) / 2 表示当前节点的水平位置。
    LayOut(pRoot->pLeft,a,m,left,(left+right)/2-1);
    LayOut(pRoot->pRight,a,m,(left+right)/2+1,right);
    level--;
    return;
}
void PrintInt(int* pInt){
    printf("%d ",*pInt);
}
int main(){
    BinaryTree *pTree;
    pTree=Construct();
    if(pTree==nullptr) return 1;
    Insert(pTree,50);
    Insert(pTree,30);
    Insert(pTree,70);
    /* int i,n;
    n=16;
    srand((unsigned)time(nullptr));
    for(i=0;i<n;i++){
        Insert(pTree,rand()%(MAX_ELEM+1));
    }*/
    Insert(pTree,70);
    Insert(pTree,56);
    Insert(pTree,13);
    Insert(pTree,18);
    Insert(pTree,60);
    Insert(pTree,10);
    Insert(pTree,36);
    Insert(pTree,32);
    Insert(pTree,40);
    Insert(pTree,80);
    Insert(pTree,77);
    Insert(pTree,55);
    printf("This is the binary tree:\n");
    Display(*pTree);
    printf("Preorder traversal:\n");
    TraversePreorder(*pTree,PrintInt);
    printf("\n\n");
    printf("Inorder traversal:\n");
    TraverseInorder(*pTree,PrintInt);
    printf("\n\n");
    printf("Postorder traversal:\n");
    TraversePostorder(*pTree,PrintInt);
    printf("\n\n");
    printf("Non-recursive inorder traversal:\n");
    TraverseInorder_Nonrecursive(*pTree,PrintInt);
    Destruct(pTree);
    return 0;
}
