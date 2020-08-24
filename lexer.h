#pragma once

#include"hash.h"
#include"token.h"

struct sh_lexer {
  int line;
  char peek;
  TOKEN* last;
  HASH_TABLE* reserve_table;
  HASH_TABLE* var_table;
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
TOKEN_LIST* token_insert(TOKEN_LIST*, TOKEN*);
TOKEN_LIST* newTokenList();