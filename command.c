#pragma once
#include<stdlib.h>
#include"command.h"
#include"variable.h"
#include"lexer.h"

COMMAND* new_assign_com(char* name, VAL v ,VAR_TYPE type) {
  ASSIGN_COM* assign_com = (ASSIGN_COM *) (malloc(sizeof(ASSIGN_COM)));
  assign_com->name = name;
  assign_com->value = v;
  assign_com->type = type;
  COMMAND* assign = (COMMAND*)(malloc(sizeof(COMMAND)));
  assign->line = lexer.line;
  assign->type = cm_assign;
  assign->next = NULL;
  assign->value.Assign = assign_com;
  return assign;
}

COMMAND* new_if_com(char* test, COMMAND* true_case, COMMAND* false_case) {
  IF_COM* if_com = (IF_COM*)(malloc(sizeof(IF_COM)));
  if_com->test = test;
  if_com->true_case = true_case;
  if_com->false_case = false_case;
  COMMAND* _if = (COMMAND*)(malloc(sizeof(COMMAND)));
  _if->line = lexer.line;
  _if->type = cm_if;
  _if->next = NULL;
  _if->value.If = if_com;
  return _if;
} 

COMMAND* new_while_com(char* test, COMMAND* action) {
  WHILE_COM* while_com = (WHILE_COM*)(malloc(sizeof(WHILE_COM)));
  while_com->test = test;
  while_com->action = action;
  COMMAND* _while = (COMMAND*)(malloc(sizeof(COMMAND)));
  _while->line = lexer.line;
  _while->type = cm_while;
  _while->next = NULL;
  _while->value.While = while_com;
  return _while;
}

COMMAND* new_until_com(char* test, COMMAND* action) {
  WHILE_COM* until_com = (WHILE_COM*)(malloc(sizeof(WHILE_COM)));
  until_com->test = test;
  until_com->action = action;
  COMMAND* _until = (COMMAND*)(malloc(sizeof(COMMAND)));
  _until->line = lexer.line;
  _until->type = cm_until;
  _until->next = NULL;
  _until->value.Until = until_com;
  return _until;
}

COMMAND* new_for_com(char* var, char** list, int len_list,  COMMAND* action) {
  FOR_COM* for_com = (FOR_COM*)(malloc(sizeof(FOR_COM)));
  for_com->var = var;
  for_com->map_list = list;
  for_com->len_list = len_list;
  for_com->action = action;
  COMMAND* _for = (COMMAND*)(malloc(sizeof(COMMAND)));
  _for->line = lexer.line;
  _for->type = cm_until;
  _for->next = NULL;
  _for->value.For = for_com;
  return _for;
}

COMMAND* new_simple_com(TOKEN* name, char* args, char** in) {
  SIMPLE_COM* simple_com = (SIMPLE_COM*)(malloc(sizeof(SIMPLE_COM)));
  simple_com->name = name;
  simple_com->args = args;
  simple_com->in = in;
  COMMAND* simple = (COMMAND*)(malloc(sizeof(COMMAND)));
  simple->line = lexer.line;
  simple->type = cm_simple;
  simple->next = NULL;
  simple->value.Simple = simple_com;
  return simple;
}