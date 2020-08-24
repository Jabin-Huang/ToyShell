#pragma once


void parser_init();
void _move();
void _move_str();
void match(int tag);
void test_parser();
COMMAND* _statement();
COMMAND* _if();
COMMAND* _elif();
COMMAND* _while();
COMMAND* _until();
COMMAND* _condition();
COMMAND* _for();
COMMAND* _builtin(COMMAND* pipefrom);
COMMAND* _arith();
COMMAND* _bool();
COMMAND* _assign();