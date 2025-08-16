#define GOODS_FILE_NAME "goodsinfo.txt"
#define MAX_ID_LEN 30
#define MAX_NAME_LEN 30
#define MAX_PRICE_LEN 30
#define MAX_DISCOUNT_LEN 30
typedef struct {
  char goods_id[MAX_ID_LEN];
  char goods_name[MAX_NAME_LEN];
  int goods_price;
  char goods_discount[MAX_DISCOUNT_LEN];
  int goods_amount;
  int goods_remain;
}GoodsInfo;
typedef struct node{
  GoodsInfo data;
  struct node *next;
}GoodsList;

GoodsInfo read_goods_info();
void init_list(GoodsList **pL);
void destory_list(GoodsList **pL);
void destory_list_and_file(GoodsList **pL);
int save_to_file(GoodsList *L);
void output_one_item(GoodsList *L);
void output_all_items(GoodsList *L);
bool insert_item(GoodsList *L, GoodsInfo item, int choice);
bool delete_item(GoodsList *L, char* goods_id);
GoodsList* search_item(GoodsList *L, char* goods_id);
bool change_item(GoodsList *L, char* goods_id, GoodsInfo new_info);
void bubble_sort(GoodsList *L);
int read_line(char str[], int n);

该部分声明已包含在 “lab52.h”中。

注意：从键盘输入/读取一个字符串时请调用 read_line 函数

#include <stdio.h>
#include <stdlib.h>
#include "lab52.h" // 请不要删除本行头文件，否则检查不通过

extern int CurrentCnt; // 请不要删除本行的全局变量声明，否则检查不通过

int main(){
    GoodsList* goodslist;
    init_list(&goodslist);
    GoodsInfo item;
    char temp_id[MAX_ID_LEN];
    while(1){
        int choice;
        printf("超市管理系统：\n");
        printf("1.显示所有商品的信息 \n");
        printf("2.修改某个商品的信息\n");
        printf("3.插入某个商品的信息\n");
        printf("4.删除某个商品的信息\n");
        printf("5.查找某个商品的信息\n");
        printf("6.商品存盘并退出系统\n");
        printf("7.对商品价格进行排序\n");
        printf("8.(慎用)删除所有内容\n");
        printf("其他.不存盘并退出系统\n");
        printf("请输入你的选择：");
        scanf("%d", &choice);

        switch(choice){
          case 1:
            output_all_items(goodslist);
            break;
          case 2:
            item = read_goods_info();
            printf("请输入要修改的商品id：");
            read_line(temp_id, MAX_ID_LEN);
            change_item(goodslist, temp_id, item);
            break;
          case 3:
            int pos;
            item = read_goods_info();
            printf("请输入要插入的商品的位置：0.尾插 1.头插 i.第i个位置\n");
            scanf("%d", &pos);
            insert_item(goodslist, item, pos);
            break;
          case 4:
            printf("请输入要删除的商品id：");
            read_line(temp_id, MAX_ID_LEN);
            delete_item(goodslist, temp_id);
            break;
          case 5:
            printf("请输入要查找的商品id：");
            read_line(temp_id, MAX_ID_LEN);
            search_item(goodslist, temp_id);
            break;
          case 6:
            save_to_file(goodslist);
            printf("商品信息已存盘\n");
            return 0;
          case 7:
            bubble_sort(goodslist);
            break;
          case 8:
            destory_list_and_file(&goodslist);
            printf("所有内容已删除\n");
            break;
          default:
            printf("程序已退出\n");
            destory_list(&goodslist);
          return 0;
        }
    }
}

void init_list(GoodsList** pL){
    FILE* fp;
    GoodsInfo goodsInfo;
    GoodsList *p, *r;
    (*pL)= (GoodsList*)malloc(sizeof(GoodsList));
    r = (*pL);
    if((fp = fopen(GOODS_FILE_NAME, "r")) == NULL){
      if((fp = fopen(GOODS_FILE_NAME, "w")) == NULL)
        printf("不能打开文件。\n");
    } 
      else{
        while(!feof(fp)){
          fscanf(fp, "%s", goodsInfo.goods_id);
          fscanf(fp, "\t%s", goodsInfo.goods_name);
          fscanf(fp, "\t%d", &goodsInfo.goods_price);
          fscanf(fp, "\t%s", goodsInfo.goods_discount);
          fscanf(fp, "\t%d", &goodsInfo.goods_amount);
          fscanf(fp, "\t%d", &goods Info.goods_remain);
          p = (GoodsList*)malloc(sizeof(GoodsList));
          p->data = goodsInfo;
          r->next = p;
          r = p;
          CurrentCnt++;
        
        }
      }
      fclose(fp);
      r->next = NULL;
      printf("商品的链表已建立，有%d个商品记录\n",CurrentCnt);
}

