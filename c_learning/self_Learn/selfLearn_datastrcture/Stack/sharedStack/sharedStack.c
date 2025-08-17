/***************************
 *  <name:共享栈（该栈为动态栈）
 *  <author: He YU Hao
 *  <date:2025.3.25
*****************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INITIAL_SIZE 20
#define INCREMENT_SIZE 5
typedef int DataType;
typedef enum{ ERROR = 0, OK = 1 }Status;

//共享栈结构定义
typedef struct  {
    DataType* data;
    int top1;//栈1的栈顶指针
    int top2;//栈2的栈顶指针
    int capacity;//当前的容量
}SharedStack;

//初始化共享栈
Status initStack(SharedStack* s){
    s->data = (DataType*)malloc(INITIAL_SIZE * sizeof(DataType));
    if(!s->data)return ERROR;
    s->top1 = -1;
    s->top2 = INITIAL_SIZE;
    s->capacity = INITIAL_SIZE;
    return OK;
}

//扩容栈
Status expandStack(SharedStack* s){
    int newCapacity = s->capacity + INCREMENT_SIZE;
    DataType* newData = (DataType*)realloc(s->data, newCapacity * sizeof(DataType));
    if (!newData) {
        // 若分配失败，释放原内存
        // free(s->data); 
        //这里不需要释放，因为他如果重新分配失败会自动保留原有内存
        return ERROR;
    }
/*如果原地址附近的内存不够会新创建一个数组，那么此时就会需要重新赋值了*/
    //调整栈2的top指针位置
    for (int i = 0; i < s->capacity - s->top2; i++) {
        newData[newCapacity - 1 - i] = s->data[s->capacity - 1 - i];
/*i - s->capacity 表示新分配内存空间相对于原数组末尾的偏移量，初始时这个偏移量为 0。
*/
    }
    s->top2 += INCREMENT_SIZE;
    s->data = newData;
    s->capacity = newCapacity;
    return OK;
}

//判断栈1是否为空
bool isStack1Empty(SharedStack* s){
    return s->top1 == -1;
}

//判断栈2是否为空
bool isStack2Empty(SharedStack* s){
    return s->top2 == s->capacity;
}

//判断共享栈是否已满
bool isSharedStackFull(SharedStack* s){
    return s->top1 + 1 == s->top2;
}

//入栈操作
Status pushStack(SharedStack* s, DataType item, int stackNumber){
    if(isSharedStackFull(s)){
        if(expandStack(s) == ERROR)return ERROR;
    }
    if(stackNumber == 1){
        s->data[++s->top1] = item;
    }else if(stackNumber == 2){
        s->data[--s->top2] = item;
    }else{
        printf("False stackNumber.\n");
        return ERROR;
    }
    return OK;
}

//出栈操作
Status popStack(SharedStack* s, DataType* item, int stackNumber){
    if(stackNumber == 1){
        if(isStack1Empty(s))return ERROR;
        *item = s->data[s->top1--];
    }else if(stackNumber == 2){
        if(isStack2Empty(s))return ERROR;
        *item = s->data[s->top2++];
    }else{
        printf("False stackNumber.\n");
        return ERROR;
    }
    return OK;
}

//查看栈顶元素
Status peekStack(SharedStack* s, DataType* item, int stackNumber){
    if(stackNumber == 1){
        if(isStack1Empty(s))return ERROR;
        *item = s->data[s->top1];
    }else if(stackNumber == 2){
        if(isStack2Empty(s))return ERROR;
        *item = s->data[s->top2];
    }else{
        printf("False stackNumber.\n");
        return ERROR;
    }
    return OK;
}

//清空栈
Status clearStack(SharedStack* s, int stackNumber){
    if(stackNumber == 1){
        while(s->top1 != -1){
            s->data[s->top1--] = 0;
        }
    }else if(stackNumber == 2){
        while(s->top2 != s->capacity){
            s->data[s->top2++] = 0;
        }
    }else{
        printf("False stackNumber.\n");
        return ERROR;
    }
    return OK;
}
/*也可以直接这样，这样做更高效，直接调整栈顶指针
Status clearStack(SharedStack* s, int stackNumber) {
    if (stackNumber == 1) {
        s->top1 = -1;
    } else if (stackNumber == 2) {
        s->top2 = s->capacity;
    } else {
        printf("False stackNumber.\n");
        return ERROR;
    }
    return OK;
}*/

//获取栈的长度
int stackLength(SharedStack* s, int stackNumber){
    if(stackNumber == 1){
        return s->top1 + 1;
    }else if(stackNumber == 2){
        return s->capacity - s->top2;
    }else{
        printf("False stackNumber.\n");
        return -1;//表示返回一个错误值
    }
}

//释放栈
Status stackFree(SharedStack* s){
    if(!isStack1Empty(s))clearStack(s,1);
    if(!isStack2Empty(s))clearStack(s,2);
    free(s->data);
    free(s);
    return OK;
}

