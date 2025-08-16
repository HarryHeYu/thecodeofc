#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
typedef struct node
{
    int val;
    struct node *left;
    struct node *right;
    struct node *parent;
    int height;
} node_t;