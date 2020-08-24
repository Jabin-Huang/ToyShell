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

/* command������ı�־λ  */
#define CMD_WANT_SUBSHELL 0x01     /* User wants a subshell: ( command ) */
#define CMD_FORCE_SUBSHELL 0x02    /* Shell needs to force a subshell. */
#define CMD_INVERT_RETURN 0x04     /* Invert the exit value. */
#define CMD_IGNORE_RETURN 0x08     /* Ignore the exit value.  For set -e. */
#define CMD_NO_FUNCTIONS 0x10      /* Ignore functions during command lookup. */
#define CMD_INHIBIT_EXPANSION 0x20 /* Do not expand the command words. */
#define CMD_NO_FORK 0x40           /* Don't fork; just call execve */
#define CMD_TIME_PIPELINE 0x80     /* Time a pipeline */
#define CMD_TIME_POSIX 0x100       /* time -p; use POSIX.2 time output spec. */
#define CMD_AMPERSAND 0x200        /* command & */
#define CMD_STDIN_REDIR 0x400      /* async command needs implicit </dev/null */
#define CMD_COMMAND_BUILTIN 0x0800 /* command executed by `command' builtin */
#define CMD_COPROC_SUBSHELL 0x1000
#define CMD_LASTPIPE 0x2000

/* ���� */
typedef struct command {
  enum command_type type;
  int flags;
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
  int flags;
} PATTERN_LIST;

/* case */
typedef struct case_com {
  int flags;
  int line;
  Token *word;           /*Ҫ���Ե�ֵ*/
  PATTERN_LIST *clauses; /*����ƥ���ģʽ����*/
} CASE_COM;

/* for name [ [ in [ word ... ] ] ; ] do list ; done */
typedef struct for_com {
  int flags;
  int line;
  Token *name;     /* ѭ��ʱ����ӳ����Ӧֵ�ı��� */
  Token *map_list; /* ��������ȫ����� */
  COMMAND *action; /*ѭ�����ڵ�����*/
} FOR_COM;
FOR_COM *new_for(Token *name, Token *map_list, COMMAND *action);

/*
  for (( expr1 ; expr2 ; expr3 )) ; do list ; done
*/

typedef struct arith_for_com {
  int flags;
  int line; /* generally used for error messages */
  COMMAND *init;
  COMMAND *test;
  COMMAND *step;
  COMMAND *action;
} ARITH_FOR_COM;
ARITH_FOR_COM *new_arith_for(COMMAND *init, COMMAND *test, COMMAND *step, COMMAND *action); 

/* if */
typedef struct if_com {
  int flags;
  COMMAND *test;       /*��������*/
  COMMAND *true_case;  /* ����� */
  COMMAND *false_case; /* �ٳ��� */
} IF_COM;

IF_COM* new_if (COMMAND*test, COMMAND* ture_case, COMMAND* false_case);

/* while */
typedef struct while_com {
  int flags;
  COMMAND *test;
  COMMAND *action;
} WHILE_COM;

WHILE_COM* new_while(COMMAND *test, COMMAND *action);

/* �����������ʽ ((...)) */
/* The arithmetic evaluation command, ((...)).  Just a set of flags and
   a WORD_LIST, of which the first element is the only one used, for the
   time being. */
typedef struct arith_com {
  int flags;
  int line;
  Token *exp;
} ARITH_COM;

ARITH_COM *new_arith(Token *exp);

/* �������ԣ� [[ ...]]*/
#define COND_AND 1
#define COND_OR 2
#define COND_UNARY 3
#define COND_BINARY 4
#define COND_TERM 5
#define COND_EXPR 6

typedef struct cond_com {
  int flags;
  int line;
  int type;
  Token *op;
  struct cond_com *left, *right;
} COND_COM;

COND_COM *new_cond(Token *op, COND_COM *left, COND_COM *right);

/* ��ԭ�ӵ����� ���ʺ��ض���ļ��� */
typedef struct simple_com {
  int flags;
  int line;
  TOKEN_LIST *words;   /* �������� ������ ������ֵ��*/
  REDIRECT *redirects; /* Ҫִ�е��ض��� */
  COMMAND *pipeFrom;
  char *in;
  char *out;
} SIMPLE_COM;

/* �������� */
typedef struct function_def {
  int flags;
  int line;
  Token *name;
  COMMAND *command;
  char *source_file; /*�����������ĸ��ļ������У�*/
} FUNCTION_DEF;


