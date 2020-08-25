#pragma once


void parser_init();
void _move();
void _move_arg();
void match(int tag);
void test_parser();
COMMAND* _statement();
COMMAND* _if();
COMMAND* _elif();
COMMAND* _while();
COMMAND* _until();
char* _exp();
COMMAND* _for();
COMMAND* _builtin(COMMAND* pipefrom);
int _arith();
COMMAND* _assign();