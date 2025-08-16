/* Array */
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define N 10
#define MAX 100

void ClearArray(int a[],int n);
void PrintArray(int a[],int n);
int MaxOfArray(int a[],int n);
void RandomizeArray(int a[],int n,int max);

void ScalarMult(int a[],int n,int k);
void PlusVector(int a[],int b[],int n);
int ScalarProduct(int a[],int b[],int n);

void ClearArray(int a[],int n){
    int i;
    for(i=0;i<n;i++){
        a[i]=0;
    }
    return;
}

void PrintArray(int a[],int n){
    int i;
    for(i=0;i<n;i++){
        printf("%d\t",a[i]);
    }
    return;
}

int MaxOfArray(int a[],int n){
    int i,max;
    max=a[0];
    for(i=1;i<n;i++){
        if(max<a[i]){
            max=a[i];
        }
    }
    return max;
}

void RandomizeArray(int a[],int n,int max){
    int i;
    srand((unsigned)time(NULL));
    for(i=0;i<n;i++){
        a[i]=rand()%(max+1);
    }
    return;
}

void ScalarMult(int a[],int n,int k){
    int i;
    for(i=0;i<n;i++){
        a[i]*=k;
    }
    return;
}

void PlusVector(int a[],int b[],int n){
    int i;
    for(i=0;i<n;i++){
        a[i]+=b[i];
    }
    return;
}

int ScalarProduct(int a[],int b[],int n){
    int i;
    int product=0;
    for(i=0;i<n;i++){
        product+=a[i]*b[i];
    }
    return product;
}

int main(){
    int a[N];
    RandomizeArray(a,N,MAX);
    PrintArray(a,N);
    return 0;
}

