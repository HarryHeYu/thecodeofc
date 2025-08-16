/*该代码和student2一样都是封装、继承、重载的讲解
但是该代码与student2有区别
该代码目前无法正常编译，student2与该代码的区别是：student2的两个子类中是使用struct来进行匿名继承的*/

/* Implement object-oriented features using the C language.
 * 1. Encapsulation
 * 2. Inheritance
 * 3. Overloading
 * 4. Polymorphism
 */
#include <stdio.h>
#include <stdlib.h>
#define C24_VERSION 202311L
#if __STDC_VERSION < C24_VERSION
    #define nullptr NULL
    typedef enum{false=0,true=1}bool;
#endif // __STDC_VERSION
#define N 100
#define MAX_LENGTH_OF_NAME 30
#define SIZE_OF_CLASS 30
#define STUDENT(name) {0,#name,100,100,100}
/*{0,#name,100,100,100}：这是宏的替代文本，每当宏 STUDENT(name) 被调用时，预处理器会用这个替代文本替换宏调用。
具体细节
0：表示学生的编号，初始值为 0。
#name：这是宏中的字符串化操作符 #，用于将宏参数 name 转换为字符串字面量。例如，如果调用 STUDENT(John)，#name 会被替换为 "John"。
100,100,100：这三个值表示学生的三科成绩（语文、数学、英语），初始值都为 100。*/

typedef enum{ERROR=0,OK=1}Status;
typedef struct{
    int number;
    char name[MAX_LENGTH_OF_NAME];
    int chinese;
    int math;
    int english;
}Student;
void PrintStudent(Student *pStudent);
void PrintStudents(Student s[],int n);
/* Overloading */
#define PrintHead(x) _Generic((x),int:PrintHeadWithNumber,\
                                char*:PrintHeadWithName\
                              )(x)
                              //如果x的类型是int，则调用PrintHeadWithNumber函数；如果x的类型是char*（字符串指针），则调用PrintHeadWithName函数。
//用宏定义重载

//在_Generic后面有两个x，第一个x用于指定要根据其类型进行判断的表达式或变量，第二个x在int:PrintHeadWithNumber,char*:PrintHeadWithName)(x)中，是作为实际调用PrintHeadWithNumber或PrintHeadWithName函数时传递的参数。
/*在#define PrintHead(x) _Generic((x),int:PrintHeadWithNumber,char*:PrintHeadWithName)(x) 这段代码中，两个x的作用分别如下：

1. 第一个x，即_Generic((x),... 中的x：

◦ 它的作用是作为_Generic关键字判断类型的依据。_Generic会对这个表达式（这里是x）进行类型检查，通过检查x的具体数据类型，来决定后续应该选择哪一个分支。

◦ 比如当调用PrintHead(num)时（num是int类型变量），_Generic就会检查num（也就是x在这次调用中的实际值）的类型为int，然后选择int类型对应的处理函数PrintHeadWithNumber。

2. 第二个x，即...)(x) 中的x：

◦ 这个x是在确定了要调用的函数（通过_Generic的类型判断）之后，作为参数传递给被选中的函数的。

◦ 接着上面的例子，当_Generic判断出x（即num）的类型为int，决定调用PrintHeadWithNumber函数后，x（num）就会作为参数传递给PrintHeadWithNumber函数，就相当于执行了PrintHeadWithNumber(num) ，函数内部就可以使用这个传递进来的参数进行相应的操作。

所以，第一个x主要用于类型判断，第二个x用于传递参数给具体的函数。*/
void PrintHeadWithNumber(int number);
void PrintHeadWithName(char* name);

typedef struct StudentList StudentList;
struct StudentList{
    /* Encapsulation */
    // Attributes (member variables)
    Student a[N];
    int n;
    int size;
    // Methods (member functions)
    void (*introduce)(void*);
    Status (*append)(void*,Student*);
    void (*print)(void* this);//用指向函数的指针把函数包进来
    // Please add more members.
    // To do ...
};
void StudentListInit(StudentList* pList);
void StudentListIntroduce(void* this);
Status StudentListAppend(void* this,Student* pStudent);
void StudentListPrint(void* this);

typedef struct ZClass MrZhangsClass;
struct ZClass{
    StudentList; /* Inheritance */ //里匿名继承       就这样不定义一个变量，就继承了
    int head;
    // Please add more attributes (member variables)
    // and more methods (member functions).
    // To do ...
};
void ZClassInit(MrZhangsClass *this);//构造函数不应该被包进对象，因为在构造前对象尚不存在--->对应的虽然析构函数可以包，但是为了对称就不包了
/*对象还未创建：

构造函数的主要任务是初始化对象的状态（如成员变量），在对象创建之前，成员变量是未定义的。因此，构造函数需要在对象创建时被调用，而不是作为对象的成员函数存在，因为此时对象还不存在。
避免自引用问题：

如果构造函数作为对象的成员函数存在，调用它时需要先有一个有效的对象实例，这就形成了一个自引用的问题：对象需要存在才能调用构造函数，但构造函数的任务是初始化对象。
初始化逻辑独立：

构造函数的初始化逻辑通常是独立的，它不依赖于对象的其他成员函数或状态。因此，将构造函数独立于对象之外，可以更好地实现初始化逻辑的封装和复用。
*/
void ZClassIntoduce(void *this);

