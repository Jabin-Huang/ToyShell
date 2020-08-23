#pragma once

 

/*
	╠Да©
*/
typedef struct var_context {
  char* name;
  union {
    int val;
    char* str;
  }value;
}Var;

