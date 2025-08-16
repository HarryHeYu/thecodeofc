#include <stdio.h>
#include <stdlib.h>
#define C24_VERSION 202311L
#if __STDC_VERSION__ < C24_VERSION
    #define nullptr NULL
    typedef enum{false=0,true=1}bool;
#endif // __STDC_VERSION
typedef enum{NO_CHILD = 0, CHILD = 1}ThreadTag;
typedef struct BinaryNode{
    int data;
    struct BinaryNode *pLeft;
    struct BinaryNode *pRight;
    ThreadTag leftTag;
    ThreadTag rightTag;
}BinaryNode, *BinaryTree;

// 创建新节点
BinaryNode* createNode(int data) {
    BinaryNode* node = (BinaryNode*)malloc(sizeof(BinaryNode));
    if (node == nullptr) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    node->data = data;
    node->pLeft = nullptr;
    node->pRight = nullptr;
    node->leftTag = NO_CHILD;
    node->rightTag = NO_CHILD;
    return node;
}

// 创建二叉树（初始化根节点）
BinaryTree createBinaryTree() {
    return createNode(0);  // 根节点作为占位节点
}

// 插入节点到二叉搜索树（BST）
void insertNode(BinaryTree root, int data) {
    if (root == nullptr) {
        printf("Root is null. Cannot insert.\n");
        return;
    }
    BinaryNode* current = root;
    BinaryNode* parent = nullptr;

    while (current != nullptr) {
        parent = current;
        if (data < current->data) {
            if (current->leftTag == CHILD)
                current = current->pLeft;
            else
                break;
        } else if (data > current->data) {
            if (current->rightTag == CHILD)
                current = current->pRight;
            else
                break;
        } else {
            printf("Duplicate value: %d cannot be inserted.\n", data);
            return;
        }
    }

    BinaryNode* newNode = createNode(data);
    if (data < parent->data) {
        parent->pLeft = newNode;
        parent->leftTag = CHILD;
    } else {
        parent->pRight = newNode;
        parent->rightTag = CHILD;
    }
}

// 中序遍历普通二叉树
void inOrderTraversal(BinaryTree root) {
    if (root == nullptr)
        return;
    if (root->leftTag == CHILD)
        inOrderTraversal(root->pLeft);
    printf("%d ", root->data);
    if (root->rightTag == CHILD)
        inOrderTraversal(root->pRight);
}
void ThreadBinaryTree(BinaryTree pRoot) {
    static BinaryNode* pPrevious = nullptr; // 静态变量，记录前驱节点，生命周期贯穿函数多次调用
    if (pRoot == nullptr) { // 若当前子树为空，直接返回
        return;
    }
    ThreadBinaryTree(pRoot->pLeft); // 递归线索化左子树，体现中序遍历“左子树 → 根 → 右子树”的顺序
    if (pRoot->leftTag == NO_CHILD) { // 若当前节点左孩子为空（即无实际左子树）
        pRoot->pLeft = pPrevious; // 将左指针指向先驱节点pPrevious，建立前驱线索
    }
    if (pPrevious != nullptr && pPrevious->rightTag == NO_CHILD) { // 若前驱节点存在且其右孩子为空
        pPrevious->pRight = pRoot; // 前驱节点的右指针指向当前节点pRoot，建立后继线索
    }
    pPrevious = pRoot; // 更新pPrevious为当前节点，以便后续节点将其作为前驱
    ThreadBinaryTree(pRoot->pRight); // 递归线索化右子树
}
// 中序遍历线索化二叉树
void threadedInOrderTraversal(BinaryTree root) {
    BinaryNode* current = root;
    while (current != nullptr) {
        // 移动到左子树的最左节点
        while (current->leftTag == CHILD)
            current = current->pLeft;
        // 打印当前节点数据
        printf("%d ", current->data);
        // 使用线索访问后继节点
        while (current->rightTag == NO_CHILD && current->pRight != nullptr) {
            current = current->pRight;
            printf("%d ", current->data);
        }
        // 移动到右子树
        current = current->pRight;
    }
}
//先序遍历线索化二叉树
void threadedPreOrderTraversal(BinaryTree root) {
    BinaryNode* current = root;
    while (current != nullptr) {
        // 访问当前节点
        printf("%d ", current->data);
        // 如果有左孩子，移动到左孩子
        if (current->leftTag == CHILD) {
            current = current->pLeft;
        } else {
            // 否则，利用线索移动到右孩子或后继节点
            current = current->pRight;
        }
    }
}
//后序遍历线索化二叉树
void threadedPostOrderTraversal(BinaryTree root) {
    BinaryNode* current = root;
    BinaryNode* prev = nullptr;
    // 找到第一个后序节点（最左下的叶子节点）
    while (current != nullptr && (current->leftTag == CHILD || current->rightTag == CHILD)) {
        if (current->leftTag == CHILD) {
            current = current->pLeft;
        } else {
            current = current->pRight;
        }
    }
    // 遍历后序节点
    while (current != nullptr) {
        printf("%d ", current->data); // 访问当前节点
        // 如果当前节点是其父节点的右孩子或没有右孩子，移动到父节点
        if (current->pRight == prev || current->rightTag == NO_CHILD) {
            prev = current;
            current = current->pRight;
        } else {
            // 否则，移动到右孩子的最左下节点
            current = current->pRight;
            while (current != nullptr && (current->leftTag == CHILD || current->rightTag == CHILD)) {
                if (current->leftTag == CHILD) {
                    current = current->pLeft;
                } else {
                    current = current->pRight;
                }
            }
        }
    }
}