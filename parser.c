#include <stdio.h>
#include <string.h>
#include "command.h"
#include "eval.h"
#include "exec.h"
#include "lexer.h"
#include "variable.h"
#include "parser.h"

TOKEN* look;

void _move() {
  look = scan();
}

void parser_init(FILE* fp) {
  lex_init(fp);
  create_symbolTable();
  _move();
}

void match(int tag) {
  if (tag == look->tag)
    _move();
  else {
    fprintf(stdout, "parse error: expected (%d), but found %s(%d) !\n", tag,
            look->lexeme, look->tag);
  }
}


COMMAND* _statement() {
  switch (look->tag) {
    case IF:
      return _if();
    case WHILE:
      return _while();
    case UNTIL:
      return _until();
    case FOR:
      return _for();
    case BUILTIN:
      return _builtin(NULL);
    case VAR:
      return _assign();
    default:
      printf("按任意键退出\n");
      getch();
      exit(0);
  }
}


COMMAND* _if() {
  match(IF);
  char* condition = _exp();
  match(THEN);
  COMMAND* s1 = _statement();
  COMMAND* before = s1;
  while (look->tag != ELSE && look->tag != ELIF && look->tag != FI) {
    COMMAND* s = _statement();
    before->next = s;
    before = s;
  }
  if (look->tag == ELSE) {
    match(ELSE);
    COMMAND* s2 = _statement();
    before = s2;
    while (look->tag != FI) {
      COMMAND* s = _statement();
      before->next = s;
      before = s;
    }  
    match(FI);
    return new_if_com(condition, s1, s2);
  } else if (look->tag == ELIF) {
    return new_if_com(condition, s1, _elif());
  } else {
    match(FI);
    return new_if_com(condition, s1, NULL);
  }
}

COMMAND* _elif() {
  match(ELIF);
  char* condition = _exp();
  match(THEN);
  COMMAND* s1 = _statement();
  COMMAND* before = s1;
  while (look->tag != ELSE && look->tag != ELIF && look->tag != FI) {
    COMMAND* s = _statement();
    before->next = s;
    before = s;
  } 
  if (look->tag == ELSE) {
    match(ELSE);
    COMMAND* s2 = _statement();
    before = s2;
    while (look->tag != FI){
      COMMAND* s = _statement();
      before->next = s;
      before = s;
    }  
    match(FI);
    return new_if_com(condition, s1, s2);
  } else if (look->tag == ELIF) {
    return new_if_com(condition, s1, _elif());
  } else {
    match(FI);
    return new_if_com(condition, s1, NULL);
  }
}

COMMAND* _while() {
  match(WHILE);
  char* condition = _exp();
  match(DO);
  COMMAND* stmt = _statement();
  COMMAND* before = stmt;
  while (look->tag != DONE) {
    COMMAND* s = _statement();
    before->next = s;
    before = s;
  }
  match(DONE);
  return new_while_com(condition, stmt);
}

COMMAND* _until() {
  match(UNTIL);
  char* conditon = _exp();
  match(DO);
  COMMAND* stmt = _statement();
  COMMAND* before = stmt;
  while (look->tag != DONE) {
    COMMAND* s = _statement();
    before->next = s;
    before = s;
  }
  match(DONE);
  return new_until_com(conditon, stmt);
}

char* _exp() {
  if (look->tag == EXP || look->tag == NUM || look->tag == HAS_DOLLAR) {
    char* exp = look->lexeme;
    match(look->tag);
    return exp;
  }
}

COMMAND* _for() {
  char* var;
  char** list = (char**)malloc(50 * sizeof(char*));
  match(FOR);
  var = newStr(0);
  strcpy(var, look->lexeme);
  match(VAR);
  match(_IN);
  int len_list = 0;
  while (look->tag != DO) {
    if (look->tag == STRING) {
      list[len_list] = newStr(0);
      strcpy(list[len_list], look->lexeme);
      len_list++;
      match(STRING);
    }
  }
  match(DO);
  COMMAND* stmt = _statement();
  COMMAND* before = stmt;
  while (look->tag != DONE) {
    COMMAND* s = _statement();
    before->next = s;
    before = s;
  }
  match(DONE);
  return new_for_com(var, list, len_list, stmt);
}

COMMAND* _builtin(COMMAND* pipefrom) {
  char* name = look->lexeme;
  match(BUILTIN);
  char* opnion = NULL;
  char** in = (char**)malloc(10 * sizeof(char*));
  if (in == NULL) exit(-1);
  if (look->tag == OPNION) {
    opnion = look->lexeme;
    match(OPNION);
  }
  int i = 0;
  if (look->tag == ARG || look->tag == FINAL_ARG) {
    while (look->tag == ARG) {
      int len = strlen(look->lexeme) + 1;
      char* s = (char*)malloc(sizeof(char) * (1 + strlen(look->lexeme)));
      if (s == NULL) exit(-1);
      in[i] = strcpy(s, (look->lexeme));
      i++;
      match(ARG);
    }
    int len = strlen(look->lexeme) + 1;
    char* s = (char*)malloc(sizeof(char) * (1 + strlen(look->lexeme)));
    if (s == NULL) exit(-1);
    in[i] = strcpy(s, (look->lexeme));
    i++;
    match(FINAL_ARG);
  }

  /*管道、重定向*/

  /*  if (look->tag =='<') {
      match('<');
      in = look->lexeme;
      ...
    }
    while (look->tag == '|') {
      match(BUILTIN);
          ...
    }*/
  return new_simple_com(name, opnion, in);
}

COMMAND* _assign() {
  char* name = look->lexeme;
  match(VAR);
  match('=');
  TOKEN* value = look;
  char *exp;
  VAR_TYPE t;
  if (look->tag == STRING) {
    exp = newStr(0);
    t = STR;
    strcpy(exp, look->lexeme);
    match(STRING);
  } else {
    exp = _exp();
    t = _INT;
  }
  return new_assign_com(name, exp, t);
}