typedef struct WClass MrsWangsClass;
struct WClass{
    StudentList;
    char head[MAX_LENGTH_OF_NAME];
    // Please add more attributes (member variables)
    // and more methods (member functions).
    // To do ...
};
void WClassInit(MrsWangsClass *this);
void WClassIntoduce(void *this);

void PrintStudent(Student* pStudent){
    printf("No.%2d,%8s, Chinese=%d, Math=%d, English=%d\n",
           pStudent->number,pStudent->name,
           pStudent->chinese,pStudent->math,pStudent->english);
}
void PrintStudents(Student s[],int n){
    int i;
    for(i=0;i<n;i++){
        PrintStudent(s+i);
    }
}
void StudentListInit(StudentList* this){
    this->size=SIZE_OF_CLASS;
    this->n=0;
    this->introduce=StudentListIntroduce;
    this->append=StudentListAppend;
}
void ZClassInit(MrZhangsClass *this){
    StudentListInit((StudentList*)this);
    this->head=0;
    this->introduce=ZClassIntoduce;
}
void WClassInit(MrsWangsClass *this){
    StudentListInit((StudentList*)this);
    this->head[0]='\0';
    (this)->introduce=WClassIntoduce;
}
void StudentListIntroduce(void* this){
    StudentList *pThis=(StudentList*)this;
    printf("\nThis is a list of %d students.\n\n",pThis->n);
}
void ZClassIntoduce(void *this){
    MrZhangsClass *pThis;
    pThis=(MrZhangsClass*)this;
    printf("\nThis is Mr. Zhang's class.\n");
    if(pThis->n>0){
        printf("There are %d boys and girls.\n",pThis->n);
        PrintHead(pThis->head);
    }
    printf("Welcome to join us.\n\n");
}
void WClassIntoduce(void *this){
    MrsWangsClass *pThis;
    pThis=(MrsWangsClass*)this;
    printf("\nThis is Mrs. Wang's class.\n");
    if(pThis->n>0){
        printf("There are %d boys and girls.\n",pThis->n);
        PrintHead(pThis->head);
    }
    printf("Welcome to join us.\n\n");
}
void PrintHeadWithNumber(int number){
    if(number<=0){
        printf("The head boy or girl hasn't been appointed.\n");
    }
    else{
        printf("The head boy or girl is No.%d\n",number);
    }
}
void PrintHeadWithName(char* name){
    if(name[0]=='\0'){
        printf("The head boy or girl hasn't been appointed.\n");
    }
    else{
        printf("The head boy or girl is %s\n.",name);
    }
}
Status StudentListAppend(void* this,Student* pStudent){
    StudentList* pList=(StudentList*)this;
    if(pList->n==pList->size){
        printf("Full.\n");
        return ERROR;
    }
    pList->a[pList->n]=*pStudent;
    pList->a[pList->n].number=pList->n+1;
    (pList->n)++;
    return OK;
}
Status StudentListDelete(void* this, int num){
    StudentList* pList = (StudentList*)this;
    if(num < 1 || num > pList->n){
        printf("Invalid number.\n");
        return ERROR;
    }
    for(int i = num; i < pList->n; i++){
        pList->a[i-1] = pList->a[i];
        pList->a[i-1].number = i;
    }
    return OK;
}
Student* StudentListSearch(void* this, int num){
    StudentList* pList = (StudentList*)this;
    if(num < 1 || num > pList->n){
        printf("Invalid number.\n");
        return nullptr;
    }
    return &pList->a[num-1];
}
Status StudentChangeData(void* this, int num, int chinese, int math , int english){
    StudentList* pList = (StudentList*)this;
    if(num < 1 || num > pList->n){
        printf("Invalid number1\n");
        return ERROR;
    }
    pList->a[num-1].chinese = chinese;
    pList->a[num-1].math = math;
    pList->a[num-1].english = english;
    return OK;
}
int main(){
    int i,n;
    // studentBase may get data from a file.
    static Student studentBase[N]={STUDENT(Zhang1),
                   STUDENT(Wang2),STUDENT(Li3),
                   STUDENT(Zhao4),STUDENT(Liu5)};
    n=5;
    MrZhangsClass zClass;
    MrsWangsClass wClass;
    ZClassInit(&zClass);
    WClassInit(&wClass);
    for(i=0;i<n;i++){
        if(i%2==1){
            zClass.append(&zClass,&studentBase[i]);
        }
        else{
            wClass.append(&wClass,&studentBase[i]);
        }
    }
    zClass.introduce(&zClass);
    StudentList* pList=(StudentList*)(&wClass);
    pList->introduce(pList); /* Polymorphism */
    return 0;
}