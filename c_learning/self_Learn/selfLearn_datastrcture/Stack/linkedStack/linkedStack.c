/***************************
 *  <name:链栈
 *  <author: He YU Hao
 *  <date:2025.3.21
*****************************/
#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>

typedef int DataType ;
typedef enum { ERROR = 0, OK = 1 } Status;

//链栈节点定义
typedef struct LStackNode{
    DataType data;
    struct LStackNode* next;
}LStackNode;

//链栈结构定义
typedef struct {
    LStackNode* top;
    int count;
}LinkedStack;

//初始化链栈
Status initStack(LinkedStack* s){
    s->top = NULL;
    s->count = 0;
    return OK;
}

//判断栈是否为空
bool isStackEmpty(LinkedStack *s){
    return s->top == NULL;
}

//入栈操作
Status pushStack(LinkedStack* s, DataType item){
    LStackNode* node = (LStackNode*)malloc(sizeof(LStackNode));
    if(!node)return ERROR;
    node->data = item;
    node->next = s->top;
    s->top = node;
    s->count++;
    return OK;
}

//出栈操作
Status popStack(LinkedStack* s, DataType* item){
    if(isStackEmpty(s))return ERROR;
    LStackNode* node = s->top;
    s->top = node->next;
    *item = node->data;
    free(node);
    s->count--;
    return OK;
}

//查看栈顶元素
Status peekStack(LinkedStack* s, DataType* item){
    if(isStackEmpty(s))return ERROR;
    *item = s->top->data;
    return OK;
}

//清空栈
Status clearStack(LinkedStack* s){
    LStackNode* node;
    while(s->top != NULL){
        node = s->top;
        s->top = node->next;
        free(node);
    }
    s->count = 0;
    return OK;
}

//获取栈的长度
int stackLength(LinkedStack* s){
    return s->count;
}

// //释放栈
// Status stackFree(LinkedStack* s){
//     if(!isStackEmpty(s))clearStack(s);
//     free(s);
//     return OK;
// }

//遍历栈
Status stackTraverse(LinkedStack* s, void(*visit)(DataType)){
    // if(s == NULL || isStackEmpty(s))return ERROR;
    if (s == NULL) return ERROR; // 检查栈是否为 NULL
    LStackNode* node = s->top;
    while(node != NULL){
        visit(node->data);
        node = node->next;
    }
    return OK;
}

//创建一个栈
LinkedStack* stackConstruct(){
    LinkedStack* s = (LinkedStack*)malloc(sizeof(LinkedStack));
    if(s == NULL){
        printf("Memory allocation failed.\n");
        exit(1);
    }
    initStack(s);
    return s;
}

//销毁栈
Status stackDestruct(LinkedStack* s) {
    if (s == NULL) return ERROR;
    clearStack(s); // 清空栈
    free(s);       // 释放栈结构
    return OK;
}

//打印元素
void printElem(DataType elem){
    printf("%d",elem);
}

//排序栈中的元素
Status sortStack(LinkedStack* s){
    if(stackLength(s) < 2)return OK;
    for(LStackNode* i = s->top; i != NULL; i = i->next){
        for(LStackNode* j = i->next; j != NULL; j = j->next){
            if(i->data > j->data){
                DataType temp = i->data;
                i->data = j->data;
                j->data = temp;
            }
        }
    }
    return OK;
}

//检查括号匹配
Status checkBrackets(LinkedStack* s, const char* expr){
    for(int i = 0; expr[i] != '\0'; i++){
        if(expr[i] == '(' || expr[i] == '[' || expr[i] == '{'){
            if(pushStack(s, expr[i]) == ERROR)return ERROR;
        }else if(expr[i] == ')' || expr[i] == ']' || expr[i] == '}'){
            DataType top;
            if(isStackEmpty(s) || popStack(s, &top) == ERROR)return ERROR;
            if( (expr[i] == ')' && top != '(') ||
                (expr[i] == ']' && top != '[') ||
                (expr[i] == '}' &&  top != '{'))return ERROR;
        }
    }
    return isStackEmpty(s) ? OK : ERROR;
/*最后再检查一次 isStackEmpty 是为了确保所有的左括号都找到了匹配的右括号。
即使在遍历表达式的过程中，所有的右括号都匹配了栈顶的左括号，但如果栈中还有剩余的左括号，
说明有未闭合的括号，这种情况下也应该返回 ERROR。*/
}

//求后缀表达式的值
Status evaluatePostfix(LinkedStack* s, char exp[], int* result){
    for(int i = 0; exp[i] != '\0'; i++){
        if('0' <= exp[i] && exp[i] <= '9'){
            if(pushStack(s, exp[i] - '0') == ERROR)return ERROR;
        }else{
            DataType b, a;
            if(popStack(s, &b) == ERROR || popStack(s, &a) == ERROR)return ERROR;
            DataType c;
            switch(exp[i]){
                case '+': c = a + b;break;
                case '-': c = a - b;break;
                case '*': c = a * b;break;
                case '/': 
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
void testLinkedStack(){
    LinkedStack* stack = stackConstruct();
    for(int i = 0; i < 10; i++)pushStack(stack, i);
    printf("Stack element:\n");
    stackTraverse(stack, printElem);
    printf("\nTop element:");
    DataType top;
    if(peekStack(stack, &top) == OK)printf("%d\n",top);
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
    stackDestruct(stack);
}

int main() {
    testLinkedStack();
    return 0;
}
