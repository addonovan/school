#include <stdlib.h>
#include "scheduler.h"

// apparently gcc 4.1.2 doesn't like binary literals, so we're gonna have to do it manually
// also, if this isn't a 32 bit int, then, hey, we made it more flexible.
// (unsigned int so there's no warning about integer overflow)
const unsigned int ACTION_MASK =   1u << ( 8 * sizeof( int ) - 1 );
const unsigned int VALUE_MASK  = ( 1u << ( 8 * sizeof( int ) - 1 ) ) - 1;

void sinit( Scheduler* this, Interval* intervals, int count )
{
  // copy the values
  $.size = count;
  $.intervals = intervals;
  icreatelinks( intervals, count );

  // allocate space for the matrix
  $.matrix = calloc( ( size_t ) count, sizeof( int* ) );
  int i;
  for ( i = 0; i < count; i++ )
  {
    $.matrix[ i ] = calloc( ( size_t ) count, sizeof( int ) );
  }

  // default the coordinates out to zero
  $.i = $.j = 0;

  // initialize our linked lists
  $.room1 = malloc( sizeof( Stack ) );
  $.room2 = malloc( sizeof( Stack ) );

  stinit( $.room1 );
  stinit( $.room2 );

  $.value = -1;
}

int scost( Scheduler* this )
{
  if ( $.i == $.j )
  {
    return $.i == 0 ? 0 : $.matrix[ $.i - 1 ][ $.j ];
  }

  int weight_included, weight_excluded;

  if ( $.i > $.j )
  {
    Interval interval = $.intervals[ $.i ];

    // solve 1D problem along the i axis
    weight_included = $.matrix[ interval.prior ][ $.j ] + interval.weight;
    weight_excluded = $.matrix[ $.i - 1 ][ $.j ];
  }
  // => $.i < $.j
  else
  {
    Interval interval = $.intervals[ $.j ];

    weight_included = $.matrix[ $.i ][ interval.prior ] + interval.weight;
    weight_excluded = $.matrix[ $.i ][ $.j - 1 ];
  }

  // default to the value if it were excluded
  int val = weight_excluded;

  // if we should've included it, note that by adding the ACTION_MASK bit
  if ( weight_included >= weight_excluded )
  {
    val = weight_included | ACTION_MASK;
  }

  return val;
}

void scalc( Scheduler* this )
{
  for ( $.i = 0; $.i < $.size; $.i++ )
  {
    for ( $.j = 0; $.j < $.size; $.j++ )
    {
      $.matrix[ $.i ][ $.j ] = scost( this ) & VALUE_MASK;
    }
  }

  $.value = $.matrix[ $.size - 1 ][ $.size - 1 ];
}

void sbacktrace( Scheduler* this )
{
  $.i = $.j = $.size - 1;
  int running_weight = 0;

  while ( $.i >= 0 && $.j >= 0 )
  {
    // if they're the same, just go up
    if ( $.i == $.j )
    {
      $.i--;
    }
    // if we're in the top half, then add it to room 1 (if it should be)
    else if ( $.i < $.j )
    {
      int weight = scost( this );

      // REEEEEE ORDER OF OPERATIONS WAS VERY ANNOYING HERE
      if ( ( weight & ACTION_MASK ) == 0 )
      {
        $.j--;
      }
      else
      {
        running_weight += $.intervals[ $.j ].weight;

        stpush( $.room2, $.intervals + $.j );
        $.j = $.intervals[ $.j ].prior;
      }
    }
    // => $.i > $.j
    // if we're int he bottom half, add it to room 2 (if it should be)
    else
    {
      int weight = scost( this );

      // 0 => don't add it, go up a row instead
      if ( ( weight & ACTION_MASK ) == 0 )
      {
        $.i--;
      }
        // else => add it to room1, and go to prior value
      else
      {
        running_weight += $.intervals[ $.i ].weight;

        stpush( $.room1, $.intervals + $.i );
        $.i = $.intervals[ $.i ].prior;
      }
    }
  }
}
