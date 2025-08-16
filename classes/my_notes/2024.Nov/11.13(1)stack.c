//接上回并修改并继续
#include <stdio.h>
#include <stdlib.h>//(malloc,realloc,free在这里面)
#define STACK_INIT_SIZE 100
#define STACK_INCREMENT 10
typedef int StackElem;//不要定，不然就不好用了，但是此处又必须定义，不然报错
//应该说是定了的话就是特定型的栈了，就不是泛型了
typedef enum{ERROR=0,OK=1}Status;
typedef enum{FALSE=0,TRUE=1}Bool;
typedef struct{
    StackElem* pTop;
    StackElem* pBase;//pBase是栈底指针，pTop是栈顶指针.pBase 是一个指针，它指向栈分配的内存的起始位置
    int size;
}Stack;


Status StackTraverse(Stack* pStack,Status(*visit)()){
    int i,n;
    n=StackLength(pStack);
    for(i=0; i<n; i++){
        if(visit(pStack->pBase+i) != OK) {
            return ERROR;
        }
    }
    return OK;
}

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
void StackFree(Stack* pStack){
    free(pStack->pBase);//实际上只需要释放 pBase 指针所指向的内存块，因为 pBase 指向的是整个栈的起始位置
    //不需要单独释放 pBase 和 pTop 之间的内存，因为它们是连续的，并且都属于同一个通过 malloc 分配的内存块。释放 pBase 指向的内存块会释放整个栈的内存。
    pStack->pBase=NULL;
    pStack->pTop=NULL;//三个分量还在，是指向的数组被free了
    pStack->size=0;
}
Stack* StackConstruct(){
    Stack* pStack=(Stack*)malloc(sizeof(Stack));//这个Stack就是构建的类型（如int，float等）（Stack*的那个Stack）
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
}
Status StackPush(Stack* pStack,StackElem* pElem){
    StackElem* pTemp;
    int newSize;
    if(pStack->pTop - pStack->pBase >= pStack->size){//判断栈是不是满了，用长度来判断
        newSize=pStack->size+STACK_INCREMENT;
        pTemp=(StackElem*)realloc (pStack->pBase,newSize*sizeof(StackElem));//重新分配内存要用realloc,newSize要乘以sizeof(StackElem)以确保分配的内存大小是正确的
        if(pTemp==NULL){
            printf("Realloc failed.\n");
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
//该函数只是将栈顶元素的值复制到 pElem 指向的内存位置，而不会删除栈顶元素。

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
                                   Status(*visit)());

/*void*不能用作函数指针。由于函数和数据可能在不同的内存区，C标准委员会从未考
虑把函数指针和数据指针统一起来。*/


Status PrintElem(StackElem* pElem){
    printf("%d\t",*pElem);
    return OK;
}                            

Status Add100(StackElem* pElem){
    (*pElem)+=100;
    return OK;
}

int main(){
    Stack staticStack,*pDynamicStack;//定义了一个静态栈的对象与一个动态对象的指针。接下来需要构造一个动态栈
    int i,n=120;
    if(StackInit(&staticStack)==ERROR){
        printf("Obeject creation failed.\n");
        return 1;
    };//对他做初始化
    pDynamicStack=StackConstruct();//构造一个动态栈,不一定成功
    if(pDynamicStack==NULL){
        printf("Stack Creation failed.\n");
        return 1;//返回1非正常运行
    }
    //可以使用对象了。静态对象本身使用的是静态内存，但是其中的pBase使用的是动态内存，因此两个对象使用后都要销毁
    for(i=0;i<n;i++){
        StackPush(&staticStack,&i);//第一个进的是静态栈，进入的元素是i。
        StackPush(pDynamicStack,&i);//第二个进的是动态栈，进入的元素是i。
    }
    int len1=StackLength(&staticStack);
    int len2=StackLength(pDynamicStack);
    puts("Static Stack:\n");
    /*puts 函数的主要功能是将传入的字符串输出到标准输出，并在字符串末尾添加一个换行符。这个函数常用于在屏幕上显示字符串，特别是在需要在输出后自动换行的情况下。
    自动换行：与 printf 函数不同，puts 函数会自动在输出的字符串后添加换行符，因此不需要手动添加 \n。
    字符串长度：puts 函数期望传入的是一个以空字符结尾的字符串，如果传入的字符串不以空字符结尾，可能会导致未定义行为。
    错误处理：如果 puts 函数成功执行，它会返回一个非负整数；如果发生错误（如输出流不可写），它会返回 EOF。*/
    printf("Length of static stack is %d\n",len1);
    StackTraverse(&staticStack,PrintElem);
    puts("\nDynamic Stack:\n");
    printf("Length of dynamic stack is %d\n",len2);
    StackTraverse(pDynamicStack,Add100);
    StackTraverse(pDynamicStack,PrintElem);
    while(StackIsEmpty(&staticStack)==FALSE){//如果非空
        StackPop(&staticStack,&i);
        printf("Pop(%d)\n",i);
    }

    StackFree(&staticStack);
    StackDestruct(pDynamicStack);//这个不能取地址，因为pDynamicStack是一个指针
    //静态对象本身使用的是静态内存，但是其中的pBase使用的是动态内存，因此两个对象使用后都要销毁
    return 0;
}



Status StackPop(Stack* pStack,void* pElem){
char *pc;
if(pStack->pTop==pStack->pBase){
return ERROR;
}
else{
pc=(char*)(pStack->pTop);
pStack->pTop=pc-pStack->size;//这行代码更新栈顶指针pTop。由于pc指向栈顶元素的开始，减去elementSize（栈中每个元素的大小）将pTop指向栈中下一个元素的开始。
memcpy(pElem,pStack->pTop,pStack->size);//这行代码使用memcpy函数将栈顶元素复制到pElem指向的内存位置。复制的字节数由pStack->elementSize决定。
return OK;
}
}//另外一种出栈方法
