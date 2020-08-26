#pragma once
#include<stdio.h>
#include"hash.h"
#include"token.h"

struct sh_lexer {
  int line;
  char peek;
  char last_peek;
  TOKEN* last;
  HASH_TABLE* reserve_table;
  HASH_TABLE* var_table;
  FILE* source;
} lexer;

void _readch();
int readch(char );

int isLetter(char );
int isNum(char );
char* newStr(char );
char* readStr();
void lex_init();
TOKEN* add_Val(char* );
TOKEN* scan();
TOKEN* newToken(char* , unsigned int );
