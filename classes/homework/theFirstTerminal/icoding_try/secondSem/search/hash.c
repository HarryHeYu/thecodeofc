#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

HASH_RESULT hash_add_int(HashTable * table, const char * key, int value){
    if(!table || !key) return HASH_ERROR;
    long hashcode = hash_string(key);
    int index = hashcode % table->size;

    HashEntry *entry = table->bucket[index];
    while(entry){
        if(entry->key.str_value && strcmp(entry->key.str_value, key) == 0){
            if(entry->value.int_value == value){
                table->last_error = HASH_ALREADY_ADDED;
                return HASH_ALREADY_ADDED;
            } else{
                entry->value.int_value = value;
                table->last_error = HASH_REPLACED_VALUE;
                return HASH_REPLACED_VALUE;
            }
        }
        entry = entry->next;
    }
    //未找到，创建新的节点
    HashEntry *new_entry = (HashEntry*)malloc(sizeof(HashEntry));
    if(!new_entry){
        table->last_error = HASH_ERROR;
        return HASH_ERROR;
    }
    new_entry->key.str_value = strdup(key);
    /*strdup 是 C 语言中的一个字符串处理函数，用于复制字符串并分配内存
    s 是要复制的源字符串（以 \0 结尾的字符数组）*/
    new_entry->value.int_value = value;
    new_entry->next = table->bucket[index];
    table->bucket[index] = new_entry;
    table->last_error = HASH_ADDED;
    return HASH_ADDED;
}

// 创建大小为hash_size的哈希表，创建成功后返回HashTable类型的指针，否则返回NULL。
HashTable *create_hash(int hash_size){
    if(hash_size <= 0) return NULL;
    //分配内存
    HashTable *table = (HashTable*)malloc(sizeof(HashTable));
    if(!table) return NULL;
    table->bucket = (HashEntry**)malloc(sizeof(HashEntry*)*hash_size);
    if(!table->bucket){
        free(table);
        return NULL;
    }
    memset(table->bucket, 0, sizeof(HashEntry*)*hash_size);
    table->size = hash_size;
    table->last_error = HASH_OK;
    return table;
}