bool insert_item(GoodsList *L, GoodsInfo goodsInfo, int choice){
    GoodsList *p, *new;
    new = (GoodsList*)malloc(sizeof(GoodsList));
    if(!new){
      return false;
    }
    new->data = goodsInfo;//将传入的商品信息赋值给新节点
    switch(choice){
      case 0:
        p = L;
        while(p->next != NULL){
          p = p->next;
        }
        p->next = new;
        new->next = NULL;
        break;
      case 2:
        new->next = L->next;//L不是实际的节点，而是头节点，头节点没有数据，所以直接指向第一个节点
        L->next = new;//头结点指向新节点
        //也就是说L->next就是第一个有实际数据的节点
        break;
      default:
       int i = 1;
       p = L;
       while(p != NULL && i < choice){//。当 i 等于 choice 时，p 将指向插入位置的前一个节点。
        p = p->next;
        i++;
       }
       if(p == NULL){
        free(new);
        return false;
       }
       new->next = p->next;
       p->next = new;
       break;
    }
    CurrentCnt++;
    return true;
}

bool delete_item(GoodsList *L, char *id){
    GoodsList *p, *q;
    p = L;
    if(L->next == NULL){
      return false;
    }
    if(strcmp(p->next->data.goods_id, id) == 0){//两个字符串在某一点完全相同，此时返回值是 0，表示两个字符串相等。
      GoodsList *temp = p->next;
      p->next = p->next->next;
      free(temp);
      CurrentCnt--;
      return true;
    }
    while(p->next != NULL){
      if(strcmp(p->next->data.goods_id, id) == 0){
        q = p->next;
        p->next = p->next->next;
        free(q);
        CurrentCnt--;
        return true;
      }
      p = p->next;
    }
    return false;
}

GoodsList* search_item(GoodsList *L, char *id){
    GoodsList *p;
    p = L->next;
    while(p != NULL){
      if(strcmp(p->data.goods_id, id) == 0){
       return p;
      }
      p = p->next;
    }
    return NULL;
}

bool change_item(GoodsList *L, char *id, GoodsInfo goodsInfo){
    GoodsList *p;
    p = search_item(L, id);
    if(p == NULL){
      return false;
    }
    p->data = goodsInfo;
    return true;
}

void output_one_item(GoodsList *pL){
  printf("%s\t%s\t%d\t%s\t%d\t%d\n", pL->data.goods_id, 
  pL->data.goods_name, pL->data.goods_price, 
  pL->data.goods_discount, pL->data.goods_amount, 
  pL->data.goods_remain);
}

void output_all_items(GoodsList *pL){
    GoodsList *p;
    p = pL->next;
    while(p != NULL){
      output_one_item(p);
      p = p->next;
    }
}

void destory_list(GoodsList **L){
  GoodsList *p, *q;
  if(L == NULL || *L == NULL){
    return;
  }
  p = *L;
  while(p != NULL){
    q = p;
    p = p->next;
    free(q);
  }
  *L = NULL;
  CurrentCnt = 0;
}

void destory_list_and_file(GoodsList** L)
{
    destory_list(L);
    remove("goodinfo.txt");
}

int save_to_file(GoodsList *L){
  FILE *fp;
  int count = 0;
  fp = fopen("goodinfo.txt", "w");
  if(fp == NULL){
    return 0;
  }
  GoodsList *p;
  p = L->next;
  while(p != NULL){
    fprintf(fp, "%s\t%s\t%d\t%s\t%d\t%d\n", p->data.goods_id, 
    p->data.goods_name, p->data.goods_price, 
    p->data.goods_discount, p->data.goods_amount, 
    p->data.goods_remain);
    p = p->next;
    count++;
  }
  fclose(fp);
  return count;
}

void bubble_sort(GoodsList *L){
  GoodsList *p, *q;
  GoodsInfo temp;
  p = L->next;
  while(p != NULL){
    q = p->next;
    while(q != NULL){
      if(p->data.goods_price > q->data.goods_price){
        temp = p->data;
        p->data = q->data;
        q->data = temp;
      }
      q = q->next;
    }
    p = p->next;
  }
}

GoodsInfo read_goods_info(){
  GoodsInfo goodsInfo;
  printf("请输入商品ID：");
  scanf("%s", goodsInfo.goods_id);
  printf("请输入商品名称：");
  scanf("%s", goodsInfo.goods_name);
  printf("请输入商品价格：");
  scanf("%d", &goodsInfo.goods_price);
  printf("请输入商品折扣：");
  scanf("%s", goodsInfo.goods_discount);
  printf("请输入商品数量：");
  scanf("%d", &goodsInfo.goods_amount);
  printf("请输入商品剩余数量：");
  scanf("%d", &goodsInfo.goods_remain);
  return goodsInfo;
}
/*GoodsInfo read_goods_info()
{
    GoodsInfo goods;
    read_line(goods.goods_id, MAX_ID_LEN);
    read_line(goods.goods_name, MAX_NAME_LEN);
    scanf("%d", &goods.goods_price);
    read_line(goods.goods_discount, MAX_PRICE_LEN);
    scanf("%d", &goods.goods_amount);
    scanf("%d", &goods.goods_remain);
    return goods;
}*/