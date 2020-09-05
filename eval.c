#pragma once
 
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "eval.h"
#include "variable.h"
#include "lexer.h"


int isNum(char);  //在lexer.c中已定义
int isLetter(char);

/*运算符优先等级 [栈顶] [当前]*/
const char pri[N_OPTR][N_OPTR] = {    
            /*  (     !    *    /    %    +    -    >=    <=    >    <   ==   !=   &&   ||    )   \0 */
/* (  */       '<',  '<', '<', '<', '<', '<', '<',  '<',  '<', '<', '<', '<', '<', '<', '<', '=', ' ',
/* !  */       '<',  '<', '>', '>', '>', '>', '>',  '>',  '>', '>', '>', '>', '>', '>', '>', '>', '>', 
/* *  */       '<',  '<', '>', '>', '>', '>', '>',  '>',  '>', '>', '>', '>', '>', '>', '>', '>', '>',
/* /  */       '<',  '<', '>', '>', '>', '>', '>',  '>',  '>', '>', '>', '>', '>', '>', '>', '>', '>',
/* %  */       '<',  '<', '>', '>', '>', '>', '>',  '>',  '>', '>', '>', '>', '>', '>', '>', '>', '>',
/*  + */       '<',  '<', '<', '<', '<', '>', '>',  '>',  '>', '>', '>', '>', '>', '>', '>', '>', '>',
/* -  */       '<',  '<', '<', '<', '<', '>', '>',  '>',  '>', '>', '>', '>', '>', '>', '>', '>', '>',
/* >= */       '<',  '<', '<', '<', '<', '<', '<',  '>',  '>', '>', '>', '>', '>', '>', '>', '>', '>',
/* <= */       '<',  '<', '<', '<', '<', '<', '<',  '>',  '>', '>', '>', '>', '>', '>', '>', '>', '>',
/* >  */       '<',  '<', '<', '<', '<', '<', '<',  '>',  '>', '>', '>', '>', '>', '>', '>', '>', '>',
/* <  */       '<',  '<', '<', '<', '<', '<', '<',  '>',  '>', '>', '>', '>', '>', '>', '>', '>', '>',
/* == */       '<',  '<', '<', '<', '<', '<', '<',  '<',  '<', '<', '<', '>', '>', '>', '>', '>', '>',
/* != */       '<',  '<', '<', '<', '<', '<', '<',  '<',  '<', '<', '<', '>', '>', '>', '>', '>', '>',
/* && */       '<',  '<', '<', '<', '<', '<', '<',  '<',  '<', '<', '<', '<', '<', '>', '>', '>', '>',
/* || */       '<',  '<', '<', '<', '<', '<', '<',  '<',  '<', '<', '<', '<', '<', '>', '>', '>', '>',
/* )  */       ' ',  ' ', ' ', ' ', ' ', ' ', ' ',  ' ',  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
/* \0 */       '<',  '<', '<', '<', '<', '<', '<',  '<',  '<', '<', '<', '<', '<', '<', '<', ' ', '=',
};


/*由运算符转译出编号 */
Operator optr2rank(char *op) {
  switch (op[0]) {
    case '(': return L_P; 
    case ')': return R_P;
    case '!':
      if (*(op + 1) == '=') return _NE;
      return NOT;
    case '*': return MUL;
    case '/': return DIV;
    case '+': return ADD;
    case '-': return SUB;
    case '>':
      if (*(op + 1) == '=') {
        return _GE;
      }
      else return GT;
    case '<':
      if (*(op + 1) == '=') {
        return _LE;
      }
      else return LT;
    case '=':
      if (*(op + 1) == '=') {
        return _EQ;
      }
      else  exit(-1);
    case '&':
      if (*(op + 1) == '&') {
        return AND;
      }
      else exit(-1);
    case '|':
      if (*(op + 1) == '|') {
        return OR;
      }
      else  exit(-1);
    case '\0': return EOE; //起止符与终止符
    default: exit(-1); 
  }
}


char orderBetween(char *op1, char *op2) {
  return pri[optr2rank(op1)][optr2rank(op2)];
}

void* cal(char* exp, int flag) {
  exp = var_expand(exp);
  void *res = NULL;
  if (flag == 0 ) {
    STACK opnd = {0, NULL};  //运算数;
    STACK optr = {0, NULL};  //运算符;
    char *eoe = savestring("");
    stack_push(&optr, eoe);  //哨兵
    while (optr.size != 0) {
      if (isNum(*exp)) {
        readNumber(&exp, &opnd);
      } else {
        switch (orderBetween((char *)optr.top->data, exp)) {
          case '<':
            readOPtr(&exp, &optr);
            break;
          case '=': {
            STACK_CONTENTS *t = stack_pop(&optr);
            char *op = (char *)t->data;
            free(op);
            op = NULL;
            free(t);
            t = NULL;
            exp++;
            break;
          }
          case '>': {
            STACK_CONTENTS *t = stack_pop(&optr);
            char *op = (char *)t->data;
            if ('!' == op[0] && '=' != op[1]) {  //一元
              STACK_CONTENTS *t = stack_pop(&opnd);
              int *n = (int *)t->data;
              free(t);
              t = NULL;
              stack_push(&opnd, calcu(NULL, op, *n));
              free(n);
              n = NULL;
            } else {  //二元
              STACK_CONTENTS *t2 = stack_pop(&opnd);
              STACK_CONTENTS *t1 = stack_pop(&opnd);
              int *n2 = (int *)t2->data;
              int *n1 = (int *)t1->data;
              free(t1);
              t1 = NULL;
              free(t2);
              t2 = NULL;
              stack_push(&opnd, calcu(*n1, op, *n2));
              free(n1);
              n1 = NULL;
              free(n2);
              n2 = NULL;
            }
            free(op);
            op = NULL;
            free(t);
            t = NULL;
            break;
          }
          default:
            exit(-1);
        }
      }
    }
    STACK_CONTENTS *t = stack_pop(&opnd);
    res =  (int *)t->data;
    free(t);
    t = NULL;
  } else if(flag == 1){
    res = exp;
  }
  return res;
}

