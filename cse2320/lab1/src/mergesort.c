#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mergesort.h"

int mergesort( Interval *array, int lo, int hi )
{
  int count = hi - lo;

  if ( count <= 1 )
  {
    return 1;
  }

  int mid = ( lo + hi ) / 2;

  int count_lo = mergesort( array,  lo, mid );
  int count_hi = mergesort( array, mid,  hi );

  count = merge( array, lo, count_lo, mid, count_hi );
  
  return count;
}

void copymerge( int *count, Interval *top, Interval* next )
{
  if ( intersects( top, next ) )
  {
    icombine( top, next );
  }
  else
  {
    *( top + 1 ) = *next;
    *count += 1;
  }
}

int merge( Interval *array, int l, int l_size, int r, int r_size )
{
  Interval *left = malloc( l_size * sizeof( Interval ) );
  memcpy( left, array + l, l_size * sizeof( Interval ) );

  Interval *right = malloc( r_size * sizeof( Interval ) );
  memcpy( right, array + r, r_size * sizeof( Interval ) );

  int l_i = 0;
  int r_i = 0;

  int index = 0;

  // first time, just copy the element into the first place
  switch ( icompare( left, right ) )
  {
    // I could break this down with another level like
    // inside the loop, but I got lazy at this point
    case Lesser:
    case Equal:
      *( array + l ) = *left;
      l_i++;
      break;

    case Greater:
      *( array + l ) = *right;
      r_i++;
      break;
  }

  while ( l_i < l_size && r_i < r_size )
  {
    Interval *tmp;
    switch ( icompare( left + l_i, right + r_i ) )
    {
      case Lesser:
        tmp = left + l_i;
        l_i++;
        break;

      case Greater:
        tmp = right + r_i;
        r_i++;
        break;

      case Equal:
        tmp = left + l_i;
        l_i++;
        r_i++;
        break;
    }

    copymerge( &index, array + l + index, tmp );
  }

  while ( l_i < l_size )
  {
    copymerge( &index, array + l + index, left + l_i );
    l_i++;
  }

  while ( r_i < r_size )
  {
    copymerge( &index, array + l + index, right + r_i );
    r_i++;
  }

  return index + 1;
}


