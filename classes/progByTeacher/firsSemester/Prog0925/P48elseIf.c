#include<stdio.h>
int main(){
    int iScore;
    printf("Please input your score:");
    scanf("%d",&iScore);
    if(iScore>=90 && iScore<=100){
        printf("Excellent.\n");
    }
    else if(iScore>=60 && iScore<90){
        printf("Pass.\n");
    }
    else if(iScore>=0 && iScore<60){
        printf("Fail.\n");
    }
    else{
        printf("Error.\n");
    }
    return 0;
}