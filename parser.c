
#include <stdio.h>
#include "command.h"
#include "exec.h"
#include "parser.h"

Token* look;

void _move() {
    look = scan(); 
}

void _move_str() {
  char* s = readStr();
  look = newToken(s, STRING);
}

void parser_init() {
  lex_init();
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
      break;
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
  COMMAND* condition = _condition();
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
    return new_if(condition, s1, s2);
  } else if (look->tag == ELIF) {
    return new_if(condition, s1, _elif());
  } else {
    match(FI);
    return new_if(condition, s1, NULL);
  }
}

COMMAND* _elif() {
  match(ELIF);
  COMMAND* condition = _condition();
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
    return new_if(condition, s1, s2);
  } else if (look->tag == ELIF) {
    return new_if(condition, s1, _elif());
  } else {
    match(FI);
    return new_if(condition, s1, NULL);
  }
}

COMMAND* _while() {
  match(WHILE);
  COMMAND* condition = _condition();
  match(DO);
  COMMAND* stmt = _statement();
  COMMAND* before = stmt;
  while (look->tag != DONE) {
    COMMAND* s = _statement();
    before->next = s;
    before = s;
  }
  match(DONE);
  return new_while(condition, stmt);
}

COMMAND* _until() {
  match(UNTIL);
  COMMAND* conditon = _condition();
  match(DO);
  COMMAND* stmt = _statement();
  COMMAND* before = stmt;
  while (look->tag != DONE) {
    COMMAND* s = _statement();
    before->next = s;
    before = s;
  }
  match(DONE);
  return new_until(conditon, stmt);
}

COMMAND* _condition() {
  switch (look->tag) {
    case DOUBLE_LEFT_BUCKET_ARITH:
      return _arith();
    case DOUBLE_LEFT_BUCKET_BOOL:
      return _bool();
    case '[':
    case TEST:
      return _test();
    default:
      return NULL;
  }
}

COMMAND* _for() {
  match(FOR);
  match(DOUBLE_LEFT_BUCKET_ARITH);
  COMMAND* init = _arith();
  match(';');
  COMMAND* test = _arith();
  match(';');
  COMMAND* next = _arith();
  match(DOUBLE_RIGHT_BUCKET_ARITH);
  match(DO);
  COMMAND* stmt = _statement();
  COMMAND* before = stmt;
  while (look->tag != DONE) {
    COMMAND* s = _statement();
    before->next = s;
    before = s;
  }
  match(DONE);
  return new_arith_for(init, test, next, stmt);
}

COMMAND* _builtin(COMMAND* pipefrom) { 
    TOKEN_LIST *tl = malloc(sizeof(TOKEN_LIST) );
    tl->word = look;
    tl->next = NULL;
    match(BUILTIN);
    int len = 0;
    char* arg = NULL;
    char* in = STDIN;
    char* out = STDOUT;

    if (look->tag == ARGS) {
      arg = look->lexeme;
    }
    
    /*Ã»Ð´Íê*/
      
    if (look->tag =='<') {
      match('<');
      in = look->lexeme;
    }
    while (look->tag == '|') {
      match(BUILTIN);
          
    }

    

}


COMMAND* _arith() { 
    
}

COMMAND* _bool() { return NULL; }

COMMAND* _assign() { return NULL; }

