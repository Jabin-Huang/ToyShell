#include "lexer.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Reserved words.  These are only recognized as the first word of a
   command. */
Token word_token_alist[] = {
    {"if", IF},
    {"then", THEN},
    {"else", ELSE},
    {"elif", ELIF},
    {"fi", FI},
    {"case", CASE},
    {"esac", ESAC},
    {"for", FOR},
    {"while", WHILE},
    {"while", WHILE},
    {"until", UNTIL},
    {"do", DO},
    {"done", DONE},
    {"function", FUNCTION},
    {"echo", BUILTIN},

};

void _readch() {
  lexer.last = lexer.peek;
  lexer.peek = getchar();
}

int readch(char c) {
  _readch();
  if (lexer.peek != c) return 0;
  lexer.peek = ' ';
  return 1;
}

Token* newToken(char* str, unsigned int tag) {
  Token* tok = (Token*)malloc(sizeof(Token));
  tok->lexeme = str;
  tok->tag = tag;
  return tok;
}

TOKEN_LIST* newTokenList() {
  TOKEN_LIST* list = (TOKEN_LIST*)malloc(sizeof(TOKEN_LIST));
  list->next = NULL;
  list->word = NULL;
  return list;
}

TOKEN_LIST* token_insert(TOKEN_LIST* list, Token* token) {
  list->next = newTokenList();
  list->next->word = token;
  list->next->next = NULL;
  return list->next;
}

