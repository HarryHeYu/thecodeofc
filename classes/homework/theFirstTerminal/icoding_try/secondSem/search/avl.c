#include <stdlib.h>
#include <stdio.h>
#include "avl.h"

#define max(a, b) ((a) > (b) ? (a) : (b))

int get_height(node_t* node){
    return (node == NULL) ? 0 : node->height;
}

node_t* rotate_right(node_t* y){
    node_t* x = y->left;
    node_t* T2 = x->right;
    //进行旋转
    x->right = y;
    y->left = T2;

    //更新父节点指针
    x->parent = y->parent;
    y->parent = x;
    if(T2) T2->parent = y;

    //更新高度
    y->height = 1 + max(get_height(y->left), get_height(y->right));
    x->height = 1 + max(get_height(x->left), get_height(x->right));
    return x; //返回新的根节点
}

node_t* rotate_left(node_t* x){
    node_t *y = x->right;
    node_t* T2 = y->left;

    //记进行旋转
    y->left = x;
    x->right = T2;

    //更新父节点指针
    y->parent = x->parent;
    y->parent = x;
    if(T2) T2->parent = x;
    //更新高度
    x->height = 1 + max(get_height(x->left), get_height(x->right));
    y->height = 1 + max(get_height(y->left), get_height(y->right));
    return y; //返回新的根节点
}

node_t* avl_insert(node_t *root, int val){
    if(root == NULL){
        node_t *new_node = (node_t*)malloc(sizeof(node_t));
        if(!new_node){
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        new_node->val = val;
        new_node->left = NULL;
        new_node->right = NULL;
        new_node->parent = NULL;
        new_node->height = 1;
        return new_node;
    }

    //根据插入值的大小递归地插入到左或右子树
    if(val < root->val){
        root->left = avl_insert(root->left, val);
        if(root->left) root->left->parent = root;
    } else if(val > root->val){
        root->right = avl_insert(root->right,val);
    }
    else{
        // 如果值已经存在，直接返回当前节点
        return root;
    }
    // 更新当前节点的高度
    root->height = 1 + max(get_height(root->left),get_height(root->right));
    //计算平衡因子来判断是否需要旋转
    int balance = get_height(root->left) - get_height(root->right);
    //LL情况(插入到左子树的左子树) -> 右旋
    if(balance > 1 && val < root->left->val){
        return rotate_right(root);
    }
    //RR情况(插入到右子树的右子树) -> 左旋
    if(balance < -1 && val > root->right->val){
        return rotate_left(root);
    }
    //LR情况(插入到左子树的右子树) -> 先左旋，再右旋
    if(balance > 1 && val > root->left->val){
        root->left = rotate_left(root->left);
        if(root->left) root->left->parent = root;
        return rotate_right(root);
    }
    //RL情况(插入到右子树的左子树) -> 先右旋，再左旋
    if(balance < -1 && val < root->right->val){
        root->right = rotate_right(root->right);
        if(root->right) root->right->parent = root;
        return rotate_left(root);
    }

    return root;
}