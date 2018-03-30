#include <stdio.h>
#include <stdlib.h>
#include "interval.h"
#include "mergesort.h"

int main()
{
  int input_count;
  scanf( "%d", &input_count );

  Interval *array = malloc( input_count * sizeof( Interval ) );
  int i;
  for ( i = 0; i < input_count; i++ )
  {
    int a, b, c, d;
    scanf( "%d %d %d %d", &a, &b, &c, &d );

    init( array + i, a, b, c, d );
  }

  int count = mergesort( array, 0, input_count );

  printf( "%d\n", count );

  for ( i = 0; i < count; i++ )
  {
    print_interval( array + i );
  }

  return 0;
}

