#pragma once
 
#include <stdlib.h>
#include "stack.h"
#include "eval.h"


int isNum(char);  //��lexer.c���Ѷ���

/*�������ת������ */
Operator optr2rank(char *op) {
  switch (*op) {
    case '(': return L_P; 
    case ')': return R_P;
    case '!':
      if (*(op + 1) == '=') return NE;
      return NOT;
    case '*': return MUL;
    case '/': return DIV;
    case '+': return ADD;
    case '-': return SUB;
    case '>':
      if (*(op + 1) == '=') return GE;
      else return GT;
    case '<':
      if (*(op + 1) == '=') return LE;
      else return LT;
    case '=':
      if (*(op + 1) == '=') return EQ;
      else  exit(-1);
    case '&':
      if (*(op + 1) == '&')  return AND;
      else exit(-1);
    case '|':
      if (*(op + 1) == '|') return OR;
      else  exit(-1);
    case '\0': return EOE; //��ֹ������ֹ��
    default: exit(-1); 
  }
}


char orderBetween(char op1, char op2) {
  return pri[optr2rank(op1)][optr2rank(op2)];
}

int arith_cal(char* exp) {
  STACK opnd = {0, NULL};  //������;
  STACK optr = {0, NULL};  //�����;
  char eoe = '\0';
  stack_push(&optr, &eoe); //�ڱ�
  while (optr.size != 0) {
    if (isNum(*exp)) {
                
    }
  }
  return 0;
}

int readNumber(char* s) {
  if (isNum(*s)) {
    
  }
}
int bool_cal(char* exp) { return 0; }
