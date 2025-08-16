//字符串
/*C语言用\0作为字符串结束标记
\0在ASCII中是0，不同于0
N个字符的字符串占N+1个
专门输入输出：
fgets(str,n,stdin);(可以输入带空格的字符串)
gets_s(str,n)(C11用来代替gets（str）)
puts(str)
const表示字符串无法修改*/
#include <stdio.h>
int LengthOfString(const char *s);
int CompareString(const char* s1, const char* s2);
int CopyString(const char* s1,char* s2);

int CopyString(const char* s1,char* s2,int n){
    
}
int LengthOfString(const char *s){
    int i =0;
    while(s[i]){
        i++;
    }
    return i;
}
int CompareString(const char* s1, const char* s2){
    int i = 0;
    while(s1[i]!='\0' && s1[i]==s2[i]){
        //"\0"表示字符串结束
        //s1[i] != '\0'表示s1[i]不是字符串结束符
        i++;
    }
    if(s1[i]>s2[i]){
        return 1;
    }
    else if (s1[i]<s2[i]){
        return -1;
    }
    else{
        return 0;
    }
}
int main(int argc, char *argv[]){
    //argc是一个整型变量，表示命令行参数的个数。他包括程序本身的名称，所以argc至少为1。
    //argv是一个指向字符指针的指针，它指向命令行参数的字符串。argv[0]是程序本身的名称，argv[1]是第一个命令行参数，以此类推。
    //argv 是一个指针数组，每个元素都指向一个以 null 结尾的字符串（C 语言中的字符串）。这些字符串包含了每个命令行参数的具体值。
    int i;
    char *s1[] = {"apple","banana","orange","grape","watermelon","peach","pear","kiwi","lemon","grapefruit"};
    char *s2[] = {"elephant","lion","monkey","snake","cat","dog","duck","hen","goat","fox"};
    printf("Parameters from the command line:\n");
    printf("argc=%d\n",argc);
    for (i = 0;i < argc;i++){
        printf("argv[%d]=%s\n",i,argv[i]);
    }

    int a,b;
    a = LengthOfString("Hello");
    b = CompareString(s1[0],s2[0]);
    printf("a = %d\n",a);
    printf("b = %d\n",b);
    return 0;
}