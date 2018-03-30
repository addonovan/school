#include <edge.h>
#include <stdlib.h>
#include <defs.inl>

EdgePool* edpool_create( int size )
{
  EdgePool* this = malloc( sizeof( EdgePool ) );

  $.size = size;
  $.next = 0;
  $.edges = calloc( $.size, sizeof( Edge ) );

  return &$;
}

Edge* edpool_new( EdgePool* this, int head, int tail, int weight )
{
  if ( $.size <= $.next )
  {
    return NULL;
  }

  Edge* edge = $.edges + $.next++;
  ed_init( edge, head, tail, weight );
  return edge;
}

void ed_init( Edge* this, int head, int tail, int weight )
{
  $.head = head;
  $.tail = tail;
  $.weight = weight;
}

int ed_compare( const void* a, const void* b )
{
  // <0 => a before b
  // =0 => a or     b
  // >0 => a after  b

  return ( ( Edge* ) a )->weight - ( ( Edge* ) b )->weight;
}

