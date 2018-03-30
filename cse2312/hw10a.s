	.global _start

/******************************************************************************/
/* M A I N                                                                    */
/******************************************************************************/

_start:
  /*
   * Registers
   * r1  Current position in input string
   * r2  Current position in output string
   * r3  Current character from input string
   * r4  Nibble from r3 => character for r2
   */

  ldr r1, =input
  ldr r2, =output

  loop:
    
    /* get the next character from the string */
    ldrb r3, [ r1 ], #1

    /* if the character is a null-terminator, then we're done */
    cmp  r3, #0
    beq  _exit

    /* process the first nibble */
    and  r4, r3, #0xF0
    /* shift nibble over so it represents the correct value */
    lsr  r4, #4
    add  r4, #'0'
    strb r4, [ r2 ], #1

    /* process the second nibble */
    and  r4, r3, #0x0F
    /* no lsr, it's already in the correct position */
    add  r4, #'0'
    strb r4, [ r2 ], #1

    bal loop

_exit:
  /* print the output string */
  mov r7, #4
  mov r0, #1
  mov r2, #17 /* 16 chars + trailing newline */
  ldr r1, =output
  swi 0

  /* quit */
  mov r7, #1
  swi 0

/******************************************************************************/
/* D A T A                                                                    */
/******************************************************************************/

	.data

input:
	.asciz "aqi$pd8U"

output:
	.space 16
	.ascii "\n"

