#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0

typedef int Status;

typedef struct {
    char *ch; // Pointer to store the string
    int Length; // Length of the string
} String;

Status StrAssign(String *T, char *chars) {
    int i, len = strlen(chars);
    T->ch = (char*) malloc(len * sizeof(char));
    if(!T->ch) return ERROR;
    for(i = 0; i < len; i++) {
        T->ch[i] = chars[i];
    }
    T->Length = len;
    return OK;
}

Status StrCopy(String *T, String S) {
    int i;
    T->ch = (char*) malloc(S.Length * sizeof(char));
    if(!T->ch) return ERROR;
    for(i = 0; i < S.Length; i++) {
        T->ch[i] = S.ch[i];
    }
    T->Length = S.Length;
    return OK;
}

Status StrEmpty(String S) {
    return S.Length == 0 ? TRUE : FALSE;
}

int StrCompare(String S, String T) {
    int i;
    for(i = 0; i < S.Length && i < T.Length; i++) {
        if(S.ch[i] != T.ch[i]) {
            return S.ch[i] - T.ch[i];
        }
    }
    return S.Length - T.Length;
}

int StrLength(String S) {
    return S.Length;
}

Status ClearString(String *S) {
    if(S->ch) {
        free(S->ch);
        S->ch = NULL;
    }
    S->Length = 0;
    return OK;
}

Status Concat(String *T, String S1, String S2) {
    int i;
    T->ch = (char*) malloc((S1.Length + S2.Length) * sizeof(char));
    if(!T->ch) return ERROR;
    for(i = 0; i < S1.Length; i++) {
        T->ch[i] = S1.ch[i];
    }
    for(i = 0; i < S2.Length; i++) {
        T->ch[S1.Length + i] = S2.ch[i];
    }
    T->Length = S1.Length + S2.Length;
    return OK;
}

Status SubString(String *Sub, String S, int pos, int len) {
    int i;
    if(pos <= 0 || pos > S.Length || len < 0 || len > S.Length - pos + 1) {
        return ERROR;
    }
    Sub->ch = (char*) malloc(len * sizeof(char));
    if(!Sub->ch) return ERROR;
    for(i = 0; i < len; i++) {
        Sub->ch[i] = S.ch[pos + i - 1];
    }
    Sub->Length = len;
    return OK;
}

Status StrInsert(String *S, int pos, String T) {
    int i;
    if(pos < 1 || pos > S->Length + 1) {
        return ERROR;
    }
    S->ch = (char*) realloc(S->ch, (S->Length + T.Length) * sizeof(char));
    if(!S->ch) return ERROR;
    for(i = S->Length; i >= pos; i--) {
        S->ch[i + T.Length - 1] = S->ch[i - 1];
    }
    for(i = pos; i < pos + T.Length; i++) {
        S->ch[i - 1] = T.ch[i - pos];
    }
    S->Length = S->Length + T.Length;
    return OK;
}

Status StrDelete(String *S, int pos, int len) {
    int i;
    if(pos < 1 || pos > S->Length - len + 1 || len < 0) {
        return ERROR;
    }
    for(i = pos + len; i <= S->Length; i++) {
        S->ch[i - len - 1] = S->ch[i - 1];
    }
    S->ch = (char*) realloc(S->ch, (S->Length - len) * sizeof(char));
    S->Length -= len;
    return OK;
}

void StrPrint(String T) {
    int i;
    for(i = 0; i < T.Length; i++) {
        printf("%c", T.ch[i]);
    }
    printf("\n");
}

void Replace(String *S, String T, String V) {
    int i = 0; 
    if(StrEmpty(T)) {
        return;
    }
    do {
        i = Index(*S, T, i);
        if(i) {
            StrDelete(S, i, StrLength(T));
            StrInsert(S, i, V);
            i += StrLength(V);
        }
    } while (i);
}

int Index(String S, String T, int pos) {
    int i = pos;
    int j = 1;
    while(i <= S.Length && j <= T.Length) {
        if(S.ch[i - 1] == T.ch[j - 1]) {
            ++i;
            ++j;
        } else {
            i = i - j + 2;
            j = 1;
        }
    }
    if(j > T.Length) {
        return i - T.Length;
    } else {
        return 0;
    }
}

int main() {
    String s1, s2, t;
    int i;
    char s;

    StrAssign(&s1, "abcde");
    printf("String s1: ");
    StrPrint(s1);
    printf("Is s1 empty: %d (0: No, 1: Yes)\n", StrEmpty(s1));
    printf("Length of s1: %d\n", StrLength(s1));

    StrCopy(&t, s1);
    printf("String t after copying s1: ");
    StrPrint(t);

    StrAssign(&s2, "fghij");
    printf("String s2: ");
    StrPrint(s2);

    i = StrCompare(s1, s2);
    if(i < 0) {
        s = '<';
    } else if(i == 0) {
        s = '=';
    } else {
        s = '>';
    }
    printf("s1 %c s2\n", s);

    ClearString(&t);
    printf("Is t empty after clearing: %d (0: No, 1: Yes)\n", StrEmpty(t));
    printf("Length of t after clearing: %d\n", StrLength(t));

    SubString(&t, s1, 2, 3);
    printf("Substring t of s1 from position 2 with length 3: ");
    StrPrint(t);

    Concat(&t, s1, s2);
    printf("String t after concatenating s1 and s2: ");
    StrPrint(t);

    StrInsert(&s1, 2, s2);
    printf("String s1 after inserting s2 at position 2: ");
    StrPrint(s1);

    StrDelete(&s1, 2, 5);
    printf("String s1 after deleting 5 characters from position 2: ");
    StrPrint(s1);

    StrAssign(&t, "abcdefgh");
    StrAssign(&s1, "bcd");
    StrAssign(&s2, "xyz");
    Replace(&t, s1, s2);
    printf("String t after replacing 'bcd' with 'xyz': ");
    StrPrint(t);

    ClearString(&s1);
    ClearString(&s2);
    ClearString(&t);

    return 0;
}