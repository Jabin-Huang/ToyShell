#pragma once


typedef struct  stack_contents {
  void* data;
  struct stack_contents* next;
}STACK_CONTENTS;

typedef struct stack {
  int size;
  STACK_CONTENTS* top;
} STACK;

STACK_CONTENTS* stack_push(STACK* s, void *data);
STACK_CONTENTS* stack_pop(STACK* s);
STACK* createStack();
