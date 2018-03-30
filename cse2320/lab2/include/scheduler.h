#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "interval.h"
#include "stack.h"

typedef struct Scheduler
{
    /** The number of intervals, and the size of our matrix. This INCLUDES the header interval. */
    int size;

    /** An array of intervals */
    Interval* intervals;
    /** the DP Table/Matrix we fill during  */
    int** matrix;

    /** the current coordinates in the matrix */
    int i, j;

    /** A stack of the intervals to go in room1 (call sbacktrace first) */
    Stack* room1;
    /** A stack of the intervals to go in room2 (call sbacktrace first) */
    Stack* room2;

    /** The value we reached after filling the table. (bottom-right most value in matrix) */
    int value;

} Scheduler;

/**
 * The Value Mask, perform a bitwise and with this value to get the numeric
 * value from the cost function.
 */
const unsigned int VALUE_MASK;

/**
 * The Action Mask, perform a bitwise and with this value to get the action enum
 * denoting what to do.
 */
const unsigned int ACTION_MASK;

/**
 * Initializes an allocated scheduler object.
 *
 * @param this
 * @param intervals
 *          The intervals for this scheduler to contain.
 * @param size
 *          The number of intervals
 */
void sinit( Scheduler* this, Interval* intervals, int size );

/**
 * Computes the cost function of the given interval. The leftmost bit will denote the
 * [Action] taken to get the cost. This should be &'d with VALUE_MASK to get the raw
 * number back.
 *
 * @return The weight of best option for the current position in the scheduler.
 */
int scost( Scheduler* this );

/**
 * Calculates the ideal intervals for each room
 */
void scalc( Scheduler* this );

/**
 * Backtraces through the matrix and finds the intervals that should be added.
 *
 * @param this
 */
void sbacktrace( Scheduler* this );

#endif
