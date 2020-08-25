#include <stdio.h>
#include <stdlib.h>
#include"command.h"
#include"parser.h"
#include"eval.h"
#include"exec.h"
#include"lexer.h"


void test_eval();

int main(int agrc, char **argv) {
  FILE * fp = fopen("C:\\Users\\Jabin\\source\\repos\\Shell\\test.sh", "r");
  if (fp == NULL) {
    printf("file not exist!\n");
  }
  parser_init(fp);
  while (lexer.peek != EOF) {
    COMMAND *cmd =  _statement();
    exec_command(cmd);
  }
  fclose(fp);
  return 0;
}

void test_eval() { 
	char *s = "4*(1+20)/6&&15";
    printf("%d\n", cal(s));
}

