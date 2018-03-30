#include <edge.h>
#include <defs.inl>

int main()
{
  Edge* edges = calloc( 5, sizeof( Edge ) );
  int i;
  for ( i = 0; i < 5; i++ )
  {
    ed_init( edges + i, 1, 2, 5 - i ); 
  }

  printf( "Before sort:\n" );
  for ( i = 0; i < 5; i++ )
  {
    printf( "edge { head = %d, tail = %d, weight = %d }\n",
        edges[ i ].head,
        edges[ i ].tail,
        edges[ i ].weight
    );
  }

  qsort(
      edges,
      5,
      sizeof( Edge ),
      &ed_compare
  );

  printf( "After sort:\n" );
  for ( i = 0; i < 5; i++ )
  {
    printf( "edge { head = %d, tail = %d, weight = %d }\n",
        edges[ i ].head,
        edges[ i ].tail,
        edges[ i ].weight
    );
  }
}

