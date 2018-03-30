#include <stdio.h>
#include "interval.h"

void iinit( Interval* this, int start, int end, int weight )
{
  $.start = start;
  $.end = end;
  $.weight = weight;
  $.prior = 0;
}

bool iconflicts( Interval *this, Interval *other )
{
  if ( $.start > other->start )
  {
    return iconflicts( other, this );
  }

  // [==============]
  // [=====]
  if ( $.start == other->start ) return true;

  // [==============]
  //        [=======]
  if ( $.end == other->end ) return true;

  // [==============]
  //      [=====]
  if ( $.start < other->start && $.end > other->end ) return true;

  // [==============]
  //          [==============]
  if ( $.start < other->start && $.end > other->start ) return true;

  return false;
}

void icreatelinks( Interval* intervals, int count )
{
  // prior will be initialized to NULL by default

  int i, j;
  for ( i = 0; i < count; i++ )
  {
    // find the largest prior
    for ( j = i - 1; j >= 0; j-- )
    {
      if ( iconflicts( intervals + j, intervals + i ) == false )
      {
        intervals[ i ].prior = j;
        break; // we've found the largest prior, no need to continue looping
      }
    }
  }
}

void iprint( Interval* this )
{
  printf( "%-3d %-3d %-3d\n", $.start, $.end, $.weight );
}
