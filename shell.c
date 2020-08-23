#include <stdio.h>
#include <stdlib.h>
#include"command.h"
#include"parser.h"

//void sh_loop(void);

int EOF_reached;

COMMAND *global_command = (COMMAND *)NULL;


int main(int agrc, char** argv) {
  // load config files, if any
  //...
  printf("%d ", agrc);
  for (int i = 0; i < agrc; ++i) printf("%s", argv[i]);
  //sh_loop();
  test_parser();
  return 0;
}