int* calcu(int t1, char *op, int t2) { 
    int *res = (int *)malloc(sizeof(int));
    *res = 0;
    if (op[0] == '!' && op[1] != '=') {
      *res = !t2;
    } else {
      switch (op[0]) {
        case '!':
          if (op[1] == '=') *res = (t1 != t2);
          break;
        case '*':
          *res = t1 * t2;
          break;
        case '/':
          *res = t1 / t2;
          break;
        case '+':
          *res = t1 + t2;
          break;
        case '-':
          *res = t1 - t2;
          break;
        case '>':
          if (op[1] == '=') {
            *res = (t1 >= t2);
          } else
            *res = (t1 > t2);
          break;
        case '<':
          if (op[1] == '=') {
            *res = (t1 <= t2);
          } else
            *res = (t1 < t2);
          break;
        case '=':
          if (op[1] == '=') {
            *res = (t1 == t2);
          } 
          else exit(-1);
          break;
        case '&':
          if (op[1] == '&') {
            *res = (t1 && t2);
          }
          else exit(-1);
          break;
        case '|':
          if (op[1] == '|') {
            *res = (t1 && t2);
          } else exit(-1);
          break;
        default:
          exit(-1);
      }
    }
    return res;
}

void readNumber(char **exp, STACK *s) {
   int* v =(int*)malloc(sizeof(int));
  if (v != NULL) {
     *v = 0;
     while (isNum(*(*exp))) {
       (*v) = (*v) * 10 + *(*exp) - '0';
       (*exp)++;
     }
     //printf("%d\n", *v);
     stack_push(s, v);
  } else
    exit(-1);
}

void readOPtr(char **exp, STACK *s) { 
    char *v = (char*)malloc(3*sizeof(char));
    if (v != NULL) {
      if ((*exp)[0] == '<' && (*exp)[1] == '=') {
        v[0] = (*exp)[0];
        v[1] = (*exp)[1];
        v[2] = '\0';
        (*exp)++;
      } else if ((*exp)[0] == '>' && (*exp)[1] == '=') {
        v[0] = (*exp)[0];
        v[1] = (*exp)[1];
        v[2] = '\0';
        (*exp)++;
      } else if ((*exp)[0] == '!' && (*exp)[1] == '=') {
        v[0] = (*exp)[0];
        v[1] = (*exp)[1];
        v[2] = '\0';
        (*exp)++;
      } else if ((*exp)[0] == '=' && (*exp)[1] == '=') {
        v[0] = (*exp)[0];
        v[1] = (*exp)[1];
        v[2] = '\0';
        (*exp)++;
      } else if ((*exp)[0] == '&' && (*exp)[1] == '&') {
        v[0] = (*exp)[0];
        v[1] = (*exp)[1];
        v[2] = '\0';
        (*exp)++;
      } else if ((*exp)[0] == '|' && (*exp)[1] == '|') {
        v[0] = (*exp)[0];
        v[1] = (*exp)[1];
        v[2] = '\0';
        (*exp)++;
      } else {
        v[0] = (*exp)[0];
        v[1] = '\0';
      }
      //printf("%s\n", v);
      stack_push(s, v);
      (*exp)++;
    } else
         exit(-1);
}

char* var_expand(char* exp) {
  char *str = newStr(0);
  char *temp = newStr(0);
  while (*exp) {
    if (*exp == '$') {
      int len = 0;
      exp++;
      if (*exp == '{') {
        exp++;
        do {
          temp[len++] = *exp;
          exp++;
        } while (*exp != '}');
        temp[len] = '\0';
        exp++;
      } else {
        do {
          temp[len++] = *exp;
          exp++;
        } while (isLetter(*exp) || isNum(*exp) ||
                 *exp == '_');
        temp[len] = '\0';
       
      }
      Var* v = var_get(temp);
      char *ele = NULL;
	
      if (v->type == _INT) {
        ele = num2String(v->value.exp);
      } else {
        ele = v->value.str;
      } 
      str = strcat_s(str, strlen(str) + strlen(ele) + 1 , ele);
    } else {
      int len = strlen(str);
      str[len++] = *exp;
      str[len] = '\0';
      exp++;
    }
  }
  free(temp);
  temp = NULL;
  return str;
}