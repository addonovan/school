#ifndef MERGESORT_H
#define MERGESORT_H

#include "interval.h"

/**
 * Performs a merge sort.
 *
 * @param array The beginning of the array to sort.
 * @param lo The lower bound (offset in array)
 * @param hi The upper bound (offset in array)
 */
int mergesort( Interval* array, int lo, int hi );

/**
 * Mergest he two sections of the array
 *
 * @param l the beginning of the left hand side
 * @param l_size the number of elements in the left hand size
 *
 * @param r the beginning of the right hand side
 * @param r_size the number of elemnets in the right hand size
 */
int merge( Interval* array, int l, int l_size, int r, int r_size );

#endif

