#pragma once
#include "general.h"

typedef struct bucket_contents {
  struct bucket_contents* next;
  char* key;		
  void* data;			//void* ����ָ��
  unsigned int khash;   //key��ϣ�����ֵ
  int times_found;		//�����ҵ��Ĵ���
} BUCKET_CONTENTS;

typedef struct hash_table {
  BUCKET_CONTENTS** bucket_array;
  int nbuckets;  //��ϣͰ��
  int nentries;  //��Ԫ�ظ���
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

//��ȡָ��bucket��Ԫ��
#define hash_items(bucket, table) \
	((table && (bucket < table->nbuckets))? \
	  table->bucket_array[bucket]: \
	  (BUCKET_CONTENTS*) NULL)

//Ĭ��BUCKETS����
#define DEFAULT_HASH_BUCKETS 128

#define HASH_ENTRIES(ht) ((ht) ? (ht)->nentries : 0)



