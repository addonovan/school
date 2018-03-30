#include <prime.h>
#include <assert.h>

int main()
{
  assert( next_prime( 2 ) == 2 );
  assert( next_prime( 3 ) == 3 );
  assert( next_prime( 4 ) == 5 );
  assert( next_prime( 9 ) == 11 );
  assert( next_prime( 49 ) == 53 );
}

