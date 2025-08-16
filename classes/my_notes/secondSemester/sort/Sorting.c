/* Sorting */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define DEBUG 0
#if DEBUG
    #define N 16
#else
    #define N 10000
    // N is the number of elements to be sorted.
#endif // DEBUG

#define COUNT_OPERATIONS 1
#if COUNT_OPERATIONS
    /* The following counters are used to count comparisons,
     * movements, and function calls, respectively. */
    _Alignas(64)
    unsigned int cmpCounter=0; // Comparisons
    unsigned int movCounter=0; // Movements
    unsigned int funCounter=0; // Function calls
    #define Less(x,y) (cmpCounter++,(x)<(y))
    #define LessEqual(x,y) (cmpCounter++,(x)<=(y))
    #define Greater(x,y) (cmpCounter++,(x)>(y))
    #define GreaterEqual(x,y) (cmpCounter++,(x)>=(y))
    #define Equal(x,y) (cmpCounter++,(x)==(y))
    #define Move(x,y) y=x; movCounter++
#else
    #define Less(x,y) ((x)<(y))
    #define LessEqual(x,y) ((x)<=(y))
    #define Greater(x,y) ((x)>(y))
    #define GreaterEqual(x,y) ((x)>=(y))
    #define Equal(x,y) ((x)==(y))
    #define Move(x,y) y=x
#endif // COUNT_OPERATIONS

typedef unsigned int Elem; // Type of elements to be sorted
_Alignas(64) Elem A[N];    // To be sorted
_Alignas(64) Elem B[N];    // To be sorted for comparison
_Alignas(64) Elem C[N];    // Auxiliary space

/* The following functions sort a[](n) into ascending order. */
void QuickSort(Elem a[],int n);
void MergeSort(Elem a[],int n);
void Merge(Elem a[], int m, int n);
void ShellSort(Elem a[],int n);
void ShellInsert(Elem a[], int n, int d);
void HeapSort(Elem a[],int n);
void RadixSort(Elem a[],int n);
void InsertionSort(Elem a[],int n);
void SelectionSort(Elem a[],int n);
void BubbleSort(Elem a[],int n);
void BinaryInsertionSort(Elem a[],int n);

int LowerBound(int n);
/* LowerBound returns the ceiling of log2(n!), which is
 * the minimum number of comparisons required to sort n
 * numbers in worst case. */

void PrintArray(unsigned int a[],int n);
/* Prints a[](n) on the screen. */

void RandomizeArray(unsigned int a[],int n);
/* Assigns 32-bit random numbers to a[0...n-1]. */

void Compare(Elem a[],Elem b[],int n);
/* If a[i]=b[i] for i=0...n-1, it prints OK.
 * Otherwise, it prints ERROR message on the screen. */

#define SWAP(x0,x1,temp)  {\
    Move(x0,temp);\
    Move(x1,x0);\
    Move(temp,x1);\
} /* End of SWAP */
#define INSERTION_SORT(a,n,i,j,temp,order)  {\
    for(i=1;i<(n);i++){\
        Move((a)[i],temp);\
        j=i-1;\
        while(j>=0 && order(temp,(a)[j])){\
            Move((a)[j],(a)[j+1]);\
            j--;\
        }\
        Move(temp,(a)[j+1]);\
    }\
} /* End of INSERTION_SORT. */

void InsertionSort(Elem a[],int n){
    #if DEBUG
    printf("%s begins:\n",__func__);
    PrintArray(a,n);
    #endif // DEBUG
    #if COUNT_OPERATIONS
    funCounter++;
    #endif // COUNT_OPERATIONS
    int i,j;
    Elem temp;
    INSERTION_SORT(a,n,i,j,temp,Less);
    #if DEBUG
    PrintArray(a,n);
    printf("%s ends.\n",__func__);
    #endif // DEBUG
}

