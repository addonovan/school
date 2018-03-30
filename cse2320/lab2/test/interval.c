#include <stdlib.h>
#include <assert.h>
#include "interval.h"
#include "scheduler.h"

/**
 * Runs unit tests on all the interval functions
 */
void testIntervals();


int main()
{
  testIntervals();
}

//
// Implementation
//

void testIntervals()
{
  Interval* intervals = malloc( 2 * sizeof( Interval ) );

  // [================]
  // [================]
  iinit( intervals + 0,   1, 5,   0 );
  iinit( intervals + 1,   1, 5,   0 );
  assert( iconflicts( intervals + 0, intervals + 1 ) == true );

  // [==============]
  //                [==============]
  iinit( intervals + 0,   1, 5,   0 );
  iinit( intervals + 1,   5, 7,   0 );
  assert( iconflicts( intervals + 0, intervals + 1 ) == false );

  // [===========]
  //      [==========]
  iinit( intervals + 0,   1, 5,   0 );
  iinit( intervals + 1,   3, 7,   0 );
  assert( iconflicts( intervals + 0, intervals + 1 ) == true );

  // [===========]
  //                 [=======]
  iinit( intervals + 0,   1, 5,   0 );
  iinit( intervals + 1,   6, 9,   0 );
  assert( iconflicts( intervals + 0, intervals + 1 ) == false );

  free( intervals );

  // test linking algorithm
  intervals = malloc( 6 * sizeof( Interval ) );

  // 1: [===]
  // 2:    [===]
  // 3:        [===]
  // 4:          [===]
  // 5:              [===]
  // 5 -> 4 -> 3 -> 2 -> 1
  iinit( intervals + 0,   0, 0,   0 );
  iinit( intervals + 1,   1, 2,   0 );
  iinit( intervals + 2,   2, 3,   0 );
  iinit( intervals + 3,   3, 4,   0 );
  iinit( intervals + 4,   4, 5,   0 );
  iinit( intervals + 5,   5, 6,   0 );
  icreatelinks( intervals, 6 );
  assert( intervals[ 0 ].prior == 0 );
  assert( intervals[ 1 ].prior == 0 );
  assert( intervals[ 2 ].prior == 1 );
  assert( intervals[ 3 ].prior == 2 );
  assert( intervals[ 4 ].prior == 3 );
  assert( intervals[ 5 ].prior == 4 );

  // 1: [===]
  // 2:   [===]
  // 3:     [===]
  // 4:       [===]
  // 5:           [===]
  // 5->4->2
  // 3->1
  iinit( intervals + 0,   0, 0,   0 );
  iinit( intervals + 1,   0, 2,   0 );
  iinit( intervals + 2,   1, 3,   0 );
  iinit( intervals + 3,   2, 4,   0 );
  iinit( intervals + 4,   3, 5,   0 );
  iinit( intervals + 5,   5, 7,   0 );
  icreatelinks( intervals, 6 );
  assert( intervals[ 0 ].prior == 0 );
  assert( intervals[ 1 ].prior == 0 );
  assert( intervals[ 2 ].prior == 0 );
  assert( intervals[ 3 ].prior == 1 );
  assert( intervals[ 4 ].prior == 2 );
  assert( intervals[ 5 ].prior == 4 );

  free( intervals );
}
