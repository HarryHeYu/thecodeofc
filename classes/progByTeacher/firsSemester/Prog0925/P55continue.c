#include<stdio.h>
int main(){
    int a=100,b=4,c,d;
    for(c=1;c<=5;c++){
        if(b-c==0){
            printf("Avoid division by zero.\n");
            continue;
        }
        d=a/(b-c);
        printf("%d/%d=%d\n",a,b-c,d);
    }
    return 0;
}