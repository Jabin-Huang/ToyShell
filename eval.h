#pragma once

#include"stack.h"
#define N_OPTR 17

typedef enum {
  L_P,
  NOT,
  MUL,
  DIV,
  MOD,
  ADD,
  SUB,
  _GE,
  _LE,
  GT,
  LT,
  _EQ,
  _NE,
  AND,
  OR,
  R_P,
  EOE
} Operator;


Operator optr2rank(char *op);

char orderBetween(char *op1, char *op2);

void* cal(char *exp, int flag);

int *calcu(int t1, char *op, int t2);

char *var_expand(char *exp);

void readNumber(char **exp, STACK *s);

void readOPtr(char **exp, STACK *s);
