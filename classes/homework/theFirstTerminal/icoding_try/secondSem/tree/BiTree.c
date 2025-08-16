#include <stdlib.h>
#include <stdio.h>
#include "bitree.h" //请不要删除，否则检查不通过


BiTNode* transform(CSNode *root){
    if(root == NULL) return NULL;
    //创建根的二叉树节点
    BiTNode *bRoot = (BiTNode*)malloc(sizeof(BiTNode));
    if(!bRoot) return NULL;
    bRoot->data = root->data;
    bRoot->left = bRoot->right = NULL;

    //队列用于BFS，同时存储普通树节点和二叉树节点的对应关系
    Queue* qTree = create_queue();
    Queue* qBin = create_queue();
    add_queue(qTree, root);
    add_queue(qBin, bRoot);

    while(!is_empty_queue(qTree)){
        CSNode* curCS = (CSNode*)del_queue(qTree);
        BiTNode* curBin = (BiTNode*)del_queue(qBin);

        BiTNode *prevChild = NULL;
        for(int i = 0; i < MAX_CHILDREN_NUM; ++i){
            CSNode* csChild = curCS->children[i];
            if(csChild == NULL) break;
            //创建二叉树节点
            BiTNode* binChild = (BiTNode*)malloc(sizeof(BiTNode));
            binChild->data = csChild->data;
            binChild->left = binChild->right = NULL;

            //第一个孩子作为左孩子，其余作为右兄弟
            if(prevChild == NULL){
                curBin->left = binChild;
            } else{
                prevChild->right = binChild;
            }
            prevChild = binChild;
            //入队
            add_queue(qTree, csChild);
            add_queue(qBin, binChild);
        }
    }
    free_queue(qTree);
    free_queue(qBin);
    return bRoot;
}

BiTNode * nearest_ancestor(BiTree root, BiTNode *p, BiTNode *q){
    if(root == NULL) return NULL;
    Stack sp, sq;
    init_stack(&sq);
    init_stack(&sp);

    if(!path(root,p,&sp) || !path(root,q,&sq)) return NULL;

    int lenp = sp.top + 1;
    int lenq = sq.top + 1;
    BiTNode *arrp[Stack_Size], *arrq[Stack_Size];
    for(int i = 0; i < lenp; i++){
        arrp[i] = sp.elem[i];
    }
    for(int i = 0; i < lenq; i++){
        arrq[i] = sq.elem[i];
    }

    //找到最后一个相同的结点
/*这两条路径从根开始，前面一段是重合的，到了某一个节点（比如B）之后，
两条路径才各自走向不同的子树。这个分叉点B，就是最近的共同祖先*/
    int i = 0, minlen = lenp < lenq ? lenp : lenq;
    BiTNode *ancestor = NULL;
    while(i < minlen && arrp[i] == arrq[i]){
        ancestor = arrp[i];
        ++i;
    }
    return ancestor;
}

bool path(BiTNode* root, BiTNode* node, Stack* s){
    if(!root || !node) return false;
    //访问当前节点，先入栈
    push(s, root);
    if(root == node){
        return true;
    }
    //尝试左子树
    if(path(root->left,node,s)){
        return true;
    }
    //尝试右子树
    if(path(root->right,node,s)){
        return true;
    }
    //左右都找不到，回溯
    BiTNode *tmp;
    pop(s,&tmp);
    return false;
}

void pre_order(BiTree root){
    Stack s;
    init_stack(&s);

    BiTNode *p = root;
    while(p || !is_empty(&s)){
        while(p){
            visit_node(p);
            push(&s,p);
            p = p->left;
        }
        if(!is_empty(&s)){
            pop(&s,&p);
            p = p->right;
        }
    }
}
/*相当于内层while循环式一直向左，
而那个if加上外层while是为了在向左到头时往上走一层然再向右走一个再一直向左*/