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
     typedef enum { false = 0, true = 1 } bool;
 #endif // __STDC_VERSION
 #define N 100
 #define MAX_LENGTH_OF_NAME 30
 #define SIZE_OF_CLASS 30
 #define STUDENT(name) {0, #name, 100, 100, 100}
 typedef enum { ERROR = 0, OK = 1 } Status;
 typedef struct {
     int number;
     char name[MAX_LENGTH_OF_NAME];
     int chinese;
     int math;
     int english;
 } Student;
 void PrintStudent(Student *pStudent);
 void PrintStudents(Student s[], int n);
 /* Overloading */
 #define PrintHead(x) _Generic((x), int: PrintHeadWithNumber, \
                                 char*: PrintHeadWithName \
                               )(x)
 void PrintHeadWithNumber(int number);
 void PrintHeadWithName(char* name);
 
 typedef struct StudentList StudentList;
 struct StudentList {
     /* Encapsulation */
     // Attributes (member variables)
     Student a[N];
     int n;
     int size;
     // Methods (member functions)
     void (*introduce)(void*);
     Status (*append)(void*, Student*);
     Status (*remove)(void*, int);
     Student* (*search)(void*, int);
     Status (*changeData)(void*, int, int, int, int);
     void (*print)(void* this);
     // Please add more members.
     // To do ...
 };
 void StudentListInit(StudentList* pList);
 void StudentListIntroduce(void* this);
 Status StudentListAppend(void* this, Student* pStudent);
 Status StudentListRemove(void* this, int num);
 Student* StudentListSearch(void* this, int num);
 Status StudentListChangeData(void* this, int num, int chinese, int math, int english);
 void StudentListPrint(void* this);
 
 typedef struct ZClass {
     struct StudentList; /* Inheritance */
     int head;
     // Please add more attributes (member variables)
     // and more methods (member functions).
     // To do ...
 } MrZhangsClass;
 
 void ZClassInit(MrZhangsClass *this);
 void ZClassIntroduce(void *this);
 Status ZClassAppend(void* this, Student* pStudent);
 Status ZClassRemove(void* this, int num);
 Student* ZClassSearch(void* this, int num);
 Status ZClassChangeData(void* this, int num, int chinese, int math, int english);
 
 typedef struct WClass {
     struct StudentList; /* Inheritance */
     char head[MAX_LENGTH_OF_NAME];
     // Please add more attributes (member variables)
     // and more methods (member functions).
     // To do ...
 } MrsWangsClass;
 
 void WClassInit(MrsWangsClass *this);
 void WClassIntroduce(void *this);
 Status WClassAppend(void* this, Student* pStudent);
 Status WClassRemove(void* this, int num);
 Student* WClassSearch(void* this, int num);
 Status WClassChangeData(void* this, int num, int chinese, int math, int english);
 
 void PrintStudent(Student* pStudent){
     printf("No.%2d, %8s, Chinese=%d, Math=%d, English=%d\n",
            pStudent->number, pStudent->name,
            pStudent->chinese, pStudent->math, pStudent->english);
 }
 void PrintStudents(Student s[], int n){
     int i;
     for(i = 0; i < n; i++){
         PrintStudent(s + i);
     }
 }
 void StudentListInit(StudentList* this){
     this->size = SIZE_OF_CLASS;
     this->n = 0;
     this->introduce = StudentListIntroduce;
     this->append = StudentListAppend;
     this->remove = StudentListRemove;
     this->search = StudentListSearch;
     this->changeData = StudentListChangeData;
 }
 void ZClassInit(MrZhangsClass *this){
     StudentListInit((StudentList*)this);
     this->head = 0;
     this->introduce = ZClassIntroduce;
     this->append = ZClassAppend;
     this->remove = ZClassRemove;
     this->search = ZClassSearch;
     this->changeData = ZClassChangeData;
 }
 void WClassInit(MrsWangsClass *this){
     StudentListInit((StudentList*)this);
     this->head[0] = '\0';
     this->introduce = WClassIntroduce;
     this->append = WClassAppend;
     this->remove = WClassRemove;
     this->search = WClassSearch;
     this->changeData = WClassChangeData;
 }
 void StudentListIntroduce(void* this){
     StudentList *pThis = (StudentList*)this;
     printf("\nThis is a list of %d students.\n\n", pThis->n);
 }
 void ZClassIntroduce(void *this){
     MrZhangsClass *pThis;
     pThis = (MrZhangsClass*)this;
     printf("\nThis is Mr. Zhang's class.\n");
     if(pThis->n > 0){
         printf("There are %d boys and girls.\n", pThis->n);
         PrintHead(pThis->head);
     }
     printf("Welcome to join us.\n\n");
 }
 void WClassIntroduce(void *this){
     MrsWangsClass *pThis;
     pThis = (MrsWangsClass*)this;
     printf("\nThis is Mrs. Wang's class.\n");
     if(pThis->n > 0){
         printf("There are %d boys and girls.\n", pThis->n);
         PrintHead(pThis->head);
     }
     printf("Welcome to join us.\n\n");
 }
 void PrintHeadWithNumber(int number){
     if(number <= 0){
         printf("The head boy or girl hasn't been appointed.\n");
     }
     else{
         printf("The head boy or girl is No.%d\n", number);
     }
 }
 void PrintHeadWithName(char* name){
     if(name[0] == '\0'){
         printf("The head boy or girl hasn't been appointed.\n");
     }
     else{
         printf("The head boy or girl is %s\n.", name);
     }
 }
 
 Status StudentListAppend(void* this, Student* pStudent){
     StudentList* pList = (StudentList*)this;
     if(pList->n == pList->size){
         printf("Full.\n");
         return ERROR;
     }
     pList->a[pList->n] = *pStudent;
     pList->a[pList->n].number = pList->n + 1;
     (pList->n)++;
     return OK;
 }
 
 Status StudentListRemove(void* this, int num){
     StudentList* pList = (StudentList*)this;
     if(num < 1 || num > pList->n){
         printf("Invalid number.\n");
         return ERROR;
     }
     for(int i = num; i < pList->n; i++){
         pList->a[i-1] = pList->a[i];
         pList->a[i-1].number = i;
     }
     pList->n--;
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
 
 Status StudentListChangeData(void* this, int num, int chinese, int math, int english){
     StudentList* pList = (StudentList*)this;
     if(num < 1 || num > pList->n){
         printf("Invalid number\n");
         return ERROR;
     }
     pList->a[num-1].chinese = chinese;
     pList->a[num-1].math = math;
     pList->a[num-1].english = english;
     return OK;
 }
 
 Status ZClassAppend(void* this, Student* pStudent){
     return StudentListAppend(this, pStudent);
 }
 
 Status ZClassRemove(void* this, int num){
     return StudentListRemove(this, num);
 }
 
 Student* ZClassSearch(void* this, int num){
     return StudentListSearch(this, num);
 }
 
 Status ZClassChangeData(void* this, int num, int chinese, int math, int english){
     return StudentListChangeData(this, num, chinese, math, english);
 }
 
 Status WClassAppend(void* this, Student* pStudent){
     return StudentListAppend(this, pStudent);
 }
 
 Status WClassRemove(void* this, int num){
     return StudentListRemove(this, num);
 }
 
 Student* WClassSearch(void* this, int num){
     return StudentListSearch(this, num);
 }
 
 Status WClassChangeData(void* this, int num, int chinese, int math, int english){
     return StudentListChangeData(this, num, chinese, math, english);
 }
 
 int main(){
     int i, n;
     // studentBase may get data from a file.
     static Student studentBase[N] = {STUDENT(Zhang1),
                    STUDENT(Wang2), STUDENT(Li3),
                    STUDENT(Zhao4), STUDENT(Liu5)};
     n = 5;
     MrZhangsClass zClass;
     MrsWangsClass wClass;
     ZClassInit(&zClass);
     WClassInit(&wClass);
     for(i = 0; i < n; i++){
         if(i % 2 == 1){
             zClass.append(&zClass, &studentBase[i]);
         }
         else{
             wClass.append(&wClass, &studentBase[i]);
         }
     }
     zClass.introduce(&zClass);
     StudentList* pList = (StudentList*)(&wClass);
     pList->introduce(pList); /* Polymorphism */
     return 0;
 }