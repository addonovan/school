
#ifndef FRACTION_H
#define FRACTION_H

typedef enum Comparison
{
  Lesser = -1,
  Equal = 0,
  Greater = 1
} Comparison;

typedef enum bool { true = 1, false = 0 } bool;

typedef struct Fraction
{
  int num;
  int den;
} Fraction;

/**
 * Compares the two fractions. Returns either `Lesser`,
 * `Equal`, or `Greater`, depending on `self`'s relation
 * to `other`.
 */
Comparison fcompare( Fraction *self, Fraction *other );

/**
 * Reduces the fraction to its lowest form.
 */
void reduce( Fraction *self );

#endif

