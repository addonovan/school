#include <stdlib.h>
#include <stdio.h>
#include "interval.h"
#include "scheduler.h"

/**
 * Prints the matrix from the given scheduler.
 *
 * @param this
 */
void print_matrix( Scheduler* this );

/**
 * Prints the room defined by the stack.
 * @param this
 */
void print_room( Stack* this );

#ifdef TESTING
// this is only here for my sample tests because my IDE doesn't support
// redirection yet
int test_main( const char* file )
{
  struct _IO_FILE* input = fopen( file, "r" );
#else
int main()
{
  struct _IO_FILE* input = stdin;
#endif

  // read the number of entries
  int count;
  fscanf( input, "%d", &count );
  printf( "%d\n", count );

  // allocate space for the intervals
  count++; // we need to account for the header node
  Interval* intervals = malloc( count * sizeof( Interval ) );
  iinit( intervals + 0, 0, 0, -1 );

  // initialize the intervals
  int start, end, weight;
  int i;
  for ( i = 1; i < count; i++ )
  {
    fscanf( input, "%d %d %d", &start, &end, &weight );
    iinit(  intervals + i,      start,  end,  weight );
    iprint( intervals + i );
  }

  /* do the calculation */
  Scheduler* scheduler = malloc( sizeof( Scheduler ) );
  sinit( scheduler, intervals, count );
  scalc( scheduler );
  sbacktrace( scheduler );

  // print the dp table
  print_matrix( scheduler );

  // print the rooms
  print_room( scheduler->room1 );
  print_room( scheduler->room2 );

  // print total weight
  printf( "%d\n", scheduler->value );

  // just let the OS clean up heap, all heap allocated resources get
  // used until this point anyways (well, except for the StackNodes, but
  // those are dealt with as they get popped)
  return 0;
}

void print_matrix( Scheduler* this )
{
  printf( "  i     j      M[i][j]   \n" );
  printf( "----- ----- -------------\n" );

  int i, j;
  for ( i = 0; i < $.size; i++ )
  {
    for ( j = 0; j < $.size; j++ )
    {
      printf( " %3d   %3d      %3d   \n", i, j, $.matrix[ i ][ j ] );
    }
  }
}

void print_room( Stack* this )
{
  Interval* interval;
  printf( "%d\n", $.count );
  while ( ( interval = stpop( &$ ) ) != NULL )
  {
    iprint( interval );
  }
}
