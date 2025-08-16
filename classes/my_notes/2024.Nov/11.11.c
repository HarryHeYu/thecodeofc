/*数据结构（数据的结构原理）
数据元素：能看成一个整体
集合，线性结构（如栈），树状结构，网/图状结构
物理结构：顺序存储结构、链式存储结构（数据的存储结构又称为数据的物理结构）*/

/*面向对象的程序设计：
（C语言是面向过程的语言，但是可以做面向对象的程序设计，就是会有点麻烦）
（C语言没有封装功能，没有局部函数的说法，因此会出现函数重名的情况导致报错->建议用‘类型名’做前缀来解决）
面向对象的语言可以打包
（用类型名作为该类型的操作函数的前缀。例如，Stack的进栈操作命名为StackPush，出栈操作命名 数据结构和面向对象的概念为StackPop。）
    面向过程：着眼于过程和过程中的事件（依据：程序本身就是一种过程）
    面向对象：（程序要解决现实世界的问题，现实世界由对象组成）着眼于对象与对象之间的联系(程序也是对象)
两种方法不矛盾，是相互补充的
一个（新）数据类型/结构（typedef）是一个对象*/

//数据类型是指一类数据所具有的共同性质
//数据的性质不仅仅体现在数据的存储格式上，也体现在数据可以做哪些操作上
//在面向对象编程的语言中，数据类型被上升为了“类”

/*C语言只能用struct封装变量。对象的变量称为成员变量，用来描述对象的属性；对象的函数称为成员函数，
用来描述对象的行为，也就是操作对象的方法。因此，对象的变量又称属性，对象的函数又称方法。*/

/*面向过程的程序设计就是要编写一个个的过程，一个过程就是一个函数；
面向对象的程序设计就是要编写一个个的对象，一个对象就是一个自定义的数据类型再加上支持这种数据类型的一组操作函数。*/


/*在静态区（全局区和栈区）创建对象：
S s;
-Init(&s);  初始化
...    使用对象
-Free(&s);  有时需要，有时不需要
如果在-Init函数中有动态分配内存的操作，
那么在使用完对象之后就需要用-Free函数释放相应的内存；否则不需要。

在动态区（堆区）创建对象：
D* pD;
pD=-Construct();  (Construct)构造函数（由先为他分配内存在初始化两步构成）
if(pD==NULL){
...    如果未成功创建，处理错误
}
...    使用对象
-Destruct(pD);   使用完后，析构（销毁）对象
pD=NULL  避免指针悬空*/
//销毁pD指向的对象并不能销毁pD本身

/*栈：栈底可以找地址*/

#include <stdio.h>
#define STACK_INIT_SIZE 100
#define STACK_INCREMENT 10
typedef enum{ERROR=0,OK=1}Status;
typedef enum{FALSE=0,TRUE=1}Bool;
typedef unsigned int StackElem;

/*StackElem是一个常量(int,float...)(也可以是一个结构体)，需自行定义，但是又不能定义死了，不然会不太好用*/

typedef struct{
    StackElem* pTop;
    StackElem* pBase;//pBase是栈底指针，pTop是栈顶指针.pBase 是一个指针，它指向栈分配的内存的起始位置
    int size;
}Stack;//用动态数组表示栈

/*上文中的Stack只能存取StackElement类型的数据。当我们需要整型栈时，就把StackElement定义为int；当我们需要浮点型栈时，就把它的定义改为ﬂoat。
这似乎没有问题。然而如果在一个程序中既需要整型栈，又需要浮点型栈，那么我们就无法定义StackElement。因此我们需要可以存取任意类型数据的通用栈，即泛型栈。*/

Status StackTraverse(Stack* pStack,Status(*visit)());

Status StackInit(Stack* pStack){
    pStack->pBase=malloc(STACK_INIT_SIZE*sizeof(StackElem));//分配空间。 赋初值？
    //malloc 被调用以分配足够的内存来存储 STACK_INIT_SIZE 个 StackElement 类型的元素。
    //在栈的实现中，pBase 指向的是栈在内存中的起始位置，即栈的底部
    if(pStack->pBase==NULL){
        return ERROR;
    }
    pStack->pTop=pStack->pBase;//初始化栈顶指针 pTop，将其设置为基地址 pBase。这意味着栈是空的，因为栈顶和栈底是相同的。
    pStack->size=STACK_INIT_SIZE;//设置栈的大小为初始大小 STACK_INIT_SIZE
    return OK;
}
/*由于 realloc 尝试在原有内存块后面扩展内存，所以 pBase 和 pTop 之间的距离（即栈中元素的数量）在重新分配内存后保持不变。
只有当原有内存块后面没有足够空间进行扩展时，realloc 可能会移动内存块到新的位置，但即使在这种情况下，pBase 和 pTop 之间的相对位置关系仍然保持不变。*/

