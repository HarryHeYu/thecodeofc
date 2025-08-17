/************************
 * <name:顺序表
 * <author:He YU Hao
 * <date: 2025.3.2
 * ***********************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// 使用 #define 定义宏常量
#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10
// 定义状态类型
typedef enum { ERROR = 0, OK = 1 } Status;
// 定义节点结构体
typedef struct {
    char name[LIST_INIT_SIZE]; // 注意如果应用指针的形式
    // 在初始化每个结点时一定要先为结点中的每个变量开辟内存空间
    char sex;
    char addr[LIST_INIT_SIZE];
    unsigned int age;
    char phonenum[20];
    int num; // 假设添加一个整数字段用于排序和比较
} node; // 结点描述
typedef struct {
    node *p;
    int length; // 当前顺序表长度
    int listsize; // 当前分配的线性表长度
} list;
list L;
// 初始化线性表
Status initList(list *l) {
    l->p = (node*)malloc(LIST_INIT_SIZE * sizeof(node));
    if (!(l->p)) exit(1);
    l->length = 0;
    l->listsize = LIST_INIT_SIZE;
    return OK;
}
// 销毁线性表
void destroyList(list* l) {
    if (l->p != NULL) {
        free(l->p);
        l->p = NULL;
        printf("销毁成功！\n");//不需要free(l),因为他不是动态分配的
    } else {
        printf("线性表不存在！\n");
    }
}
// 清空线性表
Status clearList(list* l) {
    if (l->p == NULL) {
        printf("线性表不存在\n");
        return ERROR;
    } else {
        free(l->p);
        l->p = (node*)malloc(l->listsize * sizeof(node));
        if (!(l->p)) exit(1);
        l->length = 0;
        l->listsize = LIST_INIT_SIZE;
    }
    return OK;
}
// 判断线性表是否为空
Status isListEmpty(list* l) {
    return l->p == NULL || l->length == 0;
}
// 获取顺序表长度
int getListLength(list* l) {
    if (l->p == NULL) {
        printf("线性表不存在\n");
        return ERROR;
    }
    return l->length;
}
// 获取第i个元素
Status getElem(list* l, int i, node* e) {
    if (l->p == NULL || i < 0 || i >= l->length) {
        return ERROR;
    } else {
        *e = l->p[i];
        return OK;
    }
}
// 获取第i个前驱元素
Status priorElem(list *l, int i, node* preE) {
    if (i <= 0 || l->p == NULL || i >= l->length) {//这里<=0是因为要前一个存在，而如果i=0则前一个不存在
        return ERROR;
    } else {
        *preE = l->p[i - 1];
        return OK;
    }
}
// 获取第i个后继元素
Status nextElem(list* l, int i, node* nextE) {
    if (i >= l->length - 1 || l->p == NULL || i < 0) {
        return ERROR;
    } else {
        *nextE = l->p[i + 1];
        return OK;
    }
}
// 插入元素到第i个位置(原本的往后移)
Status insertList(list* l, int i, node* e) {
    if (i < 1 || i > l->length + 1) {
/*在这个线性表中，插入元素的位置是从 1 开始计数的，而不是从 0 开始（与数组索引不同）。也就是说，插入位置 i = 1 表示在第一个元素之前插入，i = 2 表示在第二个元素之前插入，以此类推。
当 i 等于 l->length + 1 时，意味着要在当前线性表的末尾插入一个新元素。此时，新元素会成为线性表的最后一个元素。
*/
        return ERROR;
    }
    if (l->length >= l->listsize) {
        l->p = (node*)realloc(l->p, (l->listsize + LISTINCREMENT) * sizeof(node));
        if (!l->p) exit(1);
/*exit 是 C 标准库中的一个函数，其原型定义在 <stdlib.h> 头文件中。exit(1) 表示终止整个程序的执行，并向操作系统返回一个退出状态码 1，通常非零的退出状态码表示程序是因为错误而终止的。
在 insertList 函数里，如果 realloc 调用失败（即 l->p 为 NULL），执行 exit(1) 会导致整个程序立即终止，后续的代码都不会再执行。这种处理方式比较 “强硬”，适用于那些一旦出现内存分配失败，程序就无法继续正常运行的情况。*/
        l->listsize += LISTINCREMENT;
    }
    for (int j = l->length; j >= i; j--) l->p[j] = l->p[j - 1];
    l->p[i - 1] = *e;
    l->length++;
    return OK;
}
// 删除第i个元素并返回其值
Status deleteList(list* l, int i, node* e) {
    if (i < 1 || i > l->length) return ERROR;
    *e = l->p[i - 1];
    for (int j = i - 1; j < l->length - 1; j++) {
        l->p[j] = l->p[j + 1];
    }
    l->length--;
    return OK;
}
// 归并两个按非递减排列的线性表
void mergeList(list* la, list* lb, list* lc) {
    int la_len, lb_len, i = 1, j = 1, k = 0;
    node ai, bj;
    la_len = la->length;
    lb_len = lb->length;
    while (i <= la_len && j <= lb_len) {
        getElem(la, i - 1, &ai);
        getElem(lb, j - 1, &bj);
        if (ai.num <= bj.num) {
            insertList(lc, ++k, &ai);
            i++;
        } else {
            insertList(lc, ++k, &bj);
            j++;
        }
    }
    while (i <= la_len) {
        getElem(la, i - 1, &ai);
        insertList(lc, ++k, &ai);
        i++;
    }
    while (j <= lb_len) {
        getElem(lb, j - 1, &bj);
        insertList(lc, ++k, &bj);
        j++;
    }
}
// 按节点中某一元素的比较升序排列线性表中的节点
Status listAscendingOrder(list* l) {
    node e;
    if (l->p == NULL || l->length <= 1) return ERROR;
    for (int i = 0; i < l->length - 1; i++) {
        for (int j = i + 1; j < l->length; j++) {
            if (l->p[i].num > l->p[j].num) {
                e = l->p[i];
                l->p[i] = l->p[j];
                l->p[j] = e;
            }
        }
    }
    return OK;
}
// 将两线性表升序排列后再归并
void mergeListAscending(list* la, list* lb, list* lc) {
    listAscendingOrder(la);
    listAscendingOrder(lb);
    printf("表a升序后排列为：\n");
    for (int i = 0; i < la->length; i++) printf("%d ", la->p[i].num);
    printf("\n");
    printf("表b升序后排列为：\n");
    for (int i = 0; i < lb->length; i++) printf("%d ", lb->p[i].num);
    printf("\n");
    mergeList(la, lb, lc);
    printf("按升序排列再归并两表为：\n");
    for (int i = 0; i < lc->length; i++) printf("%d ", lc->p[i].num);
    printf("\n");
}
// 遍历并操作顺序表中的所有元素
Status traverseList(list* l, Status(*visit)(void*)) {
    if (l->p == NULL || l->length == 0) {
        printf("线性表为空或不存在\n");
        return ERROR;
    }
    for (int i = 0; i < l->length; i++) {
        if (visit(&l->p[i]) != OK) {
            return ERROR;
        }
    }
    return OK;
}
// 访问函数，用于打印节点信息
Status visit(void* e) {
    node* p = (node*)e;
    printf("name=%s, sex=%c, addr=%s, age=%u, phonenum=%s, num=%d\n", 
           p->name, p->sex, p->addr, p->age, p->phonenum, p->num);
    return OK;
}
// 查找元素，返回元素在顺序表中的位置，未找到返回ERROR
Status findElem(list* l, int num) {
    for (int i = 0; i < l->length; i++) {
        if (l->p[i].num == num) {
            return i;
        }
    }
    return ERROR;
}
// 测试函数（主函数）
int main() {
    list la, lb, lc;
    initList(&la);
    initList(&lb);
    initList(&lc);

    node a1 = {"Alice", 'F', "Address1", 30, "1234567890", 1};
    node a2 = {"Bob", 'M', "Address2", 25, "2345678901", 3};
    node a3 = {"Charlie", 'M', "Address3", 35, "3456789012", 5};

    node b1 = {"David", 'M', "Address4", 40, "4567890123", 2};
    node b2 = {"Eve", 'F', "Address5", 28, "5678901234", 4};
    node b3 = {"Frank", 'M', "Address6", 22, "6789012345", 6};

    insertList(&la, 1, &a1);
    insertList(&la, 2, &a2);
    insertList(&la, 3, &a3);

    insertList(&lb, 1, &b1);
    insertList(&lb, 2, &b2);
    insertList(&lb, 3, &b3);

    printf("表a中的元素：\n");
    traverseList(&la, visit);
    printf("\n");

    printf("表b中的元素：\n");
    traverseList(&lb, visit);
    printf("\n");

    mergeListAscending(&la, &lb, &lc);

    printf("合并后的表c中的元素：\n");
    traverseList(&lc, visit);

    destroyList(&la);
    destroyList(&lb);
    destroyList(&lc);
    return 0;
}