/* This is the median-of-three quicksort. */
void QuickSort(Elem a[],int n){
    #if COUNT_OPERATIONS
    funCounter++;
    #endif // COUNT_OPERATIONS
    Elem *p,*q;
    Elem pivot,temp;
    int mid,i,j;
    if(n<=9){
    /* QuickSort is switched to INSERTION_SORT when n<=9,
     * as suggested by Robert Sedgewick in 1975. */
        INSERTION_SORT(a,n,i,j,temp,Less); // Macro
        return;
    }
    // Sort a[0],a[n/2],a[n-1] into order.
    p=a;
    q=a+n-1;
    mid=n>>1;
    if(Greater(*p,*q))     SWAP(*p,*q,temp);
    if(Greater(a[mid],*q)) SWAP(a[mid],*q,temp);
    if(Greater(*p,a[mid])) SWAP(*p,a[mid],temp);
    p++;
    Move(a[mid],pivot);
    Move(*p,a[mid]);
    Move(pivot,*p);
loop:
    do { p++; }while(Less(*p,pivot));
    do { q--; }while(Greater(*q,pivot));
    if(p<q){
        SWAP(*p,*q,temp);
        goto loop;
    }
/*左指针 p 从左向右移动，找到第一个大于等于枢轴的元素
右指针 q 从右向左移动，找到第一个小于等于枢轴的元素
如果 p 和 q 没有交叉，则交换它们指向的元素
重复此过程直到 p 和 q 交叉*/
    Move(*q,a[1]);
    Move(pivot,*q);
    // At the end of Split, j is the position of pivot.
    QuickSort(a,q-a);
    QuickSort(q+1,n-(q-a)-1);
}

void MergeSort(Elem a[],int n){
    #if DEBUG
    printf("%s begins:\n",__func__);
    PrintArray(a,N);
    #endif // DEBUG
    #if COUNT_OPERATIONS
    funCounter++;
    #endif // COUNT_OPERATIONS
    // printf("This is ASSIGNMENT 1.\n");
    if(n <= 1) return ;
    int m = n / 2;
    MergeSort(a, m);
    MergeSort(a + m, n - m);
    Merge(a, m, n);
    #if DEBUG
    PrintArray(a,N);
    printf("%s ends.\n",__func__);
    #endif // DEBUG
}

// void Merge(Elem a[], int m, int n){
//     #if COUNT_OPERATIONS
//     funCounter++;
//     #endif // COUNT_OPERATIONS
//     Elem *b = (Elem*)malloc(n * sizeof(Elem));
//     if(b == NULL){
//         printf("Memory allocation failed.\n");
//         exit(EXIT_FAILURE);
//     }
//     int i = 0, j = m, k = 0;
//     while(i < m && j < n){
//         if(Less(a[i], a[j])){
//             b[k++] = a[i++];
//         }else{
//             b[k++] = a[j++];
//         }
//     }
//     while(i < m) b[k++] = a[i++];
//     while(j < n) b[k++] = a[j++];
//     memcpy(a, b, n * sizeof(Elem));
//     free(b);
// }

