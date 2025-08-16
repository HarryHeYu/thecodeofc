void BubbleSort1(int a[], int n);
void BubbleSort2(int a[], int n);

void BubbleSort1(int a[], int n){
    int temp;
    for(int i = 0; i < n-1; i++){
        for(int j = 0; j < n-1-i; j++){
            if(a[j] > a[j+1]){
                temp = a[j+1];
                a[j+1] = a[j];
                a[j] = temp;
            }
        }
    }
}

void BubbleSort2(int a[], int n){
    int temp;
    for(int i = 0; i < n-1; ++i){
        for(int j = 0; j < n-1-i; ++j){
            if(a[j] > a[j++]){
                temp = a[j-1];
                a[j-1] = a[j];
                a[j] = temp;
            }
        }
    }
}

void SelctionSort(int a[], int n);
void SelectionSort(int a[], int n){
    int i, j, min, temp;
    for(i = 0; i < n-1; ++i){
        min = i;
        for(j = i+1; j < n; ++j){
            if(a[j] < a[min]){
                min = j;
            }
        }
        if (min != i) {
            temp = a[i];
            a[i] = a[min];
            a[min] = temp;
        }
    }
}

void InsertSort(int a[], int n);
void InsertSort(int a[], int n){
    int i, j ,temp;
    for(i = 1; i < n; ++i){
        temp = a[i];
        j = i - 1;
        while(j >= 0 && temp < a[j]){
            a[j+1] = a[j];
            j--;
        }
        a[j+1] = temp;
    }
}

void MergeSort(int a[], int n);
void MergeSort(int a[], int n){
    int m;
    if(n <= 1){
        return;
    }
    m = n/2;
    MergeSort(a,m);
    MergeSort(&a[m], n-m);//相当于是换了新的数组进行排序，所以可以直接取地址&
    //MergeSort 是 逻辑上的分割，直接操作数组的地址范围，不改变原数组顺序。
    Merge(a,m,n);
}//前面的递归调用函数的目的只是不断地将数组拆分至单个元素
//而真正的排序是在后面回溯中合并的过程中进行排序
//由单个元素开始合并排序，慢慢回溯直至完成整个

void Merge(int a[], int m, int n){
    int i = 0, j = m, k = 0;
    int b[n];// In stack
    while(i < m && j < n){
        if(a[i] < a[j]){
            b[k] = a[i];
            i++;
        }else{
            b[k] = a[j];
            j++;
        }
        k++;
    }
    while(i < m){
        b[k] = a[i];
        i++;
        k++;
    }
    while(j < n){//This loop can be omitted.
        b[k] = a[j];
        j++;
        k++;
    }
    for(i = 0; i < k; i++){
        a[i] = b[i];
    }
}

void QuickSort(int a[], int n);
void QuickSort(int a[], int n){
    int k;
    if(n <= 1){
        return;
    }
    k = Split(a, n);//Returns the index of the pivot
    QuickSort(a, k);
    QuickSort(a+k+1, n-k-1);//之所以不用&a[k]是因为函数里调用的是整个数列
    //他是在整个数组里进行交换以及排序
    //QuickSort 是 物理上的分割，实际调整数组内容顺序，因此需要使用分割点 k 计算子数组范围
}

int Split(int a[], int n);
int Split(int a[], int n){
    int pivot, low, high, temp;
    pivot = a[0];
    low = 1;
    high = n-1;
    while(low < high){
        while(a[low] < pivot && low < high){
            low++;
        }
        while(a[high] >= pivot && low < high){
            high--;
        }
        if(low < high){//这意味着a[low]大于了pivot或者说a[high]大于了pivot，所以说要将两者对换
            temp = a[low];
            a[low] = a[high];
            a[high] = temp;
            low++;
            high--;
        }
    }
    if(a[high] >= pivot){
        high--;
    }
    a[0] = a[high];
    a[high] = pivot;
    return high;
}
/*MergeSort(&a[m], n-m) 依赖指针直接处理后半部分，a[m] 指向数组中间的位置，子数组仍然在原数组的一部分。
QuickSort(a + k + 1, n-k-1) 则假设 k 已经划分了数组的范围，左右部分互不干扰。*/

