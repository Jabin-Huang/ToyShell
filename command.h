#pragma once
#include "token.h"

/*
        �ض�������������ض������Ϊ�����ļ�������(ri_duplicating_input or
   ri_duplicating_output)��ʹ��DEST���ļ�������������Ȼʹ��filename���ļ�����
*/
typedef union {
  int dest;
  TOKEN_LIST *filename;
} REDIRECTEE;

typedef struct redirect {
  struct redirect *next;
  REDIRECTEE redirector;          /* ���ض����������������� */
  int rflags;                     /* private �ı�־λ */
  int flags;                      /* �ļ��Ĵ򿪷�ʽ */
  enum r_instruction instruction; /* �ض���Ĳ������� */
  REDIRECTEE redirectee;          /* �ļ������� �� �ļ��� */
  // char *here_doc_eof;    /* Here Document ��� */
} REDIRECT;

/* Command(����) ������ */
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
};


/* ���� */
typedef struct command {
  enum command_type type;
  int line;
  struct command *next;
  REDIRECT *redirecs;  // FOR  CASE�� �������ض���
  union {
    struct for_com *For;
    struct case_com *Case;
    struct while_com *While;
    struct if_com *If;
    struct connection *Connection;
    struct simple_com *Simple;
    struct function_def *Function_def;
    struct arith_com *Arith;
    struct cond_com *Cond;
    struct arith_for_com *ArithFor;
  } value;
} COMMAND;

/* ����ƴ�� */
typedef struct connection {
  int ignore;
  COMMAND *first;
  COMMAND *second;
  int connector;
} CONNECTION;

/* Values for FLAGS word in a PATTERN_LIST */
#define CASEPAT_FALLTHROUGH 0x01
#define CASEPAT_TESTNEXT 0x02

/*case �е�ģʽ */
typedef struct pattern_list {
  struct pattern_list *next;
  TOKEN_LIST *patterns; /* ����Ե�ģʽ */
  COMMAND *action;      /* ģʽ���Գɹ���ִ�е����� */
} PATTERN_LIST;

/* case */
typedef struct case_com {
  int line;
  TOKEN *word;           /*Ҫ���Ե�ֵ*/
  PATTERN_LIST *clauses; /*����ƥ���ģʽ����*/
} CASE_COM;

/* for name [ [ in [ word ... ] ] ; ] do list ; done */
typedef struct for_com {
  int line;
  TOKEN *name;     /* ѭ��ʱ����ӳ����Ӧֵ�ı��� */
  TOKEN *map_list; /* ��������ȫ����� */
  COMMAND *action; /*ѭ�����ڵ�����*/
} FOR_COM;

FOR_COM *new_for(TOKEN *name, TOKEN *map_list, COMMAND *action);

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

ARITH_FOR_COM *new_arith_for(COMMAND *init, COMMAND *test, COMMAND *step, COMMAND *action); 

/* if */
typedef struct if_com {
  int test;       /*��������*/
  COMMAND *true_case;  /* ����� */
  COMMAND *false_case; /* �ٳ��� */
} IF_COM;

IF_COM* new_if (int test, COMMAND* ture_case, COMMAND* false_case);

/* while */
typedef struct while_com {
  int test;
  COMMAND *action;
} WHILE_COM;

WHILE_COM* new_while(int test, COMMAND *action);

/* �����������ʽ ((...)) */
typedef struct arith_com {
  int line;
  TOKEN *exp;
} ARITH_COM;

ARITH_COM *new_arith(TOKEN *exp);

/* �������ԣ� [[ ...]]*/
#define COND_AND 1
#define COND_OR 2
#define COND_UNARY 3
#define COND_BINARY 4
#define COND_TERM 5
#define COND_EXPR 6

typedef struct cond_com {
  int line;
  int type;
  TOKEN *op;
  struct cond_com *left, *right;
} COND_COM;

COND_COM *new_cond(TOKEN *op, COND_COM *left, COND_COM *right);

/* ��ԭ�ӵ����� ���ʺ��ض���ļ��� */
typedef struct simple_com {
  int line;
  TOKEN_LIST *words;   /* �������� ������ ������ֵ��*/
  REDIRECT *redirects; /* Ҫִ�е��ض��� */
  COMMAND *pipeFrom;
  char *in;
  char *out;
} SIMPLE_COM;

/* �������� */
typedef struct function_def {
  int line;
  TOKEN *name;
  COMMAND *command;
  char *source_file; /*�����������ĸ��ļ������У�*/
} FUNCTION_DEF;