void Merge(Elem a[], int m, int n){
    #if COUNT_OPERATIONS
    funCounter++;
    #endif
    int i = 0, j = m, k = 0;
    Elem *b = (Elem*)malloc(sizeof(Elem) * n);
    if(!b){
        printf("memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    while(i < m && j < n){
        if(a[i] < a[j]){
            Move(a[i],b[k]);
            i++;
        } else{
            Move(a[j],b[k]);
            j++;
        }
        k++;
    }
    while(i < m) Move(a[i++], b[k++]);
    while(j < n) Move(a[j++], b[k++]);
    memcpy(a, b, n * sizeof(Elem));
    free(b);
}

void ShellSort(Elem a[],int n){
    // printf("This is ASSIGNMENT 2.\n");
    int d;
    d = n / 3;
    while(d > 0){
        ShellInsert(a, n, d);
        if(d == 1) return ;
        d = d / 3 + 1;
    }
}

void ShellInsert(Elem a[], int n, int d){
    int i , j ;
    Elem temp;
    for(i = d; i < n; i++){
        temp = a[i];
        j = i - d;
        while(j >= 0 && temp < a[j]){
            a[j + d] = a[j];
            j -= d;
        }
        a[j + d] = temp;
    }
}

void SelectionSort(Elem a[],int n){
    // printf("This is ASSIGNMENT 3.\n");
    #if DEBUG
    printf("%s begins:\n",_func_);
    PrintArray(a,n);
    #endif //DEBUG
    #if COUNT_OPERATIONS
    funCounter++;
    #endif//
    int i, j, min;
    Elem temp;
    for(i = 0; i < n - 1; i++){
        min = i;
        for(j = i + 1; j < n; j++){
            if(Less(a[j], a[min])){
                min = j;
            }
            if(min != i){
                SWAP(a[i],a[min],temp);
            }
        }
    }
    #if DEBGU
    PrintArray(a,n);
    printf("%s ends.\n",_func_);
    #endif 
}

void BubbleSort(Elem a[],int n){
    for(int i = 0; i < n-1; i++){
        for(int j = 0; j < n-1-i; j++){
            if(a[j] > a[j+1]){
                Elem tmp = a[j];
                a[j] = a[j+1];
                a[j+1] = tmp;
            }
        }
    }
    // printf("This is ASSIGNMENT 4.\n");
    
}
void SelectionSort(Elem a[], int n){
    int j;
    for(int i = 0; i < n-1; i++){
        int min = i;
        for(j = i+1; j < n; j++){
            if(a[j] < a[min]) min = j;
        }
        if(min != i){
            Elem tmp = a[i];
            a[i] = a[min];
            a[min] = tmp;
        }
    }
}
void InsertionSort(Elem a[], int n){
    for(int i = 1; i < n; i++){
        Elem tmp = a[i];
        int j = i-1;
        while(j >= 0 && a[j] > tmp){
            a[j+1] = a[j];
            j--;
        }
        a[j+1] = tmp;
    }
}
void BinaryInsertionSort(Elem a[],int n){
    int high, low , mid, i, j;
    for(i = 1; i < n; i ++){
        low = 0;
        high = i - 1;
        Elem tmp = a[i];
        while(low <= high){
            mid = (low + high) / 2;
            if(a[mid] < tmp){
                low = mid + 1;
            } else{
                high = mid - 1;
            }
        }
        for(j = i-1; j >= low; j--){
            a[j+1] = a[j];
        }
        a[low] = tmp;
    }
    // printf("This is ASSIGNMENT 5.\n");
}

void Sift(int a[], int k, int m){
/*筛选a[k],使a[k...m]满足堆的性质*/
    int i, j, rootKey;
    rootKey = a[k];
    i = k;//i指向根
    j = 2*i;//j指向i的左孩子
    while(j <= m){
        if(j+1 <= m && a[j] < a[i+1]){//右孩子值比左孩子大
            j++;//指向右孩子
        }//让j指向最大的孩子
        if(rootKey >= a[j]){//根植大于等于孩子值
            break;// 已经找到rootKey应该放在的地方
        }else{
            a[i] = a[j];//把孩子值上移，然后继续筛
            i =j;
            j = 2 * i;
        }
    }
    a[i] = rootKey;
}
void CreateHeap(int a[],int n){
/*最后一个非叶子节点是n/2，从第n/2个节点开始，逐层向上，一个一个地筛*/
    int i;
    for(i = n/2; i >= 1; i--){
        Sift(a,i,n);
    }
    return;
}
void HeapSort(Elem a[],int n){
    int i, temp;
    CreateHeap(a,n);
    for(i = n; i >= 2; i--){
        temp = a[i];
        a[i] = a[1]; //a[1]是根，是最大值
        a[1] = temp; //把a[i]放到根上，然后筛
        Sift(a,1,i-1);
    }
    // printf("This is ASSIGNMENT 6.\n");
}
void RadixSort(Elem a[],int n){
    printf("This is ASSIGNMENT 7.\n");
}

void Compare(Elem a[],Elem b[],int n){
    int i;
    for(i=0;i<n;i++){
        if(a[i]!=b[i]){
            printf("ERROR.\n");
            printf("a[%u]=%u!=b[%u]=%u\n",i,a[i],i,b[i]);
            return;
        }
    }
    printf("OK.\n");
}

void PrintArray(unsigned int a[],int n){
    int i;
    for(i=0;i<n;i++){
        printf("%3u ",(unsigned int)a[i]);
    }
    printf("\n");
}

int LowerBound(int n){
    int i=1;
    double sum=0;
    for(i=1;i<=n;i++){
        sum+=log2(i);
    }
    sum=ceil(sum);
    return (int)sum;
}

void RandomizeArray(unsigned int a[],int n){
    int i,x,y,z;
    srand((unsigned)time(NULL));
    for(i=0;i<n;i++){
        x=rand();
        y=rand();
        z=rand();
        x=x<<18;
        y=y<<3;
        a[i]=x+y+z;
        #if DEBUG
        a[i]=rand()%1000;
        #endif
    }
}
void ClearCounters(){
    #if COUNT_OPERATIONS
    cmpCounter=0;
    movCounter=0;
    funCounter=0;
    #endif // COUNT_OPERATIONS
}

void PrintResult(double clocks,unsigned int nlogn){
    printf("Running time = %.2f clocks\n",clocks);
    #if COUNT_OPERATIONS
    printf("Comparisons C(n) = %9u = %.4f*n*log2(n).\n",
           cmpCounter,(double)(cmpCounter)/(double)(nlogn));
    printf("Movements   M(n) = %9u = %.4f*n*log2(n).\n",
           movCounter,(double)(movCounter)/(double)(nlogn));
    printf("Functions   F(n) = %9u.\n",funCounter);
    #endif // COUNT_OPERATIONS
    printf("\n");
}

int main(){
    unsigned int nlogn;
    clock_t clockStart,clockEnd,clocks;
    nlogn=(unsigned int)(N*log2(N));
    printf("Sorting Algorithms:\n");
    printf("n = %d\n",N);
    printf("Theoretical lower bound on comparisons:%d\n\n",
           LowerBound(N));
    RandomizeArray(A,N);
    memcpy(B,A,N*sizeof(Elem));
    memcpy(C,A,N*sizeof(Elem));

    printf("QuickSort (median-of-three):\n");
    ClearCounters();
    clockStart=clock();
    QuickSort(A,N);
    clockEnd=clock();
    clocks=clockEnd-clockStart;
    PrintResult((double)clocks,nlogn);

    printf("InsertionSort:\n");
    ClearCounters();
    clockStart=clock();
    InsertionSort(B,N);
    clockEnd=clock();
    clocks=clockEnd-clockStart;
    Compare(A,B,N);
    PrintResult((double)clocks,nlogn);

    printf("MergeSort:\n");
    memcpy(B,C,N*sizeof(Elem));
    ClearCounters();
    clockStart=clock();
    MergeSort(B,N);
    clockEnd=clock();
    clocks=clockEnd-clockStart;
    Compare(A,B,N);
    PrintResult((double)clocks,nlogn);

    printf("ShellSort:\n");
    ShellSort(B,N);
    PrintResult((double)clocks,nlogn);

    printf("SelectionSort:\n");
    SelectionSort(B,N);
    PrintResult((double)clocks,nlogn);

    printf("BubbleSort:\n");
    BubbleSort(B,N);
    PrintResult((double)clocks,nlogn);

    printf("BinaryInsertionSort:\n");
    BinaryInsertionSort(B,N);
    PrintResult((double)clocks,nlogn);

    printf("HeapSort:\n");
    HeapSort(B,N);
    PrintResult((double)clocks,nlogn);

    printf("RadixSort:\n");
    RadixSort(B,N);
    PrintResult((double)clocks,nlogn);

    return 0;
}
