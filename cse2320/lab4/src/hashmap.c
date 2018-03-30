#include <hashmap.h>
#include <defs.inl>
#include <prime.h>

HashMap* hm_create( int capacity, int ( *hash )( void* ) )
{
  HashMap* this = malloc( sizeof( HashMap ) );

  // everything allocates to NULL or false, so it's all okay
  $.capacity = next_prime( capacity );
  $.items = calloc( $.capacity, sizeof( HashMapEntry ) );
  $.size = 0;
  $.hash = hash;

  return this;
}

int hashcode( int m, int key, int i )
{
  return
    (
      ( key % m )
      + i * ( ( 1 + key % ( m - 1 ) ) )
    ) % m;
}

void* hm_put( HashMap* this, void* key, void* value )
{
  int hash = $.hash( key );

  int i;
  for ( i = 0; i < $.capacity; i++ )
  {
    int hash_n = hashcode( $.capacity, hash, i );
    HashMapEntry item = $.items[ hash_n ];

    if ( !item.in_use || ( item.in_use && item.hash == hash ) )
    {
      void* previous = item.in_use ? item.value : NULL;
      if ( !item.in_use ) $.size++;

      item.in_use = true;
      item.key = key;
      item.value = value;
      item.hash = hash;
      $.items[ hash_n ] = item;
      return previous;
    }
  }

  // oh no :(
  printf( "[ERROR] O noes :(\n" );
  printf( "[ERROR] There were more edge names than expected\n" );
  printf( "[ERROR] (Even more than the nearest prime could compensate for)\n" );
  fflush( stdout );
  exit( 1 );
  return NULL;
}

void* hm_get( HashMap* this, void* key )
{
  int hash = $.hash( key );

  int i;
  for ( i = 0; i < $.capacity; i++ )
  {
    int hash_n = hashcode( $.capacity, hash, i );
    if ( !$.items[ hash_n ].in_use ) continue;
    if ( $.items[ hash_n ].hash != hash ) continue;

    return $.items[ hash_n ].value;
  }

  return NULL;
}

int hash_string( void* data )
{
  char* in = ( char* ) data;

  int hash = 0;
  for ( ; in[ 0 ] != '\0'; ++in )
  {
    hash += in[ 0 ] << in[ 1 ];
  }
  return hash;
}

int hash_int( void* data )
{
  return ( intptr_t ) data;
}

