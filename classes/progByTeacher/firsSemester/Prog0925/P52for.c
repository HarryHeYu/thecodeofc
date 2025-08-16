#include<stdio.h>
int main(){
    int i,n,p,val;
    for(i=0;i<10;i++){
        printf("%d\t",i);
    }
    printf("\n");
    
    /* Multiple innializations and increments */
    for(p=0,n=16,val=1; p<n; p++,val*=2){
        printf("%d\t%d\n",p,val);
    }
    return 0;
}