	.global _start

_start:
  ldr r1, =array

  /* r2 will be the max, r3 will be the min */
  ldr r2, [ r1 ]
  ldr r3, [ r1 ], #4

  /* r4 will be the number of eements we've visited */
  mov r4, #1

  loop:

    /* r5 will be the temporary variable */
    ldr r5, [ r1 ], #4

    cmp r2, r5
    bge test_min

    /* we found a number bigger than r2 */
    mov r2, r5

  test_min:
    
    cmp r3, r5
    ble loop_condition
    /* we found a number smaller than r3 */
    mov r3, r5
    
  loop_condition:
    add r4, #1
    cmp r4, #15
    blt loop
  
  /* we're out of the loop  */
  add r0, r2, r3

_exit:
  mov r7, #1
  swi 0

/*****************************************************************************/
/* Data Section                                                              */
/*****************************************************************************/

	.data
array:
	.word 24, 0, 16, -21, 34, 13, 55, -9, 7, 64, 99, -15, 75, -20, 100

