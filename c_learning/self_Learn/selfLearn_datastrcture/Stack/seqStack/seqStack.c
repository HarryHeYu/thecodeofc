/***************************
 *  <name:顺序栈
 *  <author: He YU Hao
 *  <date:2025.3.21
*****************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 50 // 初始栈容量
#define INCREMENT_SIZE 5 //扩容时增加的容量

typedef int DataType;
typedef enum {ERROR = 0, OK = 1}Status;
typedef enum { errno_NONE = 0, ERR_OVERFLOW = 1, ERR_UNDERFLOW = 2}ErrorCode;

typedef struct {
    DataType* data;
    int top;
    int capacity;
}SeqStack;

//初始化
Status initStack(SeqStack* s){
    s->data = (DataType*)malloc(INITIAL_SIZE * sizeof(DataType));
    if(!s->data) return ERROR;
    s->top = -1;
    s->capacity = INITIAL_SIZE;
    return OK; 
}

//判断是否为空
int isStackEmpty(SeqStack* s){
    return s->top == -1;
}

//判断栈是否已满
int isStackFull(SeqStack* s){
    return s->top == s->capacity - 1;
}

//扩容栈
Status expandStack(SeqStack* s){
    DataType* newData = (DataType*)realloc(s->data, (s->capacity + INCREMENT_SIZE) * sizeof(DataType));
    if(!newData) return ERROR; // 检查 realloc 是否成功
    s->data = newData;
    s->capacity += INCREMENT_SIZE;
    return OK;
}

//入栈操作
Status pushStack(SeqStack* s, DataType item){
    if(isStackFull(s)){
        if(expandStack(s) == ERROR)return ERROR;
    }
    s->data[++s->top] = item;
    return OK;
}

//出栈操作
Status popStack(SeqStack* s, DataType* item){
    if(isStackEmpty(s))return ERROR;
    *item = s->data[s->top--];
    return OK;
}

//查看栈顶元素
Status peekStack(SeqStack* s, DataType* item){
    if(isStackEmpty(s))return ERROR;
    *item = s->data[s->top];
    return OK;
}

//清空栈
Status clearStack(SeqStack* s){
    s->top = -1;
    return OK;
}
/*或者可以清空内部： 清空栈 
Status clearStack(SeqStack* s) {
    while (s->top != -1) {
        s->data[s->top--] = 0;
    }
    return OK;
}*/

//获取栈的长度
int stackLength(SeqStack* s){
    return s->top + 1;
}

//释放栈
Status stackFree(SeqStack* s){
     free(s->data);
     free(s);
     return OK;
}

//遍历栈
Status stackTraverse(SeqStack* s, void(*visit)(DataType)){
    if(isStackEmpty(s))return  ERROR;
    for(int i = 0; i <= s->top; i++)visit(s->data[i]);
    return OK;
}

//创建一个栈
SeqStack* stackConstruct(){
    SeqStack* s = (SeqStack*)malloc(sizeof(SeqStack));
    if(s == NULL){
        printf("Memory allocation failed.\n");
        exit(1);
    }
    if(initStack(s) == ERROR){
        printf("Stack initialization failed.\n");
        exit(1);
    }
    return s;
}
/*
//销毁栈
Status stackDestruct(SeqStack* s){
    clearStack(s);
    free(s->data);
    free(s);
    return OK;
}*/
//该代码在功效上和StackFree没有区别只是多了一个clearStack，在实际操作中，两个函数二选一实现即可

//打印元素
void printElem(DataType elem){
    printf("%d",elem);
}

//排序栈中的元素
Status sortStack(SeqStack* s){
    for(int i = 0; i < stackLength(s); i++){
        for(int j = i + 1; j < stackLength(s); j++){
            if(s->data[i] > s->data[j]){
                DataType temp = s->data[i];
                s->data[i] = s->data[j];
                s->data[j] = temp;
            }
        }
    }
    return OK;
}

//检查括号匹配
Status checkBrackets(SeqStack* s, const char* expr){
    for(int i = 0; expr[i] != '\0'; i++){
        if(expr[i] == '(' || expr[i] == '{' || expr[i] == '['){
            if(pushStack(s, expr[i]) == ERROR)return ERROR;
        }else if(expr[i] == ')' || expr[i] == '}' || expr[i] == ']'){
            DataType top;
            if(isStackEmpty(s) || popStack(s, &top) == ERROR)return ERROR;
            if((expr[i] == ')' && top!= '(') ||
                (expr[i] == '}' && top != '{') ||
                (expr[i] == ']' && top != '['))return ERROR;
        }
    }
    return isStackEmpty(s) ? OK : ERROR;
}

//求后缀表达式的值
Status evaluatePostfix(SeqStack* s, char exp[], int* result){
    for(int i = 0; exp[i] != '\0'; i++){
        if('0' <= exp[i] && exp[i] <= '9'){
            if(pushStack(s, exp[i] - '0') == ERROR)return ERROR;
        }else{
            DataType b, a;
            if(popStack(s, &b) == ERROR || popStack(s, &a) == ERROR)return ERROR;
            DataType c;
            switch(exp[i]){
                case'+':c = a + b;break;
                case'-':c = a - b;break;
                case'*':c = a * b;break;
                case'/':
                    if(b == 0)return ERROR;
                    c = a / b;
                    break;
                default: return ERROR;
            }
            if(pushStack(s, c) == ERROR)return ERROR;
        }
    }
    return popStack(s, result);
}

//测试函数
void testSeqStack(){
    SeqStack* stack = stackConstruct();
    for(int i = 0; i < 50; i++)pushStack(stack, i);
    printf("Stack elements:");
    stackTraverse(stack, printElem);
    printf("\nTop element: ");
    DataType top;
    if (peekStack(stack, &top) == OK) {
        printf("%d\n", top);
    }
    sortStack(stack);
    printf("Sorted stack elements: ");
    stackTraverse(stack, printElem);
    printf("\n");

    const char* expr = "{[()]}";
    printf("Bracket matching for \"%s\": %s\n", expr, checkBrackets(stack, expr) == OK ? "matched" : "not matched");

    char postfixExp[] = "512+4*+3-";
    int result;
    if (evaluatePostfix(stack, postfixExp, &result) == OK) {
        printf("Postfix expression evaluation for \"%s\": %d\n", postfixExp, result);
    }

    DataType x;
    while (!isStackEmpty(stack)) {
        if (popStack(stack, &x) == OK) {
            printf("Popped element: %d\n", x);
        }
    }
    stackFree(stack);
}

int main() {
    testSeqStack();
    return 0;
}
