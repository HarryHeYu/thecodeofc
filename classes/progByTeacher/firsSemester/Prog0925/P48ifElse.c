#include<stdio.h>
int main(){
    int iAge;
    printf("How old are you?");
    scanf("%d",&iAge);
    if(iAge==18){
        printf("Ah, we are of the same age!\n");
    }
    if(iAge>=16 && iAge<=20){
        printf("I like your age.\n");
        printf("We can make friends.\n");
    }
    else{
        printf("Thank you.\n");
    }
    return 0;
}