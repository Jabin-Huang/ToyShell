#pragma once
#include"command.h"

void exec_command(COMMAND* cmd);

void exec_assign(COMMAND* cmd);

void exec_if(COMMAND* cmd);

void exec_while(COMMAND* cmd);

void exec_until(COMMAND* cmd);

void exec_for(COMMAND* cmd);

void exec_simple(COMMAND* cmd);
