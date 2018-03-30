
#ifndef BOUND_H
#define BOUND_H

#include "fraction.h"

typedef struct Bound
{
  Fraction *frac;
  bool closed;
} Bound;

/**
 * Compares the two bounds and returns whether `self` comes
 * before (`Lesser`), after (`Greater`), or at the same time
 * (`Equal`).
 */
Comparison bcompare( Bound *self, Bound *other );

/**
 * "Combines" the two bounds. It does a comparison to check
 * if the `frac` components are equal, if so `closed` will be
 * logically order between the two, otherwise `other`'s data
 * will be copied to `self`.
 */
void bcombine( Bound *self, Bound *other );

#endif

