#ifndef LAB4_EDGES_H
#define LAB4_EDGES_H

typedef struct Edge
{

  int head;
  int tail;

  int weight;

} Edge;

typedef struct EdgePool
{

  Edge* edges;

  int size;
  int next;

} EdgePool;

EdgePool* edpool_create( int size );

Edge* edpool_new( EdgePool*, int head, int tail, int weight );

void ed_init( Edge*, int head, int tail, int weight );

int ed_compare( const void*, const void* );

#endif

