#pragma once

#define IF 258
#define THEN 259
#define ELSE 260
#define ELIF 261
#define FI 262
#define CASE 263
#define ESAC 264
#define FOR 265
#define SELECT 266
#define WHILE 267
#define UNTIL 268
#define DO 269
#define DONE 270
#define FUNCTION 271
#define COND_START 272
#define COND_END 273
#define IN 274
#define AND_AND 275
#define OR_OR 276
#define GREATER_GREATER 277
#define LESS_LESS 278
#define LE 279
#define GE 280
#define EQ 281
#define NE 282
#define ASSIGN 283
#define VAR 284
#define NUM 285
#define BUILTIN 286
#define STRING 287
#define HAS_DOLLAR 288
#define DOUBLE_LEFT_BUCKET_ARITH 289
#define DOUBLE_RIGHT_BUCKET_ARITH 290
#define DOUBLE_LEFT_BUCKET_BOOL 291
#define DOUBLE_RIGHT_BUCKET_BOOL 292
#define BREAK 293
#define CONTINUE 294
#define TEST 295
#define ARGS 296
#define FILE_PATH 297
#define ARITH_EXP 298
#define BOOL_EXP 299

typedef struct token {
  char *lexeme;
  unsigned int tag;
} TOKEN;

/* µ•¥ ¡¥±Ì */
typedef struct token_list {
  struct token_list *next;
  TOKEN *word;
} TOKEN_LIST;

