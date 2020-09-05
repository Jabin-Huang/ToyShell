#pragma once
#include "token.h"
#include "variable.h"

/* Command(命令) 的种类 */
enum command_type {
  cm_for,
  cm_case,
  cm_while,
  cm_if,
  cm_simple,
  cm_select,
  cm_connection,
  cm_function_def,
  cm_until,
  cm_arith,
  cm_cond,
  cm_arith_for,
  cm_assign,
};

/* 命令 */
typedef struct command {
  enum command_type type;
  int line;
  struct command *next;
  union {
    struct for_com *For;
    struct case_com *Case;
    struct while_com *While;
    struct until_com *Until;
    struct if_com *If;
    struct connection *Connection;
    struct simple_com *Simple;
    struct function_def *Function_def;
    struct arith_com *Arith;
    struct cond_com *Cond;
    struct arith_for_com *ArithFor;
    struct new_assign_com *Assign;
  } value;
} COMMAND;

/* 命令拼接 */
typedef struct connection {
  int ignore;
  COMMAND *first;
  COMMAND *second;
  int connector;
} CONNECTION;


/* for name [ [ in [ word ... ] ] ; ] do list ; done */
typedef struct for_com {
  char *var;       /* 循环时用于映射相应值的变量 */
  char **map_list; /* 待遍历的全体对象 */
  int len_list;
  COMMAND *action; /*循环体内的命令*/
} FOR_COM;

/*
  for (( expr1 ; expr2 ; expr3 )) ; do list ; done
*/

typedef struct arith_for_com {
  int line; /* generally used for error messages */
  COMMAND *init;
  COMMAND *test;
  COMMAND *step;
  COMMAND *action;
} ARITH_FOR_COM;

// COMMAND *new_arith_for(COMMAND *init, COMMAND *test, COMMAND *step, COMMAND
// *action);

/* if */
typedef struct if_com {
  char* test;            /*测试条件*/
  COMMAND *true_case;  /* 真出口 */
  COMMAND *false_case; /* 假出口 */
} IF_COM;

/* while */
typedef struct while_com {
  char *test;
  COMMAND *action;
} WHILE_COM;

/* while */
typedef struct until_com {
  char *test;
  COMMAND *action;
} UNTIL_COM;

/* 最原子的命令 单词和重定向的集合 */
typedef struct simple_com {
  int line;
  TOKEN *name;
  char *args;
  COMMAND *pipeFrom;
  char **in;
  char *out;
} SIMPLE_COM;

/* 函数定义 */
typedef struct function_def {
  int line;
  TOKEN *name;
  COMMAND *command;
  char *source_file; /*函数定义在哪个文件（如有）*/
} FUNCTION_DEF;

typedef struct assign_com {
  char *name;
  char *exp;
  VAR_TYPE type;
} ASSIGN_COM;

COMMAND *new_assign_com(char *name, char *exp, VAR_TYPE type);

COMMAND *new_if_com(char *test, COMMAND *true_case, COMMAND *false_case);

COMMAND *new_simple_com(TOKEN *name, char *args, char **in);

COMMAND *new_until_com(char *test, COMMAND *action);

COMMAND *new_while_com(char *test, COMMAND *action);

COMMAND *new_for_com(char *name, char **map_list, int len_list,
                     COMMAND *action);