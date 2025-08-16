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
typedef char ELEM;
typedef struct BinaryNode{
    Elem elem;
    ELEM data;
    struct BinaryNode *pLeft;
    struct BinaryNode *pRight;
}BinaryNode,*BinaryTree;