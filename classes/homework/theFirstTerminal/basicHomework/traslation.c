// 翻译（Translation）
// 编写程序可以把字母格式的电话号码翻译成数值格式：
// Enter phone number: CALLATT
// 2255288
// 如果没有电话在身边,参考这里给出的字母在键盘上的对应关系：
// （2=ABC，3=DEF，4=GHI，5=JKL，6=MNO，7=PQRS，8=TUV，9=WXYZ）
// 原始电话号码中的非字母字符（例如数字或标点符号）保持不变：
// Enter phone number: 1-800-COL-LECT1-800-265-5328
// 可以假设任何用户输入的字母都是大写字母。
// 输出范例：
// Enter phone number: 1-DCKS-A2D-OLED
// 1-3257-223-6533
#include<stdio.h>

int main(){
    char c[50];
    printf("Enter phone number:");
    scanf("%s",&c);
    for(int i=0;i<50;i++){
        if(c[i]=='-'){
        }
        else{
            if(c[i]>='A'&&c[i]<='C'){
                printf("2");
            }
            else if(c[i]>='D'&&c[i]<='F'){
                printf("3");
            }
            else if(c[i]>='G'&&c[i]<='I'){
                printf("4");
            }
            else if(c[i]>='J'&&c[i]<='L'){
                printf("5");
            }
            else if(c[i]>='M'&&c[i]<='O'){
                printf("6");
            }
            else if(c[i]>='P'&&c[i]<='S'){
                printf("7");
            }
            else if(c[i]>='T'&&c[i]<='V'){
                printf("8");
            }
            else if(c[i]>='W'&&c[i]<='Z'){
                printf("9");
            }
        }
    }
    printf("%s",c);
    return 0;
}