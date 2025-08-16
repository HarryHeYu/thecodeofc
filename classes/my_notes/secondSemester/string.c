/* Search String */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int search(char* s, char* p);
/* s: main string.
 * p: pattern string.
 * return:
 *     the position of p in s, where p appears for the first time,
 *     or -1 if p doesn't appear.
 */

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

int search(char* s, char* p){
    int i=0,j=0;
    int n = strlen(s);
    int m = strlen(p);
    int next[m];
    calculateNext(p, next);
    while(i < n && j < m){
        if( j == -1 || s[i] == p[j]){
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
    /* ASSIGNMENT TO DO:
     * Search s for p.
     */
}

int main() {
    char s[] = "abcdabacdababacabdef";
    char p[] = "ababaca";

    printf("Main string: %s\n", s);
    printf("Pattern string: %s\n", p);

    int i = search(s, p);
    if (i != -1) {
        printf("The pattern string appears at index %d\n", i);
    } else {
        printf("The pattern string does not appear in the main string.\n");
    }

    return 0;
}
