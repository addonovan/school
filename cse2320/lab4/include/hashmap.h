#ifndef LAB4_HASHMAP_H
#define LAB4_HASHMAP_H

#include <stdbool.h>

//
// Typedefs
//

typedef struct HashMapEntry {

  void* key;
  void* value;
  
  int hash;

  bool in_use;

} HashMapEntry;

typedef struct HashMap {

  int capacity;
  int size;
  
  // I randomly started getting segfaults because this value would randomly
  // mutate to some other address (which I, of course, don't have access to)
  // despite this never being modified after the constructor.
  // Moving it down here fixes everything. And if you think it's a buffer
  // overrun, it probably isn't considering that capacity and size are not
  // affected at all after doing this
  // It would always be reassigned to 0x1ccf3adb2, regardless of original value,
  // and this has never happend to capacity or size yet.
  // Also the stack trace put it at the very first get in main.c
  HashMapEntry* items;

  int ( *hash )( void* );

} HashMap;

//
// Functions
//

HashMap* hm_create( int capacity, int ( *hash )( void* ) );

void* hm_put( HashMap*, void* key, void* value );

void* hm_get( HashMap*, void* key );

//
// Hashing functions
//

int hash_string( void* data );

int hash_int( void* data );

#endif

