#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "interval.h"

#define LOGGING 1
#define lassert( format, actual, expected ) \
        if ( LOGGING && actual != expected ) \
        { \
          printf( "Actual  != Expected\n" ); \
          printf( format " != " format "\n", actual, expected ); \
        } \
        assert( actual == expected )

int main()
{
  Interval *intervals = malloc( 5 * sizeof( Interval ) );

  //
  // Containment & Comparisons
  //

  init( intervals + 0,  1,  1,  5,  1 );
  init( intervals + 1, -1,  1,  5,  1 );
  init( intervals + 2,  2,  1,  5,  1 );
  init( intervals + 3,  1,  1,  4,  1 );
  init( intervals + 4,  1,  1,  5,  1 );

  // check that the bounds are properly understood
  lassert( "%d", ( intervals + 0 )->lower->frac->num, 1 );
  lassert( "%d", ( intervals + 0 )->lower->frac->den, 1 );
  lassert( "%d", ( intervals + 0 )->upper->frac->num, 5 );
  lassert( "%d", ( intervals + 0 )->upper->frac->den, 1 );

  // check that open bounds lose their negative signs
  lassert( "%d", ( intervals + 1 )->lower->frac->num, 1 );

  // check the the bounds are properly closed
  lassert( "%d", ( intervals + 0 )->lower->closed, true  );
  lassert( "%d", ( intervals + 1 )->lower->closed, false );

  // check that comparisons work when one interval occurs after another
  lassert( "%d", icompare( intervals + 0, intervals + 2 ), Lesser );

  // check that comparisons work when two intervals start at the same
  // point but don't end at the same point
  lassert( "%d", icompare( intervals + 0, intervals + 3 ), Greater );

  // check that comparisions works when two intervals are the same
  lassert( "%d", icompare( intervals + 0, intervals + 4 ), Equal );

  init( intervals + 0, -25, 2, -50, 1 );
  lassert( "%d", icompare( intervals + 0, intervals + 0 ), Equal );

  // delete the previous intervals
  free( intervals );


  //
  // Intersections
  //


  // create 2 new ones to test intersection
  intervals = malloc( 2 * sizeof( Interval ) );

  // [======]
  //            [==========]
  init( intervals + 0,  1, 1,  5, 1 );
  init( intervals + 1, 10, 1, 15, 1 );

  lassert( "%d", intersects( intervals + 0, intervals + 1 ), false );

  // [==============]
  //                [=================]
  init( intervals + 0,  1, 1, 15, 1 );
  init( intervals + 1, 15, 1, 30, 1 );

  lassert( "%d", intersects( intervals + 0, intervals + 1 ), true );

  // [==============]
  //     [===]
  init( intervals + 1,  5, 1, 10, 1 );

  lassert( "%d", intersects( intervals + 0, intervals + 1 ), true );

  // [=======)
  //         [======]
  init( intervals + 0,  1,  1,  -5, 1 );
  init( intervals + 1,  5,  1,  10, 1 );

  lassert( "%d", intersects( intervals + 0, intervals + 1 ), true );

  // (=======)
  //         (======)
  init( intervals + 0,  -1,  1,  -5,  1 );
  init( intervals + 1,  -5,  1,  -10, 1 );

  lassert( "%d", intersects( intervals + 0, intervals + 1 ), false );

  
  // (========)
  //                (========)    
  init( intervals + 0, -25, 2, -50, 1 );
  init( intervals + 1, -75, 1, -100, 1 );

  lassert( "%d", intersects( intervals + 0, intervals + 1 ), false );

  // (==========)
  // (==========)
  init( intervals + 0, -25, 2, -50, 1 );
  init( intervals + 1, -25, 2, -50, 1 );

  lassert( "%d", intersects( intervals + 0, intervals + 1 ), true );

  // [========]
  //      [========]
  init( intervals + 0, 1, 1, 25, 1 );
  init( intervals + 1, -25, 2, -50, 1 );

  lassert( "%d", intersects( intervals + 0, intervals + 1 ), true );


  //
  // Combining
  //


  free( intervals );
  intervals = malloc( 3 * sizeof( Interval ) );

  // [========]
  //      [========]
  // [=============]
  init( intervals + 0,  1,  1,   10,  1 );
  init( intervals + 1,  5,  1,   15,  1 );
  init( intervals + 2,  1,  1,   15,  1 );

  icombine( intervals + 0, intervals + 1 );
  lassert( "%d", icompare( intervals + 0, intervals + 2 ), Equal );

  // [========]
  //          (=========]
  // [==================]
  init( intervals + 0,   1, 1,  10, 1 );
  init( intervals + 1, -10, 1,  20, 1 );
  init( intervals + 2,   1, 1,  20, 1 );

  icombine( intervals + 0, intervals + 1 );
  lassert( "%d", icompare( intervals + 0, intervals + 2 ), Equal );

  return 0;
}

