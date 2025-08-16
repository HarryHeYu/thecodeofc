#include<stdio.h>
#define MY_SECRET_NUMBER 7
int main(){
    int iGuessingNumber;
    printf("There is a secret number in my mind.\n");
    printf("It is between 0 and 9.\n");
    printf("Can you guess what it is?\n");
guessAgain:
    scanf("%d",&iGuessingNumber);
    if(iGuessingNumber!=MY_SECRET_NUMBER){
        printf("No. Please try again.\n");
        goto guessAgain;
    }
    else{
        printf("Yes,it is.\n");
    }
    return 0;
}