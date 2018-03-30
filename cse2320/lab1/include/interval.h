
#ifndef INTERVAL_H
#define INTERVAL_H

#include "bound.h"

/**
 * An interval with two bounds.
 */
typedef struct Interval
{

  Bound *lower;
  Bound *upper;

} Interval;

/**
 * Initializes the interval at the given memory address
 * with the given properties.
 *
 * @param interval
 *        The address to initialize a new interval in.
 *
 * @param low_num
 *        The lower bound's numerator (this should be negative to
 *        denote an open bound).
 * @param low_den
 *        The lower bound's denominator.
 *
 * @param up_num
 *        The upper bound's numerator (this should be negative to
 *        denote an open bound).
 * @param up_den
 *        The upper bound's denominator.
 */
void init( Interval *interval, int low_num, int low_den, int up_num, int up_den );

/**
 * Prints the interval in the following form:
 * (-)low_num low_den (-)up_num up_den
 *
 * @param self
 *        The interval to print.
 */
void print_interval( Interval *self );

/**
 * Compares this interval with another one.
 *
 * @param self
 *        Our interval.
 * @param other
 *        The interval to compare self with.
 * 
 * @return
 *      Lesser:  if `self` should come before `other` when
 *               ordering
 *      Greater: if `self` should come after `other` when
 *               ordering
 *      Equal:   if the order `self` and `other` appear in
 *               wouldn't matter.
 */
Comparison icompare( Interval *self, Interval *other );

/**
 * @return `true` if `self` and `other` have an intersecting
 *      bounds. Or, `true`, if the bounds should be combined. 
 */
bool intersects( Interval *self, Interval *other );

/**
 * Combines the two intervals and copies the lowest bound
 * into `self->lower` and the largest bound into `self->upper`
 */
void icombine( Interval *self, Interval *other );

#endif

