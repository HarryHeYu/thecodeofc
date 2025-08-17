#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void calculateNext(char *p, int next[]);
int KMP(char *s, char *p);

void calculateNext(char *p, int next[]){
    int j = 0, k = -1, m = strlen(p);
    next[0] = -1;
    while(j < m - 1){
        if(k == -1 || p[j] == p[k]){
            j++;
            k++;
            next[j] = k;
        }else{
            k = next[k];
        }
    }
}

int KMP(char *s, char *p){
    int i = 0, j = 0, n = strlen(s), m = strlen(p);
    int next[m];
    calculateNext(p, next);
    while(i < n && j < m){
        if(j == -1 || s[i] == p[j]){
            i++;
            j++;
        }else{
            j = next[j];
        }
    }
    if(j == m){
        return i - j;
    }else{
        return -1;
    }
}

int main() {
    char s[] = "abcdabacdababacabdef";
    char p[] = "ababaca";

    printf("Main string: %s\n", s);
    printf("Pattern string: %s\n", p);

    int i = KMP(s, p);
    if (i != -1) {
        printf("The pattern string appears at index %d\n", i);
    } else {
        printf("The pattern string does not appear in the main string.\n");
    }

    return 0;
}
