#include <stdio.h>
#include <stdlib.h>
#include"command.h"
#include"parser.h"
#include"eval.h"
#include"exec.h"
#include"lexer.h"


int main(int agrc, char **argv) {
  FILE *fp = NULL;
  errno_t  err = 0;
  if(argv[1]) err = fopen_s(&fp, argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr, "File open failed! Please input in hands:\n");
    fp = stdin;
  }
  parser_init(fp);
  printf("%d\n", lexer.peek);
  while (lexer.peek != EOF) {
    COMMAND *cmd =  _statement();
    exec_command(cmd);
  }
  getch();
  if(fp) fclose(fp);
  return 0;
}


