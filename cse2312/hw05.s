	.global _start

/*****************************************************************************/
/* Subroutines                                                               */
/*****************************************************************************/

/**
 * Converts a single ASCII character to its decimal equivalent.
 *
 * Inputs:
 * - r5: A single ASCII character
 *
 * Outputs:
 * - r6: The decimal value of the ASCII numeral, or -1 if the character was not
 *       a valid ASCII numeral.
 */
atoi:
  /* 
   * by default, set r6 to -1, we'll set this to the correct value if
   * r5 ends up being a valid ASCII numerical character
   */
  mov   r6, #-1

  /* < '0'? it's a bad value */
  cmp   r5, #'0'
  blt   atoi_return

  /* > '9'? it's a bad value */
  cmp   r5, #'9'
  bgt   atoi_return

  /* => it's a good value, so subtract '0' to get dec value */
  sub   r6, r5, #'0'

  /* return back to the loop */
  atoi_return:
    bx  r14

/*****************************************************************************/
/* Main Body                                                                 */
/*****************************************************************************/

_start:

  /*
   * r3 = loop counter (remaining iterations)
   * r4 = running sum (because r0 is taken until end)
   * r5 = current char
   * r6 = decimal value of r5 (-1 if not a number) 
   */
  mov r3, #5
  
  loop:
    /* if we've hit 0 items left, exit */
    cmp   r3, #0
    beq   _exit

    /* read 2 chars from stdin */
    mov   r7, #3
    mov   r0, #0
    ldr   r1, =input
    mov   r2, #2
    swi   0

    /* grab the first character from the input (ignore \n) */
    ldrb  r5, [r1]

    /* convert r5 to a decimal value (stored in r6) */
    bl    atoi   
    
    /* if it was an invalid character, don't count it against the user */
    cmp   r6, #0
    blt   loop

    /* if it was a correct value, then add it to the running sum */
    add   r4, r6

    /* we've run through an iteration, so head back to the top */
    sub   r3, #1
    bal   loop

_exit:
  /* copy the running sum to r0 */
  mov   r0, r4

  mov   r7, #1
  swi   0

/*****************************************************************************/
/* Data                                                                      */
/*****************************************************************************/

	.data
input:
	.space 2
