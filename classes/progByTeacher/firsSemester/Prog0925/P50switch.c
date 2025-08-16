#include<stdio.h>
int main(){
    int iChoice;
    printf("1.Red   2.Green   3.Blue \n");
    printf("What is your favorite color?\n");
    scanf("%d",&iChoice);
    switch(iChoice){
        case 1:
        case 2:
            printf("It's not my favorite color.\n");
            break;
        case 3:
            printf("It is my favorite color too.\n");
            break;
        default:
            printf("Error in selection.\n");
    }
    return 0;
}