// 逆序（Reversal）
// 编写程序读一条消息，然后逆序打印出这条消息。
// 输出范例：
// Enter a message: Don't get mad, get even.
// Reversal is: .neve teg ,dam teg t'noD
// Enter a message: Hello, world!
// Reversal is: !dlrow ,olleH
// 提示：一次读取消息中的一个字符（用getchar函数），并且把这些字符存储在数组中，当数组写满或者读到字符 '\n' 时停止读入。
#include<stdio.h>
#include<string.h>
#define size 200
int main(){
    char message[size]={0};
    int i,sizes=0;
    printf("Enter a message: ");
    for(i=0,sizes=0;i<size;i++){
        message[i]=getchar();
        if(message[i]=='\n'){
            break;
        }
        sizes++;
    }
    message[sizes]='\0';

    char reversal[size]={0};
    for(i=0;i<sizes;i++){
        reversal[i]=message[sizes-i-1];
    }
     reversal[i]='\0';
    printf("Reversal is: %s",reversal);
    return 0;
}