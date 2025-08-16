#include<stdio.h>
int main(){
    int i,j;
    i=0;
    while(i<3){
        j=0;
        while(j<3){
            printf("%d%d\t",i,j);
            j++;
        }
        printf("\n");
        i++;
    }
    return 0;
}