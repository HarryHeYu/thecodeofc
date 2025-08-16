/*Sorting */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEBUG 0
//1时为调试模式，0时为不调试模式
#if DEBUG
    #define N 10
    //N is the number of elements to be sorted
    #define MAX_IN_DEBUD_MODE 100
#else
    #define N 10000
#endif

/*Let's use a[] to denote an array,where
 a is the address of the array,
 and n is the length of the array.
 */
void Randomize(int a[],int n);
//This function assigns a[](n) random numbers.

void Print(int a[],int n);
//This function prints a[](n) on the screen.

//The following three functions sort a[](n) in ascending order.
void BubbleSort(int a[],int n);
//This function sorts a[](n) in ascending order.(从小到大排序)
/*冒泡排序：比较相邻的元素。如果第一个比第二个大，就交换他们两个。
对每一对相邻元素做同样的工作，从开始第一对到结尾的最后一对。这步做完后，最后的元素会是最大的数。
针对所有的元素重复以上的步骤，除了最后一个。
重复步骤1~3，直到排序完成。
最大的泡泡先冒上来（最先到位）*/
void SelectionSort(int a[],int n);
//This function sorts a[](n) in ascending order.
/*选择函数：初始化：在未排序序列中找到最小（大）元素，存放到排序序列的起始位置。
遍历：从剩余未排序元素中继续寻找最小（大）元素，然后放到已排序序列的末尾。
重复：重复第二步，直到所有元素均排序完毕。*/
void InsertionSort(int a[],int n);
//This function sorts a[](n) in ascending order.
/*插入排序：从第二个元素开始，假设第一个元素已经是有序的。
将当前元素与已排序的元素进行比较，找到合适的位置插入。
将当前元素插入到找到的位置，并将后面的元素向后移动。
重复以上步骤，直到所有元素都被插入到正确的位置。*/

int MinOfArray(int a[],int n);

void BubbleSort(int a[],int n){
    int i,j,temp;
    for(i=0;i<n-1;i++){
        /*i=0次,n-1;i=1,n-2;i=2,n-3....i,n-1-i*/
        for(j=0;j<n-1-i;j++){
            if(a[j]>a[j+1]){
                temp=a[j];
                a[j]=a[j+1];
                a[j+1]=temp;
            }
        }
    }
}

void Randomize(int a[],int n){
    int i;
    srand((unsigned)time(NULL));
    for(i=0;i<n;i++){
        #if DEBUG
        a[i]=rand()%MAX_IN_DEBUD_MODE;
        #else
        a[i]=rand();//不调试的话就不做限制
        #endif
    }
    return;
}

void Print(int a[],int n){
   int *p;
   for(p=a; p<a+n; p++){
    printf("%d\t",*p);
   }
   return;
}

int MinOfArray(int a[],int n){
    int min,i;
    min = 0;
    for(i=1;i<n;i++){
        if(a[i]<a[min]){
            min=i;
        }
    }
    return min;
}

void SelectionSort(int a[],int n){
    int i,min,j,temp;
    for(i=0;i<n-1;i++){
        min=i;
        for(j=i+1;j<n;j++){
            if(a[j]<a[min]){
                min=j;
            }
        }
        temp=a[i];
        a[i]=a[min];
        a[min]=temp;
    }
}
/*外层循环（for(i=0;i<n-1;i++)）：

这个循环控制排序的轮数。由于数组的最后一个元素在第 n-1 轮排序后自然成为最大（或最小）的，所以只需要进行 n-1 轮。
初始化最小值索引（min=i）：

在每一轮排序开始时，将当前轮的起始索引 i 赋值给 min，表示当前轮中最小元素的索引。
内层循环（for(j=i+1;j<n;j++)）：

这个循环用于在未排序的数组部分（从索引 i+1 到 n-1）寻找最小元素。
如果找到一个比当前 min 索引处更小的元素，就更新 min 的值。
交换元素：

内层循环结束后，min 将指向这一轮中找到的最小元素的索引。
如果 min 不等于 i（即最小元素不在当前轮的起始位置），则将 i 索引处的元素与 min 索引处的元素交换。
更新数组：

使用临时变量 temp 来帮助完成交换操作，确保交换过程中不会丢失任何元素*/

void InsertionSort(int a[],int n){
    int i, j,temp;
    for(i=1; i<n; i++){
        temp=a[i];
        j=i-1;
        while(j>=0 && temp<a[j]){
            a[j+1]=a[j];
            j--;
        }
    }
}
/*外层循环（for(i=1; i<n; i++)）：

这个循环从数组的第二个元素开始（索引为1），一直到数组的最后一个元素。因为插入排序默认第一个元素（索引为0）是已排序的。
临时变量（temp=a[i];）：

将当前要插入的元素赋值给临时变量 temp。
内层循环（while(j>=0 && temp<a[j]){...}）：

这个循环用于在已排序的数组部分（从当前元素的前一个元素开始）找到 temp 应该插入的位置。
只要 j 是非负数且 temp 小于 a[j]，就将 a[j] 向后移动一个位置。
元素移动（a[j+1]=a[j];）：

将 a[j] 的值复制到 a[j+1]，为 temp 插入腾出空间。
更新索引（j--;）：

减少 j 的值，继续向前比较。
插入元素：

当内层循环结束时（即找到了正确的插入位置或到达了数组的开始），将 temp 插入到正确的位置 a[j+1]。*/

int main(){
    int a[N];
    #if DEBUG
        printf("Running in the debug mode.\n");
    #endif
    Randomize(a,N);
    Print(a,N);
    // BubbleSort(a,N);
    SelectionSort(a,N);
    printf("\n Sorted2:\n");
    Print(a,N);
    return 0;
}