#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define BLOCK_SIZE 4 // Block size for the chain

typedef int Status;

typedef struct BlockNode {
    char ch[BLOCK_SIZE];
    struct BlockNode *next;
} BlockNode;

typedef struct {
    BlockNode *head, *tail;
    int Length;
} String;

Status StrAssign(String *T, char *chars) {
    int i, len = strlen(chars);
    BlockNode *p, *q;
    p = q = NULL;
    T->head = T->tail = NULL;

    for(i = 0; i < len; i++) {
        if(i % BLOCK_SIZE == 0) {
            q = (BlockNode*) malloc(sizeof(BlockNode));
            if(!q) return ERROR;
            q->next = NULL;
            if(!T->head) {
                T->head = q;
            } else {
                p->next = q;
            }
            p = q;
        }
        q->ch[i % BLOCK_SIZE] = chars[i];
    }
    T->tail = q;
    T->Length = len;
    return OK;
}

Status StrCopy(String *T, String S) {
    BlockNode *p, *q, *newNode;
    p = S.head;
    q = NULL;
    T->head = T->tail = NULL;

    while(p) {
        newNode = (BlockNode*) malloc(sizeof(BlockNode));
        if(!newNode) return ERROR;
        for(int i = 0; i < BLOCK_SIZE; i++) {
            newNode->ch[i] = p->ch[i];
        }
        newNode->next = NULL;
        if(!T->head) {
            T->head = newNode;
        } else {
            q->next = newNode;
        }
        q = newNode;
        p = p->next;
    }
    T->tail = q;
    T->Length = S.Length;
    return OK;
}

Status StrEmpty(String S) {
    return S.Length == 0 ? TRUE : FALSE;
}

int StrCompare(String S, String T) {
    BlockNode *p = S.head, *q = T.head;
    int i = 0, j = 0;

    while(p && q) {
        if(p->ch[i] != q->ch[j]) {
            return p->ch[i] - q->ch[j];
        }
        i++;
        j++;
        if(i == BLOCK_SIZE) {
            p = p->next;
            i = 0;
        }
        if(j == BLOCK_SIZE) {
            q = q->next;
            j = 0;
        }
    }
    return S.Length - T.Length;
}

int StrLength(String S) {
    return S.Length;
}

Status ClearString(String *S) {
    BlockNode *p, *q;
    p = S->head;
    while(p) {
        q = p->next;
        free(p);
        p = q;
    }
    S->head = S->tail = NULL;
    S->Length = 0;
    return OK;
}

Status Concat(String *T, String S1, String S2) {
    BlockNode *p, *q, *newNode;
    p = S1.head;
    q = NULL;
    T->head = T->tail = NULL;

    while(p) {
        newNode = (BlockNode*) malloc(sizeof(BlockNode));
        if(!newNode) return ERROR;
        for(int i = 0; i < BLOCK_SIZE; i++) {
            newNode->ch[i] = p->ch[i];
        }
        newNode->next = NULL;
        if(!T->head) {
            T->head = newNode;
        } else {
            q->next = newNode;
        }
        q = newNode;
        p = p->next;
    }

    p = S2.head;
    while(p) {
        newNode = (BlockNode*) malloc(sizeof(BlockNode));
        if(!newNode) return ERROR;
        for(int i = 0; i < BLOCK_SIZE; i++) {
            newNode->ch[i] = p->ch[i];
        }
        newNode->next = NULL;
        if(!T->head) {
            T->head = newNode;
        } else {
            q->next = newNode;
        }
        q = newNode;
        p = p->next;
    }
    T->tail = q;
    T->Length = S1.Length + S2.Length;
    return OK;
}

Status SubString(String *Sub, String S, int pos, int len) {
    int i, j = 0, k = 0;
    BlockNode *p, *q, *newNode;
    p = S.head;
    q = NULL;
    Sub->head = Sub->tail = NULL;

    if(pos <= 0 || pos > S.Length || len < 0 || len > S.Length - pos + 1) {
        return ERROR;
    }

    for(i = 0; i < pos - 1; i++) {
        j++;
        if(j == BLOCK_SIZE) {
            p = p->next;
            j = 0;
        }
    }

    for(i = 0; i < len; i++) {
        if(k % BLOCK_SIZE == 0) {
            newNode = (BlockNode*) malloc(sizeof(BlockNode));
            if(!newNode) return ERROR;
            newNode->next = NULL;
            if(!Sub->head) {
                Sub->head = newNode;
            } else {
                q->next = newNode;
            }
            q = newNode;
        }
        newNode->ch[k % BLOCK_SIZE] = p->ch[j];
        k++;
        j++;
        if(j == BLOCK_SIZE) {
            p = p->next;
            j = 0;
        }
    }
    Sub->tail = q;
    Sub->Length = len;
    return OK;
}

Status StrInsert(String *S, int pos, String T) {
    int i, j = 0, k = 0;
    BlockNode *p, *q, *r, *newNode;
    p = S->head;
    q = NULL;

    if(pos < 1 || pos > S->Length + 1) {
        return ERROR;
    }

    for(i = 0; i < pos - 1; i++) {
        j++;
        if(j == BLOCK_SIZE) {
            p = p->next;
            j = 0;
        }
    }

    r = p;
    if(j > 0) {
        for(i = j; i < BLOCK_SIZE; i++) {
            r = r->next;
        }
    }

    for(i = 0; i < T.Length; i++) {
        if(k % BLOCK_SIZE == 0) {
            newNode = (BlockNode*) malloc(sizeof(BlockNode));
            if(!newNode) return ERROR;
            newNode->next = NULL;
            if(!q) {
                q = newNode;
            } else {
                r->next = newNode;
            }
            r = newNode;
        }
        newNode->ch[k % BLOCK_SIZE] = T.ch[i];
        k++;
    }

    newNode->next = p->next;
    p->next = q;

    S->Length += T.Length;
    return OK;
}

Status StrDelete(String *S, int pos, int len) {
    int i, j = 0, k = 0;
    BlockNode *p, *q, *r;
    p = S->head;
    q = NULL;

    if(pos < 1 || pos > S->Length - len + 1 || len < 0) {
        return ERROR;
    }

    for(i = 0; i < pos - 1; i++) {
        j++;
        if(j == BLOCK_SIZE) {
            p = p->next;
            j = 0;
        }
    }

    r = p;
    for(i = 0; i < len; i++) {
        k++;
        if(k % BLOCK_SIZE == 0) {
            q = r->next;
            r->next = q->next;
            free(q);
        }
    }

    S->Length -= len;
    return OK;
}

void StrPrint(String T) {
    BlockNode *p = T.head;
    int i = 0;
    while(p) {
        for(i = 0; i < BLOCK_SIZE; i++) {
            printf("%c", p->ch[i]);
        }
        p = p->next;
    }
    printf("\n");
}

void Replace(String *S, String T, String V) {
    int i = 0;
    String Sub;
    if(StrEmpty(T)) {
        return;
    }
    do {
        SubString(&Sub, *S, i, StrLength(T));
        if(StrCompare(Sub, T) == 0) {
            StrDelete(S, i + 1, StrLength(T));
            StrInsert(S, i + 1, V);
            i += StrLength(V);
        } else {
            i++;
        }
    } while (i <= S->Length - StrLength(T));
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

    return 0;
}