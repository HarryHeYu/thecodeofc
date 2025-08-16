/* String */
#include<stdio.h>
#include<string.h>

#define N 128

int LengthOfString(const char *s);
// This function returns the length of s.
// strlen

int CompareString(const char* s1,const char* s2);
// strcmp

int CopyString(const char* s1,char* s2,int n);
// strcpy

int LookForString(char* strArray[],int n,const char* s);
/* This function finds a string in an array.
 * Input: strArray, the base address of the string array.
 *        n, the number of strings in the strArray.
 *        s, the object string.
 * Return: the position of the object string if there is,
 *         otherwise -1. 
 * Note: #include<string.h>
 */

int LookForString(char* strArray[],int n,const char* s){
    int i;
    for(i=0;i<n;i++){
        if(strcmp(strArray[i],s)==0){
            return i;
        }
    }
    return -1;
}


int LengthOfString(const char *s){
    int i=0;
    while(s[i]){
       i++;
    }
    return i;
}

int CompareString(const char* s1,const char* s2){
    int i=0;
    while(s1[i]!='\0' && s1[i]==s2[i]){
        i++;
    }
    if(s1[i]>s2[i]){
        return 1;
    }
    else if(s1[i]<s2[i]){
        return -1;
    }
    else{
        return 0;
    }
}

int CopyString(const char* s1,char* s2,int n){
    int i=0;
    while(s1[i] && i<n){
        s2[i]=s1[i];
        i++;
    }
    s2[i]='\0';
    return i;
}

int main(int argc,char* argv[]){
    int i,position; 
    char s1[]="Tom";
    char s2[N];
    printf("The length of %s is %d.\n\n",s1,LengthOfString(s1));
    printf("argc=%d.\n",argc);
    printf("The command line:\n");
    for(i=0;i<argc;i++){
        printf("argv[%d]=%s\t",i,argv[i]);
    }
    puts("\n");    
    printf("Is there any %s in the command line?\n",s1);
    position=LookForString(argv,argc,s1);
    if(position<0){
        printf("No %s in the command line.\n",s1);
    }
    else{
        printf("argv[%d]=%s.\n",position,argv[position]);
    }
    return 0;
}

