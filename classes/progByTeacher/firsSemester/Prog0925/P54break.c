#include<stdio.h>
int main(){
    int i;
    for(i=0;i<=20;i++){
        if(i*i>150){
            break;
        }
        printf("%d\t%d\n",i,i*i);
    }
    return 0;
}