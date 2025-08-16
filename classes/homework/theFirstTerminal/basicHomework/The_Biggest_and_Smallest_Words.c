// 最大最小单词简略版（The Biggest and Smallest Words）
// 编写程序找出一组单词中“最小”单词和“最大”单词。
// 用户输入单词后，程序根据字典顺序决定排在最前面和最后面的单词。
// 当用户输入4个字母的单词时，程序停止读入。假设所有单词都不超过20个字母。
// 输出范例1：
// Enter word: dog
// Enter word: zebra
// Enter word: rabbit
// Enter word: catfish
// Enter word: walrus
// Enter word: cat
// Enter word: fish

// Smallest word: cat
// Largest word zebra
// 输出范例2：
// Enter word: computer
// Enter word: ink
// Enter word: light
// Enter word: bookrack
// Enter word: book

// Smallest word: book
// Largest word: light
#include<stdio.h>
#include<string.h>
int main(){
    char word[100][21],temp[100]={"\0"};
    int i,j,k;
    for(i=0;j!=4;i++){
        printf("Enter word: ");
        scanf("%s",&word[i]);
        j=strlen(word[i]);
    }
    for(j=0;j<i;j++){
        for(k=0;k<i-j;k++){
            if(strcmp(word[k],word[k+1])>0){
                strcpy(temp,word[k]);
                strcpy(word[k],word[k+1]);
                strcpy(word[k+1],temp);
            }
        }//冒泡排序
    }
    printf("Smallest word: %s\n",word[1]);
    printf("Largest word: %s\n",word[i]);
    return 0;
}