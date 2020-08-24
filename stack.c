#include <stdlib.h>
#include "stack.h"


STACK_CONTENTS* stack_push(STACK* s, void* data) {
  STACK_CONTENTS* new = (STACK_CONTENTS*)malloc(sizeof(STACK_CONTENTS));
  if (new != NULL) {
    new->data = data;
    new->next = s->top;
    s->top = new;
    s->size++;
    return new;
  }
  return NULL;
}

STACK_CONTENTS* stack_pop(STACK* s) {
  STACK_CONTENTS* old_top = s->top;
  STACK_CONTENTS* new_top = s->top->next;
  s->top = new_top;
  s->size--;
  return old_top;
}

 

STACK* createStack() {
  STACK* new = (STACK*)malloc(sizeof(STACK));
  if (new != NULL) {
    new->top = NULL;
    new->size = 0;
    return new;
  }
  return NULL;
}