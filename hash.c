#include "hash.h"
#include <stdlib.h>
#include <string.h>


/*
    buckets:ָ����Ͱ��
*/
HASH_TABLE* hash_create(int buckets) {
  HASH_TABLE* new_table;
  register int i;
  new_table = (HASH_TABLE*)malloc(sizeof(HASH_TABLE));
  if (buckets == 0) {
    buckets = DEFAULT_HASH_BUCKETS;
  }

  new_table->bucket_array =
      (BUCKET_CONTENTS**)malloc(buckets * sizeof(BUCKET_CONTENTS));
  new_table->nbuckets = buckets;
  new_table->nentries = 0;

  for (i = 0; i < buckets; ++i) {
    new_table->bucket_array[i] = (BUCKET_CONTENTS*)NULL;
  }

  return new_table;
}

int hash_size(HASH_TABLE* table) { return (HASH_ENTRIES(table)); }

/*
    ba:���Ƶ����ݣ�
    sh_string_func_t���������õķ���
*/
static BUCKET_CONTENTS* copy_bucket_array(BUCKET_CONTENTS* ba,
                                          sh_string_func_t* cpdata) {
  BUCKET_CONTENTS *new_bucket, *n, *e;

  if (ba == 0) {
    return ((BUCKET_CONTENTS*)0);
  }

  for (n = (BUCKET_CONTENTS*)0, e = ba; e; e = e->next) {
    if (n == 0) {
      new_bucket = (BUCKET_CONTENTS*)malloc(sizeof(BUCKET_CONTENTS));
      n = new_bucket;
    } else {
      n->next = (BUCKET_CONTENTS*)malloc(sizeof(BUCKET_CONTENTS));
      n = n->next;
    }
    n->key = savestring(e->key);
    //ʹ��ָ���ĸ�ֵ���������У�������ʹ��savestring�� ��
    n->data =
        e->data ? (cpdata ? (*cpdata)(e->data) : savestring(e->data)) : NULL;
    n->khash = e->khash;
    n->times_found = e->times_found;
    n->next = (BUCKET_CONTENTS*)NULL;
  }
  return new_bucket;
}

/*
    table:�����Ƶ�hashtable
    cpdata��bucket�ĸ��Ʒ���
*/
HASH_TABLE* hash_copy(HASH_TABLE* table, sh_string_func_t* cpdata) {
  HASH_TABLE* new_table;
  int i;

  if (table == 0) {
    return ((HASH_TABLE*)NULL);
  }

  new_table = hash_create(table->nbuckets);

  for (i = 0; i < table->nbuckets; ++i) {
    new_table->bucket_array[i] =
        copy_bucket_array(table->bucket_array[i], cpdata);
  }
  new_table->nentries = table->nentries;
  return new_table;
}

unsigned int hash_string(const char* s) {
  register unsigned int i;
  for (i = 0; *s; s++) {
    i *= 16777619;
    i ^= *s;
  }
  return i;
}

/* h Ϊ�ַ�����ϣֵ��HASH_BUCKET��һ������ת��Ϊ���� */
#define HASH_BUCKET(s, t, h) (((h) = hash_string(s)) & ((t)->nbuckets - 1))

/*
    ����str����bucket������
*/
int hash_bucket(const char* str, HASH_TABLE* table) {
  unsigned int h;
  return h = (hash_string(str) & (table->nbuckets - 1));
}

/*
    ����str���ڵ�bucket_contentָ�룬����������򷵻�NULL��
*/
BUCKET_CONTENTS* hash_search(const char* str, HASH_TABLE* table) {
  BUCKET_CONTENTS* list;
  int bucket;
  unsigned int hv;

  if (table == 0 || HASH_ENTRIES(table) == 0) {
    return (BUCKET_CONTENTS*)NULL;
  }

  bucket = HASH_BUCKET(str, table, hv);

  for (list = table->bucket_array ? table->bucket_array[bucket] : 0; list;
       list = list->next) {
    if (hv == list->khash && strcmp(list->key, str) == 0) {
      list->times_found++;
      return (list);
    }
  }

  return (BUCKET_CONTENTS*)NULL;
}

/*
    ɾ��һ����㣨δ�ͷ��ڴ棩�����ظý���ָ�룬���û������ͷ�
*/
BUCKET_CONTENTS* hash_remove(const char* str, HASH_TABLE* table) {
  int bucket;
  BUCKET_CONTENTS *prev, *temp;
  unsigned int hv;

  if (table == 0 || HASH_ENTRIES(table) == 0) {
    return (BUCKET_CONTENTS*)NULL;
  }

  bucket = HASH_BUCKET(str, table, hv);
  prev = (BUCKET_CONTENTS*)NULL;
  for (temp = table->bucket_array[bucket]; temp; temp = temp->next) {
    if (hv == temp->khash && strcmp(temp->key, str)) {
      if (prev)
        prev->next = temp->next;
      else
        table->bucket_array[bucket] = temp->next;

      table->nentries--;
      return (temp);
    }
    prev = temp;
  }
  return ((BUCKET_CONTENTS*)NULL);
}

BUCKET_CONTENTS* hash_insert(char* str, HASH_TABLE* table) {
  BUCKET_CONTENTS* item;
  int bucket;
  unsigned int hv;

  if (table == 0) {
    table = hash_create(0);
  }

  item =  hash_search(str, table);
  if (item == 0) {
    bucket = HASH_BUCKET(str, table, hv);

    item = (BUCKET_CONTENTS*)malloc(sizeof(BUCKET_CONTENTS));
    item->next = table->bucket_array[bucket];
    table->bucket_array[bucket] = item;

    item->data = NULL;
    item->key = str;
    item->khash = hv;
    item->times_found = 0;

    table->nentries++;
  }

  return (item);
}

void hash_flush(HASH_TABLE* table, sh_free_func_t* free_data) {
  int i;
  register BUCKET_CONTENTS *bucket, *item;

  if (table == 0 || HASH_ENTRIES(table) == 0) return;

  for (i = 0; i < table->nbuckets; ++i) {
    bucket = table->bucket_array[i];
    while (bucket) {
      item = bucket;
      bucket = bucket->next;

      if (free_data)
        (*free_data)(item->data);
      else {
        free(item->data);
      }
      free(item->key);
      free(item);
    }
    table->bucket_array[i] = (BUCKET_CONTENTS*)NULL;
  }
  table->nentries = 0;
}

void hash_dispose(HASH_TABLE* table) {
  free(table->bucket_array);
  free(table);
}


/*
    hash_wfunc:����ʱ��ȡ�ķ���

*/
void hash_walk(HASH_TABLE* table, hash_wfunc* func) {
  register int i;
  BUCKET_CONTENTS* item;

  if (table == 0 || HASH_ENTRIES(table) == 0) return;

  for (i = 0; i < table->nbuckets; ++i) {
    for (item = hash_items(i, table); item; item = item->next) {
      if ((*func)(item) < 0) 
        return;
    }  
  }
}