
#include "fraction.h"

Comparison fcompare( Fraction *self, Fraction *other )
{
  int ours   = self->num * other->den;
  int theirs = self->den * other->num;

  if ( ours > theirs )
  {
    return Greater;
  }
  else if ( ours < theirs )
  {
    return Lesser;
  }
  else
  {
    return Equal;
  }
}

void reduce( Fraction *self )
{
  int a = self->num;
  int b = self->den;

  while ( b > 0 )
  {
    int rem = a % b;
    a = b;
    b = rem;
  }

  // a is the GCD

  self->num /= a;
  self->den /= a;
}

