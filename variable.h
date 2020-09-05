#pragma once

 #include"hash.h"

typedef union val {
  int exp;
  char* str;
} VAL;
typedef enum {STR, _INT} VAR_TYPE;
/*
	变量
*/
typedef struct var {
  char* name;
  VAR_TYPE type;
  VAL value;
}Var;

HASH_TABLE* var_table;

Var* var_get(char* name);

Var* var_update(char* name, Var* v);

char* num2String(int v);