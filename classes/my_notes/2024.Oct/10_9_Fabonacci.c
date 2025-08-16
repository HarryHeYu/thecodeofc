#include<stdio.h>
#include<math.h>


//斐波那契数列的函数实现
long Fabonacci(int i){
    if(i==1 || i==2){
        return 1;
       }
    else{
        return Fabonacci(i-2) + Fabonacci(i-1); 
    }
}    
