#include <stdio.h>
#include <stdlib.h>
#include"command.h"
#include"parser.h"
#include"eval.h"
//void sh_loop(void);

int EOF_reached;

COMMAND *global_command = (COMMAND *)NULL;

void test_eval();

int main(int agrc, char **argv) {
  // load config files, if any
  //...
  printf("%d ", agrc);
  for (int i = 0; i < agrc; ++i) printf("%s", argv[i]);
  //sh_loop();
  //test_parser();
  parser_init();
  _statement();
  return 0;
}

void test_eval() { 
	char *s = "4*(1+20)/6&&15";
    printf("%d\n", cal(s));
}

