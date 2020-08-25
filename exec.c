#include "exec.h"

#include <stdlib.h>

#include "command.h"
#include "eval.h"
#include "variable.h"
#include "general.h"

void exec_command(COMMAND* cmd) {
  switch (cmd->type) {
    case cm_assign:
      exec_assign(cmd);
    case cm_if:
      exec_if(cmd);
    case cm_while:
      exec_while(cmd);
    case cm_until:
      exec_until(cmd);
    case cm_for:
      exec_until(cmd);
    case cm_simple:
      exec_simple(cmd);
    default:
      exit(-1);
  }
}

void exec_assign(COMMAND* cmd) {
  ASSIGN_COM* assign = cmd->value.Assign;
  Var* new = (Var*)malloc(sizeof(Var));
  new->name = assign->name;
  new->value = assign->value;
  new->type = assign->type;
  var_insert(assign->name, new);
}

void exec_if(COMMAND* cmd) {
  IF_COM* if_com = cmd->value.If;
  COMMAND* c = NULL;
  if (cal(if_com->test)) {
    c = if_com->true_case;
  } else {
    c = if_com->false_case;
  }
  while (c != NULL) {
    exec_command(c);
    c = c->next;
  }
}

void exec_while(COMMAND* cmd) {
  WHILE_COM* while_com = cmd->value.While;
  COMMAND* c = NULL;
  while (cal(while_com->test)) {
    c = while_com->action;
    while (c != NULL) {
      exec_command(c);
      c = c->next;
    }
  }
}

void exec_until(COMMAND* cmd) {
  UNTIL_COM* until_com = cmd->value.Until;
  COMMAND* c = until_com->action;
  do {
    while (c != NULL) {
      exec_command(c);
      c = c->next;
    }
  } while (cal(until_com->test));
}

void exec_for(COMMAND* cmd) { 
    FOR_COM* for_com = cmd->value.For; 
    for (int i = 0; i < for_com->len_list; ++i) {
      Var* v = val_get(for_com->var);
      v->value.str = savestring(for_com->map_list[i]);
      COMMAND* c = for_com->action;
      while (c != NULL) {
        exec_command(c);
        c = c->next;
      }
    }
}

void exec_simple(COMMAND* cmd) { 
    SIMPLE_COM* simple_com = cmd->value.Simple;
  if (strcmp(simple_com->name, "echo")) {
      printf("%s\n", (simple_com->in)[0]);
  }

}