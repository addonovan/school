#include <stdlib.h>
#include "stack.h"

void stinit( Stack *this )
{
  $.count = 0;
  $.head = NULL;
}

void stpush( Stack* this, Interval* interval )
{
  StackNode* node = malloc( sizeof( StackNode ) );
  node->data = interval;
  node->next = $.head;

  $.head = node;
  $.count++;
}

Interval* stpop( Stack* this )
{
  if ( $.count == 0 )
  {
    return NULL;
  }

  $.count--;

  StackNode* current = $.head;
  $.head = current->next;

  Interval* front = current->data;
  free( current );

  return front;
}
