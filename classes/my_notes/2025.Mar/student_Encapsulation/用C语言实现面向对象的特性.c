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
    void (*print)(void* this);
    // Please add more members.
    // To do ...
};
void StudentListInit(StudentList* pList);
void StudentListIntroduce(void* this);
Status StudentListAppend(void* this,Student* pStudent);
void StudentListPrint(void* this);

typedef struct ZClass MrZhangsClass;
struct ZClass{
    struct StudentList; /* Inheritance */
    int head;
    // Please add more attributes (member variables)
    // and more methods (member functions).
    // To do ...
};
void ZClassInit(MrZhangsClass *this);
void ZClassIntoduce(void *this);

typedef struct WClass MrsWangsClass;
struct WClass{
    struct StudentList;
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
    this->introduce=WClassIntoduce;
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
