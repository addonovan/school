	.global _start

/*
 * Registers:
 * r0 - the max row sum
 * r1 - the current index in the matrix
 * r2 - a row's sum
 * r7 - current int in matrix
 * r8 - inner loop counter (j)
 * r9 - outer loop counter (i)
 */

/******************************************************************************/
/* Subroutines                                                                */
/******************************************************************************/

/**
 * Sums up the four next values in the array.
 * 
 * Requirements:
 * - r1 is the current index in the matrix
 *
 * Side effects:
 * - r2 has the row's sum
 * - r7 will be the last value from the row
 * - r8 will be the row's length
 */
row_sum:
  mov r8, #0  /* iteration count */
  mov r2, #0  /* running sum */

  row_sum_loop:

    /* get the next int from the array */
    ldr r7, [ r1 ], #4

    /* r2 += r7 */
    add r2, r7

    /* r8++ */
    add r8, #1

    /* loop while r8 < 4 */
    cmp r8, #4
    blt row_sum_loop

  /* jump back to previous location */
  bx r14

/******************************************************************************/
/* Main                                                                       */
/******************************************************************************/

_start:
  /* get the matrix's start */
  ldr r1, =matrix

  /* set the current max to be the first row's sum */
  bl row_sum
  mov r0, r2

  /* The row counter */
  /* set to 1 because we've already read one row off */
  mov r9, #1

  /* perform the row sum for each row  */
  loop:
    /* loop until r9 >= 8 */
    cmp r9, #8
    bge _exit

    /* calculate the sum of the row */
    bl row_sum

    /* we've run through another iteration */
    add r9, #1

    /* is new row sum > current max? */
    cmp r2, r0
    ble loop

    /* copy larger value in r0 */
    mov r0, r2
    bal loop

_exit:
  mov r7, #1
  swi 0

/******************************************************************************/
/* Data                                                                       */
/******************************************************************************/

	.data

matrix:
    .word 12,   10,    3,    0 /*   25   */
    .word  5,    0,   16,   22 /*   43   */    
    .word  7,   15,    0,   10 /*   32   */
    .word  7,    8,    9,   10 /*   34   */
    .word  2,    3,    5,    7 /*   17   */
    .word 11,   13,   17,   19 /* [ 60 ] */
    .word  5,   10,   15,   20 /*   50   */
    .word  1, 	 2,    3,   53 /*   59   */