//遍历栈
Status stackTraverse(SharedStack* s, void(*visit)(DataType), int stackNumber){
    if(stackNumber == 1){
        if(isStack1Empty(s))return ERROR;
        for(int i = 0; i <= s->top1; i++)visit(s->data[i]);
    }else if(stackNumber == 2){
        if(isStack2Empty(s))return ERROR;
        for(int i = s->top2; i < s->capacity; i++)visit(s->data[i]);
    }else{
        printf("False stackNumber.\n");
        return ERROR;
    }
    return OK;
}

//创建一个共享栈
SharedStack* stackConstruct(){
    SharedStack* s = (SharedStack*)malloc(sizeof(SharedStack));
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

//销毁共享栈
Status stackDestruct(SharedStack* s){
    stackFree(s);
    return OK;
}//其实该代码和释放栈的函数是一样的

//打印元素
void printElem(DataType elem){
    printf("%d\t",elem);
}

//排序栈中的元素
Status sortStack(SharedStack* s, int stackNumber){
    if(stackLength(s, stackNumber) < 2)return OK;
    if(stackNumber == 1){
        for(int i = 0; i < stackLength(s,1); i++){
            for(int j = i + 1; j < stackLength(s,1); j++){
                if(s->data[i] > s->data[j]){
                    DataType temp = s->data[i];
                    s->data[i] = s->data[j];
                    s->data[j] = temp;
                }
            }
        }
    }else if(stackNumber == 2){
        for(int i = s->capacity - 1; i >= s->top2; i--){
            for(int j = i - 1; j >= s->top2; j --){
                if(s->data[i] > s->data[j]){
                    DataType temp = s->data[i];
                    s->data[i] = s->data[j];
                    s->data[j] = temp;
                }
            }
        }
    }else{
        printf("False stackNumber.\n");
        return ERROR;
    }
    return OK;
}

//检查括号匹配
Status checkBrackets(SharedStack* s, const char* expr){
    for(int i = 0; expr[i] != '\0'; i++){
        if (expr[i] == '(' || expr[i] == '{' || expr[i] == '['){
            if(pushStack(s, expr[i], 1) == ERROR)return ERROR;
        }else if (expr[i] == ')' || expr[i] == '}' || expr[i] == ']'){
            DataType top;
            if(isStack1Empty(s) ||popStack(s,&top,1) == ERROR)return ERROR;
            if ((expr[i] == ')' && top != '(') ||
                (expr[i] == '}' && top != '{') ||
                (expr[i] == ']' && top != '[')) {
                return ERROR;
            }
        }
    }
    return isStack1Empty(s) ? OK : ERROR;
}

//求后缀表达式的值
Status evaluatePostfix(SharedStack* s, char exp[], int* result){
    for(int i = 0; exp[i] != '\0'; i++){
        if('0' <=exp[i] && exp[i] <= '9'){
            if(pushStack(s,exp[i]-'0',1) == ERROR)return ERROR;
        }else{
            DataType b, a;
            if(popStack(s,&b,1) == ERROR || popStack(s,&a,1) == ERROR)return ERROR;
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
            if(pushStack(s,c,1) == ERROR)return ERROR;
        }
    }
    return popStack(s,result,1);
}

/* 测试函数 */
void testSharedStack() {
    SharedStack* stack = stackConstruct();
    for (int i = 0; i < 10; i++) {
        pushStack(stack, i, 1);
        pushStack(stack, i + 10, 2);
    }
    printf("Stack 1 elements: ");
    stackTraverse(stack, printElem, 1);
    printf("\nStack 2 elements: ");
    stackTraverse(stack, printElem, 2);
    printf("\nTop element of Stack 1: ");
    DataType top1;
    if (peekStack(stack, &top1, 1) == OK) {
        printf("%d\n", top1);
    }
    printf("Top element of Stack 2: ");
    DataType top2;
    if (peekStack(stack, &top2, 2) == OK) {
        printf("%d\n", top2);
    }
    sortStack(stack, 1);
    printf("Sorted Stack 1 elements: ");
    stackTraverse(stack, printElem, 1);
    printf("\nSorted Stack 2 elements: ");
    sortStack(stack, 2);
    stackTraverse(stack, printElem, 2);
    printf("\n");

    const char* expr = "{[()]}";
    printf("Bracket matching for \"%s\": %s\n", expr, checkBrackets(stack, expr) == OK ? "matched" : "not matched");

    char postfixExp[] = "512+4*+3-";
    int result;
    if (evaluatePostfix(stack, postfixExp, &result) == OK) {
        printf("Postfix expression evaluation for \"%s\": %d\n", postfixExp, result);
    }

    DataType x;
    while (!isStack1Empty(stack)) {
        if (popStack(stack, &x, 1) == OK) {
            printf("Popped element from Stack 1: %d\n", x);
        }
    }
    while (!isStack2Empty(stack)) {
        if (popStack(stack, &x, 2) == OK) {
            printf("Popped element from Stack 2: %d\n", x);
        }
    }
    stackDestruct(stack);
}

int main() {
    testSharedStack();
    return 0;
}