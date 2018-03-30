	.global _start

_start:
  mov r1, #13       /* a: r1 = 13  */
  mov r2, #0b110    /* b: r2 = 6   */
  
  sub r3, r1, r2    /* c: r3 = 7   */  

  mov r4, #0x14
  mul r4, r3, r4    /* d: r4 = 140 */

  lsr r5, r4, #2    /* e: r5 = 35  */

  mov r0, r5        /* f: r0 = 35  */

_exit:
  mov r7, #1
  swi 0

