#pragma once
#include "general.h"

typedef struct bucket_contents {
  struct bucket_contents* next;
  char* key;		
  void* data;			//void* 万能指针
  unsigned int khash;   //key哈希过后的值
  int times_found;		//被查找到的次数
} BUCKET_CONTENTS;

typedef struct hash_table {
  BUCKET_CONTENTS** bucket_array;
  int nbuckets;  //哈希桶数
  int nentries;  //总元素个数
}HASH_TABLE;

typedef int hash_wfunc(BUCKET_CONTENTS*);

#if !defined(savestring)
#define savestring(x) (char*)strcpy(malloc(1 + strlen(x)), (x))
#endif

HASH_TABLE* hash_create(int);
HASH_TABLE* hash_copy(HASH_TABLE*, sh_string_func_t*);
void hash_flush(HASH_TABLE*, sh_free_func_t*);
void hash_walk(HASH_TABLE* table, hash_wfunc* func);
void hash_dspose(HASH_TABLE*);
 

int hash_bucket(const char*, HASH_TABLE*);
int hash_size(HASH_TABLE*);

BUCKET_CONTENTS* hash_search(const char*, HASH_TABLE*);
BUCKET_CONTENTS* hash_insert(char *, HASH_TABLE* );
BUCKET_CONTENTS* hash_remove(const char*, HASH_TABLE*);

unsigned int hash_string(const char* );

//获取指定bucket的元素
#define hash_items(bucket, table) \
	((table && (bucket < table->nbuckets))? \
	  table->bucket_array[bucket]: \
	  (BUCKET_CONTENTS*) NULL)

//默认BUCKETS数量
#define DEFAULT_HASH_BUCKETS 128

#define HASH_ENTRIES(ht) ((ht) ? (ht)->nentries : 0)



