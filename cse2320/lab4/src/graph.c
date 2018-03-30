#include <graph.h>
#include <defs.inl>

Forest* frst_create( int vertex_count )
{
  Forest* this = malloc( sizeof( Forest ) );

  $.edges = NULL;
  $.edge_count = -1;
  $.mst_weight = 0;

  $.weights = calloc( vertex_count, sizeof( int ) );
  $.tree_count = vertex_count;

  $.parents = calloc( vertex_count, sizeof( int ) );

  // put every node in their own location
  int i;
  for ( i = 0; i < $.tree_count; i++ )
  {
    $.parents[ i ] = i;
  }

  return &$;
}

int frst_find( Forest* this, int vertex_num )
{
  int i, j, root;

  for ( i = vertex_num - 1; $.parents[ i ] != i; i = $.parents[ i ] );
  root = i;

  for ( i = vertex_num - 1; $.parents[ i ] != i; i = j )
  {
    j = $.parents[ i ];
    $.parents[ i ] = root;
  }

  return root;
}

void frst_union( Forest* this, int root1, int root2 )
{
  if ( $.weights[ root1 ] > $.weights[ root2 ] )
  {
    $.parents[ root2 ] = root1;
    $.weights[ root1 ] += $.weights[ root2 ];
  }
  else
  {
    $.parents[ root1 ] = root2;
    $.weights[ root2 ] += $.weights[ root1 ];
  }
  $.tree_count--;
}

void frst_mst( Forest* this, Edge* edges, int edge_count )
{
  $.edges = edges;
  $.edge_count = edge_count;

  // first, sort the edges least -> greatest
  qsort(
      edges,
      edge_count,
      sizeof( Edge ),
      &ed_compare
  );

  // compress the edges so long as the edges aren't part of the same tree
  int i;
  for ( i = 0; i < edge_count; i++ )
  {
    int a = frst_find( &$, edges[ i ].tail );
    int b = frst_find( &$, edges[ i ].head );
    
    // mark this edge as deleted
    if ( a == b )
    {
      edges[ i ].tail = edges[ i ].head = -1;
      continue;
    }

    $.mst_weight += edges[ i ].weight;
    frst_union( &$, a, b );
  }
}

void frst_print( Forest* this, HashMap* id_map )
{
  // this is pretty garbage

  printf( "Sum of spanning edges: %d\n", $.mst_weight );

  // we're going to use the hashmap as if it didn't have any actual data in the
  // keys (i.e. use it like a hashset)
  HashMap* set = hm_create( $.tree_count, &hash_int ); 

  int i, j;
  int current;

  // make sure we get every subtree
  for ( i = 0; i < $.tree_count; i++ )
  {
    current = -1;
    for ( j = 0; j < $.edge_count; j++ )
    {
      // if this edge was deleted, then skip it
      if ( ( $.edges[ j ].head & $.edges[ j ].tail ) == -1 ) 
        continue;

      int root = frst_find( &$, $.edges[ j ].tail );

      if ( hm_get( set, generic( root ) ) != NULL )
      {
        // if the tail doesn't work, then try the head
        root = frst_find( &$, $.edges[ j ].head ); 

        // if that was already in the set too, then skip this element
        if ( hm_get( set, generic( root ) ) != NULL ) continue;
      }

      // we found a new root
      current = root;
      hm_put( set, generic( root ), ( void* ) true );
      break;
    }
    
    // find all members of this new root
    printf( "==== [ New Tree ] ====\n" );
    for ( ; j < $.edge_count; j++ )
    {
      // if this edge was deleted, then skip it
      if ( ( $.edges[ j ].head & $.edges[ j ].tail ) == -1 ) continue;

      int root = frst_find( &$, $.edges[ j ].tail );
      
      // check to see if the root of the tail-end is in our set
      if ( root != current )
      {
        // check to see if the root of the head-end is in our set
        root = frst_find( &$, $.edges[ j ].head );

        // neither are, so go onto next
        if ( root != current ) continue;
      }

      // we found a member of this tree!
      
      // let's get its name from the map
      char* tail_name = hm_get( id_map, generic( $.edges[ j ].tail ) ); 
      char* head_name = hm_get( id_map, generic( $.edges[ j ].head ) );
      int weight = $.edges[ j ].weight;

      printf( "%s %s %d\n", head_name, tail_name, weight );
    }
    printf( "======================\n" );
  }
}

