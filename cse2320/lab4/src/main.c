#include <hashmap.h>
#include <edge.h>
#include <defs.inl>
#include <stdint.h>
#include <graph.h>

HashMap* vertex_name_map = NULL;
HashMap* vertex_id_map = NULL;

EdgePool* edge_pool = NULL;

int next_vertex_index = 0;

int assign_index( char* name )
{
  void* id = hm_get( vertex_name_map, name );
  if ( id == NULL )
  {
    hm_put( vertex_name_map, name, generic( next_vertex_index ) );
    hm_put( vertex_id_map, generic( next_vertex_index ), name );

    return next_vertex_index++;
  }
  else
  {
    // also deallocate the current string, as it's not needed
    free( name );

    return ( intptr_t ) id;
  }
}

void read_edge()
{
  char* head_name = calloc( 26, sizeof( char ) );
  char* tail_name = calloc( 26, sizeof( char ) );
  int weight;

  scanf( "%25s %25s %d\n", head_name, tail_name, &weight );

  int head_id = assign_index( head_name );
  int tail_id = assign_index( tail_name );

  edpool_new( edge_pool, head_id, tail_id, weight );
}

int main()
{
  int vertices, edges;
  scanf( "%d %d\n", &vertices, &edges );

  vertex_name_map = hm_create( vertices, hash_string );
  vertex_id_map = hm_create( vertices, hash_int );  

  edge_pool = edpool_create( vertices );

  int i;
  for ( i = 0; i < edges; i++ )
  {
    read_edge( vertex_name_map, vertex_id_map );
  }

  if ( vertex_name_map->size < vertices )
  {
    printf( 
        "[WARNING] There are fewer vertices than expected "
        "(expected %d, found %d)\n",
        vertices,
        vertex_name_map->size
    );
  }
  else if ( vertex_name_map->size > vertices )
  {
    printf(
        "[ERROR] There were more vertices than expected "
        "(expected %d, found %d)\n",
        vertices,
        vertex_name_map->size
    );
    fflush( stdout );
    return 1;
  }

  Forest* forest = frst_create( vertex_name_map->size ); 
  frst_mst( forest, edge_pool->edges, edge_pool->size ); 
  frst_print( forest, vertex_id_map );
}