void StackFree(Stack* pStack){
    free(pStack->pBase);//实际上只需要释放 pBase 指针所指向的内存块，因为 pBase 指向的是整个栈的起始位置
    //不需要单独释放 pBase 和 pTop 之间的内存，因为它们是连续的，并且都属于同一个通过 malloc 分配的内存块。释放 pBase 指向的内存块会释放整个栈的内存。
    pStack->pBase=NULL;
    pStack->pTop=NULL;//三个分量还在，是指向的数组被free了
    pStack->size=0;
}/*free 函数将释放 pBase 指向的内存块，将其归还给操作系统。这意味着程
序不再拥有这块内存的控制权。
释放内存后，内存中的数据不会被自动清除。也就是说，原始数据可能仍然存在于内存中，直到操作系统重新分配这块内存给其他用途，并且覆盖了原有的数据。*/

Stack* StackConstruct(){
    Stack* pStack=(Stack*)malloc(sizeof(Stack));
    if(pStack==NULL){
        return NULL;
    }//失败了就返回空（NULL）
    StackInit(pStack);
    return pStack;
}

void StackDestruct(Stack* pStack){
    free(pStack->pBase);
    free(pStack);
}

Bool StackIsEmpty(Stack* pStack){
    if(pStack->pTop==pStack->pBase){//如果栈顶指针等于栈底指针则意味着栈内未存储数据也即为空
        return TRUE;
    }
    else{
        return FALSE;
    }
}

int StackLength(Stack* pStack){
    return pStack->pTop - pStack->pBase;
}

void StackClear(Stack* pStack){
    pStack->pTop=pStack->pBase;
}//要清空栈内的数据元素，其实并不需要真的把那些数据删除，只要把栈顶指针移到栈底即可。
/*要真正地清空有两种方法：一.重置栈顶指针，仅重置pTop使其指向栈底，这样栈看起来是是空的，即是内存中仍然存储着旧数据
（请注意，这种方法并不释放内存，也不清除内存中的数据。它只是改变了栈顶指针的位置，使得栈的逻辑状态看起来是空的。）
二.清除占中所有的数据，即遍历栈并逐个元素地清楚数据*/

Status StackPush(Stack* pStack,StackElem* pElem){
    StackElem* pTemp;
    if(pStack->pTop - pStack->pBase >= pStack->size){//判断栈是不是满了，用长度来判断
        newSize=pStack->size+STACK_INCREMENT;
        pTemp=(StackElem*)realloc (pStack->pBase,newSize);//重新分配内存要用realloc
        if(pTemp==NULL){
            return ERROR;
        }
        pStack->pBase=pTemp;
        pStack->pTop=pStack->pBase+pStack->size;
        pStack->size+=STACK_INCREMENT;
    }
    *(pStack->pTop)=*pElem;
    pStack->pTop++;
    return OK;
}

Status StackPop(Stack* pStack,StackElem* pElem){
    if(StackIsEmpty(pStack)){
        return ERROR;
    }
    pStack->pTop--;
    *pElem=*(pStack->pTop);
    return OK;
}

Status StackGetTop(Stack* pStack,StackElem* pElem){
    if(StackIsEmpty(pStack)){
        return ERROR;
    }
    *pElem=*(pStack->pTop-1);
    return OK;
}//栈顶指针始终指向栈顶的空单元，栈顶元素所在的位置实际是pTop−1。

Status StackTraverse(Stack* pStack,Status(*visit)()){
    int i,n;
    n=StackLength(pStack);
    for(i=0;i<n;i++){
        if(visit(&pStack->pBase[i])==ERROR){////不加&的话就只能读不能写
            return ERROR;
        }
    }
    return OK;
}

Status StatusInit(Stack* pStack);//初始化栈
void StackFree(Stack* pStack);//释放栈   释放栈是指销毁栈的数据结构，释放栈占用的内存资源。
Stack* StackConstruct();//有构造函数就必定有析构函数
void StackDestruct(Stack* pStack);//传参传指针
Bool StackIsEmpty(Stack* pStack);//判断栈是否为空
int StackLength(Stack* pStack);//栈的长度
void StackClear(Stack* pStack);//清空栈  清空栈是指将栈中的所有元素移除，但保留栈的数据结构本身(即内存还在)。
Status StackPush(Stack* pStack,StackElem* pElem);//入栈。Stack* pStack:告诉是入哪个栈，StackElem* e:告诉入栈的元素(元素可以不用指针)
Status StackPop(Stack* pStack,StackElem* pElem);//出栈。栈必须是指针
Status StackGetTop(Stack* pStack,StackElem* pElem);//取栈顶元素
Status StackTraverse(Stack* pStack,
                                   Status(*visit)());//遍历栈(对每一个对象都进行操作),第二个指针是一个函数指针，指向一个函数，用visit函数来对栈中元素进行操作
//从typedef struct开始到这里是一个对象

//  #define StackLength(pStack) \
//  ((pStack)->pTop-(pStack)->pBase)
// 这里用宏实现-Length。上面的两行本是一个逻辑行，因为一行写不下，
// 所以分成两行书写，第1行末尾的反斜线是续行符。




