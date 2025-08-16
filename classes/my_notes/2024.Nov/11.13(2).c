//链表
/*链表中每个节点有两个分量，一个存数据一个指向下一个节点的指针
一般都用void做通用指针*/
#include <stdio.h>
#include <stdlib.h>//(malloc,realloc,free在这里面)
#define STACK_INIT_SIZE 100
#define STACK_INCREMENT 10
typedef int StackElem;//不要定，不然就不好用了，但是此处又必须定义，不然报错
typedef enum{ERROR=0,OK=1}Status;
typedef enum{FALSE=0,TRUE=1}Bool;
typedef struct{
    StackElem* pTop;
    StackElem* pBase;//pBase是栈底指针，pTop是栈顶指针.pBase 是一个指针，它指向栈分配的内存的起始位置
    int size;
}Stack;

typedef struct ListNode{
    struct ListNode *pFirstNode;/*这里的struct关键字是必须的，因为它告诉编译器ListNode是一个结构体类型，而不是一个已经定义的类型或者变量名。
        如果没有struct关键字，编译器会将ListNode视为一个已经定义的类型或变量名，这会导致编译错误，因为在这个结构体定义的上下文中，ListNode还没有被完全定义。
        其实pFirstNode就是指向下一个节点的指针*/
    void* pData;
}ListNode;//，第一个ListNode是结构名，最后这一个是类型名//链表节点
/*这是一个递归的定义，在定义ListNode时，又用到了ListNode。在一个节点中有两个分量：pNextNode是指向下一个节点的指针；pData是指向任意数据的通用指针。*/

typedef struct{
    ListNode *pFirstNode;
    int length;
}List;//表
//把表定义出来了，先定义节点，再利用节点定义表


//私有函数前面加了一个static，仅供链表使用
static ListNode* ListMakeNode(void* pData);//创建节点
static void ListFreeNodes(List* pNode);//释放节点
//上面两个函数一般用不到，只有开发链表的人才会用到（叫做对象的私有函数：外部用不着）

List* ListConsstruct();//初始化函数和构造函数写一个就可以了（前提是在动态内存上,这里就不写了）
void ListDestruct(List* pList);
int ListLength(List* pList);
void ListClear(List* pList);
Status ListAppendToTail(List* Plist,void* pData);//加到尾部
Status ListInsertAtFront(List* pList,void* pData);//加到头部
Status ListTraverse(List* pList, Status (*visit)());

List* ListConstruct(){
    List *pList;
    pList=(List*)malloc(sizeof(List));
    return pList;
}

void ListDestruct(List* pList){
    //在freelist前应该先把所有的节点都释放掉
    // free(pList);//Wrong,只free掉了头，后面的数据就找不到了但是数据还在，会造成严重的内存泄漏
}