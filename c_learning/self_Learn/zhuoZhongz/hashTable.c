#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"


HASH_RESULT hash_add_str(HashTable* table, const char *key, char *value) {
    // 检查参数是否有效
    if (table == NULL || key == NULL || value == NULL || table->bucket == NULL || table->size <= 0) {
        return HASH_ERROR;
    }

    // 计算哈希索引
    unsigned long index = (unsigned long)hash_string(key) % table->size;
    HashEntry *entry = table->bucket[index];

    // 遍历链表，检查是否已存在相同的键
    while (entry) {
        if (strcmp(entry->key.str_value, key) == 0) {
            if (entry->val_tag != HASH_STRING) {
                return HASH_ERROR; // 类型不匹配
            }

            if (strcmp(entry->value.str_value, value) == 0) {
                return HASH_ALREADY_ADDED; // 键值对已存在
            }

            // 替换现有值
            char *new_value = strdup(value);
            if (new_value == NULL) {
                return HASH_ERROR; // 内存分配失败
            }

            free(entry->value.str_value);
            entry->value.str_value = new_value;
            return HASH_REPLACED_VALUE; // 替换成功
        }
        entry = entry->next;
    }

    // 如果键不存在，则创建新条目并添加到链表头部
    HashEntry *new_entry = (HashEntry *)malloc(sizeof(HashEntry));
    if (new_entry == NULL) {
        return HASH_ERROR; // 内存分配失败
    }

    new_entry->key.str_value = strdup(key);
    if (new_entry->key.str_value == NULL) {
        free(new_entry);
        return HASH_ERROR; // 内存分配失败
    }

    new_entry->value.str_value = strdup(value);
    if (new_entry->value.str_value == NULL) {
        free(new_entry->key.str_value);
        free(new_entry);
        return HASH_ERROR; // 内存分配失败
    }

    new_entry->val_tag = HASH_STRING;
    new_entry->next = table->bucket[index];
    table->bucket[index] = new_entry;

    return HASH_OK; // 添加成功
}

HASH_RESULT hash_del_str(HashTable* table, char* key) {
    // 检查参数是否有效
    if (table == NULL || key == NULL || table->bucket == NULL || table->size <= 0) {
        return HASH_ERROR;
    }

    // 计算哈希索引
    unsigned long index = (unsigned long)hash_string(key) % table->size;
    HashEntry *entry = table->bucket[index];
    HashEntry *prev = NULL;

    // 遍历链表查找目标键
    while (entry) {
        if (strcmp(entry->key.str_value, key) == 0) {
            // 找到目标键并执行删除操作
            if (prev) {
                prev->next = entry->next;
            } else {
                table->bucket[index] = entry->next;
            }

            free(entry->key.str_value);
            free(entry->value.str_value);
            free(entry);

            return HASH_DELETED; // 删除成功
        }

        prev = entry;
        entry = entry->next;
    }

    return HASH_NOT_FOUND; // 未找到键
}
