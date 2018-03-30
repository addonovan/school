#include <stdio.h>
#include <stdlib.h>
#include "interval.h"

void init( Interval *interval, int low_num, int low_den, int up_num, int up_den )
{
  Bound *bounds = malloc( 2 * sizeof( Bound ) );
  Fraction *fractions = malloc( 2 * sizeof( Fraction ) );

  // initialize the lower bound
  interval->lower = bounds;
  interval->lower->closed = low_num > 0;
  interval->lower->frac = fractions;
  interval->lower->frac->num = low_num < 0 ? -low_num : low_num;
  interval->lower->frac->den = low_den;
  reduce( interval->lower->frac );

  // initialize the upper bound
  interval->upper = bounds + 1;
  interval->upper->closed = up_num > 0;
  interval->upper->frac = fractions + 1;
  interval->upper->frac->num = up_num < 0 ? -up_num : up_num;
  interval->upper->frac->den = up_den;
  reduce( interval->upper->frac );
}

void print_interval( Interval *self )
{
  printf( 
      "%d %d %d %d\n",
      self->lower->frac->num * ( self->lower->closed ? 1 : -1 ),
      self->lower->frac->den,

      self->upper->frac->num * ( self->upper->closed ? 1 : -1 ),
      self->upper->frac->den
  );
}

Comparison icompare( Interval *self, Interval *other )
{
  Comparison comp;
  // if it's clear which comes first, then just return that
  if ( ( comp = bcompare( self->lower, other->lower ) ) != Equal )
  {
    return comp;
  }
  // if they both start at the same place, then compare the upper bounds
  else
  {
    return bcompare( self->upper, other->upper );
  }
}

//
// Intersection
//

/**
 * Checks if two intervals have any overlap, beyond a single
 * point. This will *not* account for intervals that touch
 * each other at a single point, that is checked by another
 * function.
 *
 * This will only compare half of the necessary conditions, to
 * check all conditions fully, you must call the method again
 * with the parameters flipped.
 *
 * This checks if two intervals overlap at all. Intervals can
 * be said to be overlapp if one of the following is true:
 * 1) `b` is completely contained by `a`
 *    a: [========================]
 *    b:         [=========]
 *
 * 2) `b` begins before `a` ends, and continues after
 *    a: [=========]
 *    b:     [==========]
 */
bool intersection_overlap( Interval *a, Interval *b )
{
  // [============]
  //    [=====]
  if ( bcompare( a->lower, b->lower ) == Lesser
    && bcompare( a->upper, b->upper ) == Greater )
  {
    return true;
  }

  // [===========]
  //      [=========]
  if ( bcompare( a->lower, b->lower ) == Lesser
    && bcompare( a->upper, b->lower ) == Greater )
  {
    return true;
  }

  return false;
}

bool intersecting_bounds( Bound *a, Bound *b, bool same_side )
{
  return fcompare( a->frac, b->frac ) == Equal
      && ( same_side || a->closed || b->closed );
}

/**
 * Checks if the two intervals have any endpoints that
 * intersect each other.
 */
bool intersection_endpoints( Interval *a, Interval *b )
{
  return intersecting_bounds( a->lower, b->lower, true )
      || intersecting_bounds( a->lower, b->upper, false )
      || intersecting_bounds( a->upper, b->lower, false )
      || intersecting_bounds( a->upper, b->upper, true );
}

bool intersects( Interval *self, Interval *other )
{
  return intersection_endpoints( self, other )
      || intersection_overlap(   self, other )
      || intersection_overlap(  other, self  );
}

//
// Combining
//

void icombine( Interval *self, Interval *other )
{
  // copy lowest of the two lower bounds into ours
  if ( bcompare( self->lower, other->lower ) != Lesser )
  {
    bcombine( self->lower, other->lower );
  }

  // copy the higher of the bounds into ours
  // we must check both the lower and upper bounds of the
  // other interval, because the lower bound might be greater
  // than our upper bound
  if ( bcompare( self->upper, other->upper ) != Greater )
  {
    bcombine( self->upper, other->upper );
  }
  else if ( bcompare( self->upper, other->lower ) != Greater )
  {
    bcombine( self->upper, other->upper );
  }
}

