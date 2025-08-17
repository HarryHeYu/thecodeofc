typedef enum{
    HASH_OK,
    HASH_ERROR,
    HASH_ADDED,
    HASH_REPLACED_VALUE,
    HASH_ALREADY_ADDED,
    HASH_DELETED,
    HASH_NOT_FOUND,
} HASH_RESULT;

typedef enum
{
	HASH_PTR,
	HASH_NUMERIC,
	HASH_STRING,
} HASH_VAL_TAG;

typedef struct __HashEntry HashEntry;
struct __HashEntry{
    union{
        char  *str_value;
        double dbl_value;
        int    int_value;
    } key;
    HASH_VAL_TAG val_tag;
    union{
        char  *str_value;
        double dbl_value;
        int    int_value;
        long   long_value;
        void  *ptr_value;
    } value;
    HashEntry *next;
};

struct __HashTable{
    HashEntry **bucket;        
    int size;
    HASH_RESULT last_error;
};
typedef struct __HashTable HashTable;

// 向哈希表中添加元素，其中键类型为char*， 元素类型为char*。
HASH_RESULT hash_add_str(HashTable* table, const char *key, char *value );
// 从哈希表中删除元素，其中键类型为char*。
HASH_RESULT hash_del_str(HashTable *table, char *key );

long hash_string(const char *str){
    long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    if(hash < 0)
        hash *= -1;
    return hash;
}