#include <stdbool.h>

#ifndef INTERVAL_H
#define INTERVAL_H

#define $ ( *this )

/**
 * An interval in the problem.
 */
typedef struct Interval
{
    /** start time */
    int start;
    /** end time */
    int end;
    /** weight of the interval (larger is better) */
    int weight;

    /** The greatest previous interval's index (set by icreatelinks) */
    int prior;
} Interval;

/**
 * Initializes an interval with the start, end, and weight values.
 *
 * @param this
 * @param start
 *          The interval's start time.
 * @param end
 *          The interval's end time.
 * @param weight
 *          The weight of the interval.
 */
void iinit( Interval* this, int start, int end, int weight );

/**
 * Checks if this interval intersects with the other.
 *
 * Assumptions:
 * 1) `this` MUST be guaranteed to start before or at the same time as `other`
 *
 * @param this
 * @param other
 *          The other interval to check.
 * @return `true` if there's an intersection, `false` if there isn't.
 */
bool iconflicts( Interval* this, Interval* other );

/**
 * Creates links between the intervals in the array
 *
 * @param intervals
 *          The intervals
 * @param count
 *          The number of intervals.
 */
void icreatelinks( Interval* intervals, int count );

/**
 * Prints this interval to the console.
 * @param this
 */
void iprint( Interval* this );

#endif
