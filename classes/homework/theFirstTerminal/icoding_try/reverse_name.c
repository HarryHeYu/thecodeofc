#include<stdio.h>
#include<ctype.h>
#include<string.h>
void reverse_name(char * name){
    char first_name[1024];//名
    char last_name[1024];//姓
    int i =0, j = 0, k = 0;
    while(isspace(name[i]))i++;
    while(isalpha(name[i]));
    first_name[0] = name[i];
    first_name[1] = '\0';
    i++;
    while(isspace(name[i]))i++;
    while (isalpha(name[i])) {
        last_name[j++] = name[i++];
    }
    last_name[j] = '\0';
    strcpy(name, last_name);
    strcat(name, ",");
    strcat(name, first_name);
    strcat(name, ".");
    int len = strlen(name);
    while(len > 0 && isspace(name[len-1])){
        name[len-1] = '\0';
        len--;
    }
}
void reverswName(char *name){
    char temp[100], first[50], last[50];
    int i = 0, j = 0;
    while(name[i] == ' ')i++;
    while(name[i]){
        while(name[i] && name[i] != ' ')temp[j++] = name[i++];
        while(name[i] == ' ')i++;
        if(name[i])temp[j++] = ' ';
    }
    temp[j] = '\0';
    sscanf(temp, "%s %s",first, last);
    sprintf(name, "%s, %c.",last, first[0]);
}
int read_line(char str[],int n){
    int i=0;
    char ch;
    while((ch=getchar())!='\n'){
        if(i<n)str[i++]=ch;
    }
    str[i]='\0';
    return i;
}
int main(){
    char name[100];
    read_line(name,sizeof(name));
    reverse_name(name);
    printf("%s",name);
    return 0;
}