
#include "bound.h"

Comparison bcompare( Bound *self, Bound *other )
{
  Comparison comp;
  // if it's clearly lesser or greater, then look no further
  if ( ( comp = fcompare( self->frac, other->frac ) ) != Equal )
  {
    return comp;
  }
  else
  {
    if ( self->closed && !other->closed )
    {
      return Lesser;
    }
    else if ( !self->closed && other->closed )
    {
      return Greater;
    }
    else
    {
      return Equal;
    }
  }
}

void bcombine( Bound *self, Bound *other )
{
  if ( bcompare( self, other ) == Equal )
  {
    self->closed = self->closed || other->closed;
  }
  else
  {
    self->frac = other->frac;
    self->closed = other->closed;
  }
}

