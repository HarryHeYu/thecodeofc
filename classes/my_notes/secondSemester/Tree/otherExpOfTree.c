#include <stdio.h>
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
typedef char ELEM;
//双亲表示法
typedef struct {
    ELEM data;
    Elem elem;
    int parent;
}TreeNode1;
typedef struct {
    TreeNode1* nodeArr[MAX_ELEM];
    int numOfNodes;
}Tree1;

//孩子表示法
typedef struct ChildNode{
    int childIndex;
    struct ChildNode* pNext;
}ChildNode;
typedef struct {
    ELEM data;
    Elem elem;
    ChildNode* pFirstChild;
}TreeNode2;
typedef struct {
    TreeNode2 nodeArr[MAX_ELEM];
    int root;
    int numOfNodes;
}Tree2;

//孩子-兄弟2表示法
/*左手牵着大孩子，右手牵着兄弟*/
typedef struct TreeNode3{
    ELEM data;
    Elem elem;
    struct TreeNode3* pFirstChild;
    struct TreeNode3* pNextSibling;
}TreeNode3, *Tree3;