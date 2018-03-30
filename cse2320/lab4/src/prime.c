#include <prime.h>
#include <stdbool.h>

int isqrt( int i )
{
  // uses binary search to find the square root

  int lower = 1;
  int upper = i;
  int mid = ( lower + upper ) / 2;

  while ( lower < upper )
  {
    int square = mid * mid;

    if ( square == i )
    {
      break;
    }
    else if ( square < i )
    {
      lower = mid + 1;
    }
    else
    {
      upper = mid - 1;
    }

    mid = ( lower + upper ) / 2;
  }

  // this will be the closest thing we have to the square root
  return mid;
}

bool is_prime( int n )
{
  if ( n == 1 ) return false;
  if ( n <= 3 ) return true;
  if ( n % 2 == 0 ) return false;

  // small primes
  if ( n == 5 || n == 7 || n == 11 || n == 13 || n == 17 )
  {
    return true;
  }

  int i;
  for ( i = 3; i <= isqrt( n ); i += 2 )
  {
    if ( n % i == 0 )
    {
      return false;
    }
  }

  return true;
}

int next_prime( int n )
{
  // oh boy, I love side effects!
  while ( !is_prime( n++ ) );

  return n - 1;
}

