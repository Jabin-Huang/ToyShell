#include "lexer.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Reserved words */
TOKEN word_token_alist[] = {
    {"if", IF},       {"then", THEN},         {"else", ELSE},   {"elif", ELIF},
    {"fi", FI},       {"case", CASE},         {"esac", ESAC},   {"for", FOR},
    {"while", WHILE}, {"while", WHILE},       {"until", UNTIL}, {"do", DO},
    {"done", DONE},   {"function", FUNCTION}, {"echo", BUILTIN}};

void _readch() { lexer.peek = getchar(); }

int readch(char c) {
  _readch();
  if (lexer.peek != c) return 0;
  lexer.peek = ' ';
  return 1;
}

TOKEN* newToken(char* str, unsigned int tag) {
  TOKEN* tok = (TOKEN*)malloc(sizeof(TOKEN));
  if (tok != NULL) {
    tok->lexeme = str;
    tok->tag = tag;
    lexer.last = tok;
    return tok;
  }
  return NULL;
}

TOKEN_LIST* newTokenList() {
  TOKEN_LIST* list = (TOKEN_LIST*)malloc(sizeof(TOKEN_LIST));
  if (list != NULL) {
    list->next = NULL;
    list->word = NULL;
    return list;
  }
  return NULL;
}

TOKEN_LIST* token_insert(TOKEN_LIST* list, TOKEN* token) {
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
  int reserve_len = sizeof(word_token_alist) / sizeof(TOKEN);

  for (int i = 0; i < reserve_len; ++i) {
    TOKEN* t = &word_token_alist[i];
    BUCKET_CONTENTS* item = hash_insert(t->lexeme, lexer.reserve_table);
    item->data = t;
    printf("key %s, khash %u\n", item->key, item->khash);
  }
}

TOKEN* add_Val(char* name) {
  BUCKET_CONTENTS* item = hash_insert(name, lexer.var_table);
  return item->data = newToken(name, VAR);
}

TOKEN* scan() {
  /* 忽略注释, 空格 */
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

  /*运算符*/
  switch (lexer.peek) {
    case '&': {
      if (readch('&'))
        return newToken("&&", AND_AND);
      else
        return newToken("&", '&');
    }
    case '|': {
      if (readch('|'))
        return newToken("||", OR_OR);
      else
        return newToken("|", '|');
    }
    case '=': {
      if (readch('='))
        return newToken("==", EQ);
      else
        return newToken("=", '=');
    }
    case '!': {
      if (readch('='))
        return newToken("!=", NE);
      else
        return newToken("!", '!');
    }
    case '>': {
      if (readch('='))
        return newToken(">=", GE);
      else
        return newToken(">", '>');
    }
    case '<': {
      if (readch('='))
        return newToken("<=", LE);
      else
        return newToken("<", '<');
    }
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
        else if (lexer.last->tag == BUILTIN)
          return newToken(str, ARGS);
      } else
        return newToken("-", '-');
    }
    default:
      break;
  }

  /*数字*/
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
  /*变量、保留词*/
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

  /*变量引用*/
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
      lexer.peek = ' ';
    } else {
      do {
        str[len++] = lexer.peek;
        _readch();
      } while (isLetter(lexer.peek) || isNum(lexer.peek) || lexer.peek == '_');
      str[len] = '\0';
    }
    return newToken(str, HAS_DOLLAR);
  }

  /*字符串*/
  if ((lexer.last && lexer.last->tag == '=') || lexer.peek == '"' || lexer.peek == '\'') {
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

  /*算术表达式 ((...)) */
  if (lexer.peek == '(') {
    _readch();
    if (lexer.peek == '(') {
      _readch();
      char* str = newStr(0);
      int len = 0;
      int cnt = 0; //通过括号匹配，使表达式中的 "))"不被误解析为结束分界符
      while(!(cnt == -1 && lexer.peek == ')' && lexer.last->tag == ')')) {
        if (lexer.peek == ' ') continue;
        str[len++] = lexer.peek;
        if (lexer.peek == '(') cnt++;
        if (lexer.peek == ')') cnt--;
        _readch();
      }
      //倒数第二个')'不应出现在表达式中
      str[--len] = '\0';
      lexer.peek = ' ';
      return newToken(str, ARITH_EXP);
    } else {
      return newToken("(", '(');
    }
  }

  /*条件测试 [[ ... ]]*/
  if (lexer.peek == '[') {
    _readch();
    if (lexer.peek == '[') {
      _readch();
      char* str = newStr(0);
      int len = 0;
      int cnt = 0;  //通过括号匹配，使表达式中的 "]]"不被误解析为结束分界符
      while (!(cnt == -1 && lexer.peek == ']' && lexer.last->tag == ']')) {
        if (lexer.peek == ' ') continue;
        str[len++] = lexer.peek;
        if (lexer.peek == '[') cnt++;
        if (lexer.peek == ']') cnt--;
        _readch();
      }
      //倒数第二个']'不应出现在表达式中
      str[--len] = '\0';
      lexer.peek = ' ';
      return newToken(str, BOOL_EXP);
    } else {
      return newToken("[", '[');
    }
  }

  /*文件路径, 约定只有相对路径*/
  if ((lexer.last && lexer.last->tag == '.') && lexer.peek == '\\') {
    char* str = readStr();
    return newToken(str, FILE_PATH);
  }

  /*其他单字符*/
  TOKEN* t = newToken(newStr(lexer.peek), lexer.peek);
  lexer.peek = ' ';

  return t;
}