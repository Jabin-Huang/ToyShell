#pragma once
#include "token.h"

/*
        重定向描述，如果重定向操作为复制文件操作符(ri_duplicating_input or
   ri_duplicating_output)，使用DEST（文件描述符），不然使用filename（文件名）
*/
typedef union {
  int dest;
  TOKEN_LIST *filename;
} REDIRECTEE;

typedef struct redirect {
  struct redirect *next;
  REDIRECTEE redirector;          /* 被重定向的描述符或变量名 */
  int rflags;                     /* private 的标志位 */
  int flags;                      /* 文件的打开方式 */
  enum r_instruction instruction; /* 重定向的操作类型 */
  REDIRECTEE redirectee;          /* 文件描述符 或 文件名 */
  // char *here_doc_eof;    /* Here Document 相关 */
} REDIRECT;

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
};

/* command（命令）的标志位  */
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

/* 命令 */
typedef struct command {
  enum command_type type;
  int flags;
  int line;
  struct command *next;
  REDIRECT *redirecs;  // FOR  CASE中 的特殊重定向
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

/* 命令拼接 */
typedef struct connection {
  int ignore;
  COMMAND *first;
  COMMAND *second;
  int connector;
} CONNECTION;

/* Values for FLAGS word in a PATTERN_LIST */
#define CASEPAT_FALLTHROUGH 0x01
#define CASEPAT_TESTNEXT 0x02

/*case 中的模式 */
typedef struct pattern_list {
  struct pattern_list *next;
  TOKEN_LIST *patterns; /* 需测试的模式 */
  COMMAND *action;      /* 模式测试成功后执行的命令 */
  int flags;
} PATTERN_LIST;

/* case */
typedef struct case_com {
  int flags;
  int line;
  Token *word;           /*要测试的值*/
  PATTERN_LIST *clauses; /*进行匹配的模式链表*/
} CASE_COM;

/* for name [ [ in [ word ... ] ] ; ] do list ; done */
typedef struct for_com {
  int flags;
  int line;
  Token *name;     /* 循环时用于映射相应值的变量 */
  Token *map_list; /* 待遍历的全体对象 */
  COMMAND *action; /*循环体内的命令*/
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
  COMMAND *test;       /*测试条件*/
  COMMAND *true_case;  /* 真出口 */
  COMMAND *false_case; /* 假出口 */
} IF_COM;

IF_COM* new_if (COMMAND*test, COMMAND* ture_case, COMMAND* false_case);

/* while */
typedef struct while_com {
  int flags;
  COMMAND *test;
  COMMAND *action;
} WHILE_COM;

WHILE_COM* new_while(COMMAND *test, COMMAND *action);

/* 计算算术表达式 ((...)) */
/* The arithmetic evaluation command, ((...)).  Just a set of flags and
   a WORD_LIST, of which the first element is the only one used, for the
   time being. */
typedef struct arith_com {
  int flags;
  int line;
  Token *exp;
} ARITH_COM;

ARITH_COM *new_arith(Token *exp);

/* 条件测试， [[ ...]]*/
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

/* 最原子的命令 单词和重定向的集合 */
typedef struct simple_com {
  int flags;
  int line;
  TOKEN_LIST *words;   /* 命令名， 参数， 变量赋值等*/
  REDIRECT *redirects; /* 要执行的重定向 */
  COMMAND *pipeFrom;
  char *in;
  char *out;
} SIMPLE_COM;

/* 函数定义 */
typedef struct function_def {
  int flags;
  int line;
  Token *name;
  COMMAND *command;
  char *source_file; /*函数定义在哪个文件（如有）*/
} FUNCTION_DEF;


