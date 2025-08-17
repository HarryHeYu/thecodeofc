#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long long uint64_t;

typedef struct {
    uint64_t *data;
    int top;
    int capacity;
} Stack;

void init_stack(Stack *s, int capacity) {
    s->data = malloc(capacity * sizeof(uint64_t));
    s->top = -1;
    s->capacity = capacity;
}

void push(Stack *s, uint64_t x) {
    if (s->top + 1 >= s->capacity) {
        s->capacity *= 2;
        s->data = realloc(s->data, s->capacity * sizeof(uint64_t));
    }
    s->data[++s->top] = x;
}

void pop(Stack *s) {
    if (s->top == -1) {
        printf("Empty\n");
    } else {
        s->top--;
    }
}

void query(Stack *s) {
    if (s->top == -1) {
        printf("Anguei!\n");
    } else {
        printf("%llu\n", s->data[s->top]);
    }
}

void size(Stack *s) {
    printf("%d\n", s->top + 1);
}

int main() {
    int T;
    scanf("%d", &T);
    Stack s;
    init_stack(&s, 1000000); // 初始容量1e6
    for (int t = 0; t < T; t++) {
        int n;
        scanf("%d", &n);
        s.top = -1; // 清空栈
        for (int i = 0; i < n; i++) {
            char op[10];
            scanf("%s", op);
            if (strcmp(op, "push") == 0) {
                uint64_t x;
                scanf("%llu", &x);
                push(&s, x);
            } else if (strcmp(op, "pop") == 0) {
                pop(&s);
            } else if (strcmp(op, "query") == 0) {
                query(&s);
            } else if (strcmp(op, "size") == 0) {
                size(&s);
            }
        }
    }
    free(s.data);
    return 0;
}