/* Spaghetti */
// Goto statement considered HARMFUL.
// DO NOT write such code as follows.
#include<stdio.h>
int main(){
    int i;
    i=0;
t1: if(i<10) goto t2;
    else goto t3;
t2: printf("%d\t",i);
    i++;
    goto t1;
t3: return 0;
}