int isLetter(char c) { return 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z'; }

int isNum(char c) { return '0' <= c && c <= '9'; }

char* newStr(char c) {
  char* str = NULL;
  if (c != 0) {
    str = (char*)malloc(sizeof(char) + 1);
    if (str == 0) {
      fprintf(stderr, "failed to new a str\n!");
      return 0;
    }
    str[0] = c;
    str[1] = '\0';
  } else {
    str = (char*)malloc(256 * sizeof(char));
    if (str == 0) {
      fprintf(stderr, "failed to new a str\n!");
      return 0;
    }
  }
  return str;
}

char* readStr() {
  char* str = newStr(0);
  int len = 0;
  do {
    str[len++] = lexer.peek;
    _readch();
  } while (lexer.peek != ' ' && lexer.peek != '\n' && lexer.peek != '\t');
  str[len] = '\0';
  return str;
}

void lex_init() {
  lexer.line = 0;
  lexer.peek = ' ';
  lexer.last = ' ';
  lexer.reserve_table = hash_create(0);
  lexer.var_table = hash_create(0);
  int reserve_len = sizeof(word_token_alist) / sizeof(Token);

  for (int i = 0; i < reserve_len; ++i) {
    Token* t = &word_token_alist[i];
    BUCKET_CONTENTS* item = hash_insert(t->lexeme, lexer.reserve_table);
    item->data = t;
    printf("key %s, khash %u\n", item->key, item->khash);
  }
}

Token* add_Val(char* name) {
  BUCKET_CONTENTS* item = hash_insert(name, lexer.var_table);
  return item->data = newToken(name, VAR);
}

Token* scan() {
  /* ����ע��,�ո� */
  for (;; _readch()) {
    if (lexer.peek == '#') {
      do {
        _readch();
      } while (lexer.peek != '\n');
      if (lexer.peek == '\n') lexer.line = lexer.line + 1;
    } else if (lexer.peek == '\n')
      lexer.line = lexer.line + 1;
    else if (lexer.peek == '\t' || lexer.peek == ' ')
      continue;
    else
      break;
  }

  /*�����*/
  switch (lexer.peek) {
    case '&':
      if (readch('&'))
        return newToken("&&", AND_AND);
      else
        return newToken("&", '&');
    case '|':
      if (readch('|'))
        return newToken("||", OR_OR);
      else
        return newToken("|", '|');
    case '=':
      if (readch('='))
        return newToken("==", EQ);
      else
        return newToken("=", '=');
    case '!':
      if (readch('='))
        return newToken("!=", NE);
      else
        return newToken("!", '!');
    case '>':
      if (readch('='))
        return newToken(">=", GE);
      else
        return newToken(">", '>');
    case '<':
      if (readch('='))
        return newToken("<=", LE);
      else
        return newToken("<", '<');
    case '-': {
      char* str = readStr();
      if (str[1] == 'o')
        return newToken("-o", OR_OR);
      else if (str[1] == 'a')
        return newToken("-a", AND_AND);
      else if (str[1] != '\0') {
        if (strcmp(str, "-eq") == 0)
          return newToken("-eq", EQ);
        else if (strcmp(str, "-ne") == 0)
          return newToken("-ne", NE);
        else if (strcmp(str, "-gt") == 0)
          return newToken("-gt", '>');
        else if (strcmp(str, "-lt") == 0)
          return newToken("-lt", '<');
        else if (strcmp(str, "-ge") == 0)
          return newToken("-ge", GE);
        else if (strcmp(str, "-le") == 0)
          return newToken("-le", LE);
        else
          return newToken(str, ARGS);
      } else
        return newToken("-", '-');
    }
  }

  /*����*/
  if (isNum(lexer.peek)) {
    char* str = newStr(0);
    int len = 0;
    do {
      str[len++] = lexer.peek;
      _readch();
    } while (isNum(lexer.peek));
    str[len] = '\0';
    return newToken(str, NUM);
  }

  /*������������*/
  if (lexer.last != '$' && (isLetter(lexer.peek) || lexer.peek == '_')) {
    char* str = newStr(0);
    int len = 0;
    do {
      str[len++] = lexer.peek;
      _readch();
    } while (isLetter(lexer.peek) || isNum(lexer.peek) || lexer.peek == '_');
    str[len] = '\0';
    BUCKET_CONTENTS* item;
    if (item = hash_search(str, lexer.reserve_table)) {
      return item->data;
    } else if (item = hash_search(str, lexer.var_table)) {
      return item->data;
    } else {
      return add_Val(str);
    }
  }

  /*��������*/
  if (lexer.peek == '$') {
    char* str = newStr(0);
    str[0] = '$';
    int len = 1;
    _readch();
    if (lexer.peek == '{') {
      do {
        str[len++] = lexer.peek;
        _readch();
      } while (lexer.peek != '}');
      str[len++] = '}';
      str[len] = '\0';
    } else {
      do {
        str[len++] = lexer.peek;
        _readch();
      } while (isLetter(lexer.peek) || isNum(lexer.peek) || lexer.peek == '_');
      str[len] = '\0';
    }
    return newToken(str, DOLLAR_VAR);
  }

  /*�ַ���*/
  if (lexer.last == '=' || lexer.peek == '"' || lexer.peek == '\'') {
    char* str;
    if (lexer.peek == '"' || lexer.peek == '\'') {
      char t = lexer.peek;
      str = newStr(0);
      int len = 0;
      do {
        str[len++] = lexer.peek;
        _readch();
      } while (lexer.peek != t);
      str[len++] = lexer.peek;
      str[len] = '\0';
      lexer.peek = ' ';
    } else {
      str = readStr();
    }
    return newToken(str, STRING);
  }

  //��ѧ���ʽ����
  if (lexer.peek == '(' || lexer.peek == ')') {
    char t = lexer.peek;
    _readch();
    if (lexer.peek == '(') {
      return newToken("((", DOUBLE_LEFT_BUCKET_ARITH);
    } else if (lexer.peek == ')') {
      return newToken("))", DOUBLE_RIGHT_BUCKET_ARITH);
    } else {
      return newToken(newStr(t), t);
    }
  }

  //�ַ������ļ����� ?
  if (lexer.peek == '[' || lexer.peek == ']') {
    char t = lexer.peek;
    _readch();
    if (lexer.peek == '[') {
      return newToken("[[", DOUBLE_LEFT_BUCKET_BOOL);
    } else if (lexer.peek == ']') {
      return newToken("]]", DOUBLE_RIGHT_BUCKET_BOOL);
    } else {
      return newToken(newStr(t), t);
    }
  }

  //�ļ�·��, �涨��б�ܿ�ͷ
  if (lexer.peek == '\\') {
    char* str = readStr();
    return newToken(str, FILE_PATH);
  }

  /*�������ַ�*/
  Token* t = newToken(newStr(lexer.peek), lexer.peek);
  lexer.peek = ' ';

  return t;
}
