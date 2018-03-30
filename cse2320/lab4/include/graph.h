#ifndef LAB4_GRAPH_H
#define LAB4_GRAPH_H

#include <hashmap.h>
#include <edge.h>

typedef struct Forest
{
  
  int* parents;
  int* weights;

  int tree_count;

  Edge* edges;
  int edge_count;
  int mst_weight;

} Forest;

Forest* frst_create( int vertex_count );

int frst_find( Forest*, int vertex_num );

void frst_union( Forest*, int root1, int root2 );

void frst_mst( Forest*, Edge* edges, int edge_count );

void frst_print( Forest*, HashMap* id_map );

#endif

