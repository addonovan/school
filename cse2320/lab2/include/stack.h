#ifndef STACK_H
#define STACK_H

#include "interval.h"

typedef struct StackNode StackNode;
typedef struct Stack Stack;

/**
 * A node in the stack.
 */
struct StackNode
{
  Interval* data;
  StackNode* next;
};

/**
 * A stack implemented using a singly linked list.
 */
struct Stack
{
  int count;
  StackNode* head;
};

/**
 * Initializes a stack.
 *
 * @param this
 */
void stinit( Stack* this );

/**
 * Pushes a new element onto this stack.
 *
 * @param this
 * @param interval
 *          The element to push onto the stack.
 */
void stpush( Stack* this, Interval* interval );

/**
 * Pops the head off of the stack.
 *
 * @param this
 * @return
 *          The first element on the stack.
 */
Interval* stpop( Stack* this );

#endif
