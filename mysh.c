#include <stdio.h>
#include <stdlib.h>
#include"command.h"
#include"parser.h"
#include"eval.h"
#include"exec.h"
#include"lexer.h"


int main(int agrc, char **argv) {
  FILE * fp = fopen(argv[1], "r");
  if (fp == NULL) {
    printf("file not exist!\n");
  }
  parser_init(fp);
  while (lexer.peek != EOF) {
    COMMAND *cmd =  _statement();
    exec_command(cmd);
  }
  getch();
  if(fp) fclose(fp);
  return 0;
}


