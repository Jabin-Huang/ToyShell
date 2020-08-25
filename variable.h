#pragma once

 #include"hash.h"

typedef union val {
  int numVal;
  char* str;
} VAL;
typedef enum {STR, _INT} VAR_TYPE;
/*
	±‰¡ø
*/
typedef struct var {
  char* name;
  VAR_TYPE type;
  VAL value;
}Var;

HASH_TABLE* var_table;

Var* val_get(char* name);

Var* var_insert(char* name, Var* v);

char* num2String(int v);