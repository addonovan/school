	.global _start

_start:

  mov r0, #0   @ r0 = 0
  mov r1, #10  @  x = 10

  /* if ( x > 5 ) */
  cmp r1, #5
  ble cmp_x_lt_5_after
  @{
    add r0, r0, #1 @ r0 += 1
  @}

  cmp_x_lt_5_after:

  /* if ( x >= 10 ) */
  cmp r1, #10
  blt cmp_x_lt_10
  @{
    add r0, r0, #5 @ r0 += 5
    bal cmp_x_10_after
  @}
  /* else => x < 10 */
  cmp_x_lt_10:
  @{
    add r0, r0, #3 @ r0 += 3
  @}

  cmp_x_10_after:
  
  /* if ( 9 < x && x <= 10 )
   * where x is an int is the same as
   * if ( x == 10 )
   */
  cmp r1, #10
  bne _exit /* skip to end if condition broken */
  @{
    add r0, r0, #7  @ r0 += 7
  @}

  /* r0 should be 0 + 1 + 5 + 7 = 13 */

_exit:
  mov r7, #1
  swi 0
