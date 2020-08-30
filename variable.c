#include "variable.h"

#include <stdlib.h>

#include "hash.h"

#include"lexer.h"

void create_symbolTable() { var_table = hash_create(0); }

Var* var_get(char* name) {
  BUCKET_CONTENTS* item;
  if (item = hash_search(name, var_table)) {
    return (Var*)item->data;
  } else {
    return var_update(name, NULL);
  }
}

Var* var_update(char* name, Var* v) {
  BUCKET_CONTENTS* item = hash_search(name, var_table);
  if (item == NULL) item = hash_insert(name, var_table);
  if (v != NULL) {
    item->data = v;
    return item->data;
  } else {
    Var* new = (Var*)malloc(sizeof(Var));
    if (new) {
      new->name = name;
      new->value.str = "";
      new->type = STR;
      item->data = new;
      return item->data;
    } else
      fprintf(stderr, "malloc memory failed!\n");
  }
}


char* num2String(int v) {
  char* s = newStr(0);
  _itoa_s(v, s, DEFAULT_STRLEN , 10);
  return s;
}
