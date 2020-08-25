#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "command.h"
#include "exec.h"
#include "lexer.h"
#include "parser.h"
#include "eval.h"
#include "variable.h"
TOKEN* look;

void _move() {
    look = scan(); 
    printf("%s\n", look->lexeme);
}

void _move_arg() {
  char* s = readStr();
  if (s != '\0')
    look = newToken(s, ARG);
  else
    _move();
}

void parser_init() {
  lex_init();
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

void test_parser() {
  parser_init();
  while (look->lexeme != EOF) {
    printf("(%s, %d)\n", look->lexeme, look->tag);
    _move();
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
    default:
      return _assign();
  }
}

// COMMAND* statements(int end) {
//  if (look->tag == end) {
//    return NULL;
//  } else
//    return new_stmts(_statement(), statements());
//}

COMMAND* _if() {
  match(IF);
  char* condition = _exp();
  match(THEN);
  COMMAND* s1 = _statement();
  COMMAND* before = s1;
  do {
    COMMAND* s = _statement();
    before->next = s;
    before = s;
  } while (look->tag != ELSE && look->tag != ELIF && look->tag != FI);
  if (look->tag == ELSE) {
    match(ELSE);
    COMMAND* s2 = _statement();
    before = s2;
    do {
      COMMAND* s = _statement();
      before->next = s;
      before = s;
    } while (look->tag != FI);
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
  do {
    COMMAND* s = _statement();
    before->next = s;
    before = s;
  } while (look->tag != ELSE && look->tag != ELIF && look->tag != FI);
  if (look->tag == ELSE) {
    match(ELSE);
    COMMAND* s2 = _statement();
    before = s2;
    do {
      COMMAND* s = _statement();
      before->next = s;
      before = s;
    } while (look->tag != FI);
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
  if (look->tag == EXP || look->tag == NUM || look->tag == STRING) {
    char* exp = look->lexeme;
    match(look->tag);
    return exp;
  }
}

COMMAND* _for() {
  char* var;
  char** list = (char**)malloc(50*sizeof(char*));
  match(FOR);
  var = newStr(0);
  strcpy(var, look->lexeme);
  match(VAR);
  match(_IN);
  int len_list = 0;
  while (look->tag != DO) {
    _move_arg();
    if (look->tag == STRING) {
      list[len_list++] = newStr(0);
      strcpy(list[len_list], look->lexeme);
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
    char** in = (char**)malloc(10*sizeof(char*));
    if (look->tag == OPNION) {
      opnion = look->lexeme;
    }
    _move_arg();
    int i = 0;
    if (look->tag == ARG) {
      while (look->tag == ARG) {
        in[i++] = savestring(look->lexeme);
        _move_arg();
      }
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
  VAL v;
  VAR_TYPE t;
  if (look->tag == STRING) {
    v.str = newStr(0);
    t = STR;
    strcpy(v.str, look->lexeme);
    match(STRING);
  } else {
    v.numVal = cal(_exp());
    t = _INT;
  }
  return new_assign_com(name, v, t);
}

