#include <stdio.h>
#include <stdlib.h>
#include "minbinheap.h"

//最小堆的初始化
void init_min_heap(PMinHeap pq, int capacity){
    if(!pq || capacity <= 0) return;
    pq->heap_array = (PMinHeapNode)malloc(sizeof(MinHeapNode) * capacity);
    if(!pq->heap_array){
        pq->heap_size = 0;
        pq->capacity = 0;
        return ;
    }
    pq->heap_size = 0;
    pq->capacity = capacity;
}

//最小堆元素插入
bool heap_insert_value(PMinHeap pq, int value){
    if(!pq || pq->heap_size >= pq->capacity) return false;
    //插入到数组末尾
    int idx = pq->heap_size;
    pq->heap_array[idx].value = value;
    //otherInfo可不必初始化，也可置零
    pq->heap_array[idx].otherInfo.i = 0;
    pq->heap_array[idx].otherInfo.j = 0;
    pq->heap_size++;
    //上浮调整
    while(idx > 0){
        int parent_idx = parent(idx);
        if(pq->heap_array[idx].value < pq->heap_array[parent_idx].value){
            swap_node(&pq->heap_array[idx],&pq->heap_array[parent_idx]);
            idx = parent_idx;
        } else{
            break;
        }
    }
    return true;
}

void min_heapify(PMinHeap pq, int i){
    if(!pq || !pq->heap_array || pq->heap_size <= 0) return ;
    int l = left(i);
    int r = right(i);
    int smallest = i;

    //找到i、左孩子、右孩子中值最小的下标
    if(l < pq->heap_size && pq->heap_array[l].value < pq->heap_array[smallest].value){
        smallest = l;
    }
    if (r < pq->heap_size && pq->heap_array[r].value < pq->heap_array[smallest].value) {
        smallest = r;
    }

    if (smallest != i) {
        swap_node(&pq->heap_array[i], &pq->heap_array[smallest]);
        min_heapify(pq, smallest); // 递归向下堆化
    }
}


void merge_arrays(const int *arr, int n, int k, int* output){
    typedef struct{
        int value;
        int row;
        int idx;
    }HeapNode;
    HeapNode* heap = (HeapNode*)malloc(sizeof(HeapNode)*n);
    //heap是最小堆，用于合并
    int heap_size = n;
    //初始化堆，将每一行的第0个元素存入堆中
    for(int i = 0; i < n; i++){
        heap[i].value = arr[i*k];
        heap[i].row = i;
        heap[i].idx = 0;
    }
    //建堆
    for(int i = heap_size/ 2 - 1; i >= 0; --i){
        int current = i;
        while(1){
            int smallest = current;
            int l = 2*current + 1;
            int r = 2*current + 2;
            if(l < heap_size && heap[l].value < heap[smallest].value)smallest = l;
            if(r < heap_size && heap[r].value < heap[smallest].value)smallest = r;
            if(smallest != current){
                HeapNode tmp = heap[current];
                heap[current] = heap[smallest];
                heap[smallest] = tmp;
                current = smallest;
            } else{
                break;
            }
        }
    }
    //归并输出
    int out_idx = 0;
    while(heap_size > 0){
        HeapNode min = heap[0];
        output[out_idx++] = min.value;
        //如果该行还有下一个元素，替换根节点，否则用最后一个元素替换根节点并减少堆大小
        if(min.idx + 1 < k){
            heap[0].value = arr[min.row * k + min.idx + 1];
            heap[0].row = min.row;
            heap[0].idx = min.idx + 1;
        } else{
            heap[0] = heap[heap_size - 1];
            --heap_size;
            //如果那一行已经没有剩余元素，用堆中最后一个元素覆盖堆顶，并减少堆大小（堆元素数量-1）。
        }
        //堆调整
        int current = 0;
        while (1) {
            int smallest = current;
            int l = 2*current + 1;
            int r = 2*current + 2;
            if (l < heap_size && heap[l].value < heap[smallest].value) smallest = l;
            if (r < heap_size && heap[r].value < heap[smallest].value) smallest = r;
            if (smallest != current) {
                HeapNode tmp = heap[current];
                heap[current] = heap[smallest];
                heap[smallest] = tmp;
                current = smallest;
            } else {
                break;
            }
        }
    }
    free(heap);
}