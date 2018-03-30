#include <hashmap.h>
#include <string.h>
#include <defs.inl>
#include <stdio.h>
#include <stdint.h>

int main()
{
  HashMap* this = hm_create( 5, &hash_int );

  hm_put( &$, generic( 1 ), "hello world" );
  assert(
      strcmp( 
        hm_get( &$, ( void* ) 1 ),
        "hello world"
      ) == 0
  );

  hm_put( &$, ( void* ) 2, "a" );
  hm_put( &$, ( void* ) 3, "b" );
  hm_put( &$, ( void* ) 4, "c" );
  hm_put( &$, ( void* ) 5, "d" );

  int i;
  for ( i = 0; i < $.capacity; i++ )
  {
    HashMapEntry entry = $.items[ i ];
    printf( "[idx = %d] [key = %d] %s\n",
        i,
        ( int ) ( intptr_t ) entry.key,
        ( char* ) entry.value
    );
  }

  // test string->int
  printf( "======\n" );

  this = hm_create( 5, &hash_string );
  for ( i = 0; i < $.capacity; i++ )
  {
    char* string = calloc( 2, sizeof( char ) );
    string[ 0 ] = 'a' + i;
    hm_put( &$, string, generic( i ) );
  }

  for ( i = 0; i < $.capacity; i++ )
  {
    HashMapEntry entry = $.items[ i ];
    printf( "[idx = %d] [key = %s] %d\n",
        i,
        ( char* ) entry.key,
        ( int ) ( intptr_t ) entry.value
    );
  }

  printf( "------\n" );

  for ( i = 0; i < $.capacity; i++ )
  {
    char* string = calloc( 2, sizeof( char ) );
    string[ 0 ] = 'a' + i;
    printf( "{ key = %s, value = %d }\n",
        string,
        ( int ) ( intptr_t ) hm_get( &$, string )
    );
  }
}
