	.global _start

/*
 * Registers:
 * r0-r7  are general purpose
 * r8-r11 subroutine parameters/returns
 */

/******************************************************************************/
/* S U B R O U T I N E S                                                      */
/******************************************************************************/

/**
 * Calculates the GCD of parameters a and b.
 *
 * Inputs:
 *  r8  a
 *  r9  b
 *
 * Outputs:
 *  r8  the GCD of a and b
 *  r9  the GCD of a and b
 */
gcd:
  cmp r8, r9

  /* a == b? we're done */
  bxeq  lr

  /* a > b => a = a - b */
  subgt r8, r9

  /* b > a => b = b - a */
  sublt r9, r8

  /* loop until condition is met (don't affect lr) */
  bal gcd

/**
 * Converts a single digit, positive number to its ASCII character form
 *
 * Inputs:
 *   r8  The number
 *
 * Outputs:
 *   r8  The character representing the number
 */
itoa:
  add r8, #'0'
  bx lr

/**
 * Prints a given ASCII character value
 *
 * Inputs:
 *   r8  The charcter to print
 *
 * Outputs:
 *   None
 */
print:
  /* preserve outside values */
  stmfd sp!, { r0-r7 }


  /* print the output string */
  mov   r7, #4
  mov   r0, #1
  mov   r2, #2 /* 1 char + '\n' */
  ldr   r1, =output

  /* write the character to the string */
  strb  r8, [ r1 ]

  /* actually print the string */
  swi   0

  /* reset outside register values */
  ldmfd sp!, { r0-r7 }
  bx    lr

/******************************************************************************/
/* M A I N                                                                    */
/******************************************************************************/

_start:
  mov r8, #24
  mov r9, #18
  bl  gcd
  bl  itoa
  bl  print

  mov r8, #78
  mov r9, #34
  bl  gcd
  bl  itoa
  bl  print

  mov r8, #99
  mov r9, #36
  bl  gcd
  bl  itoa
  bl  print

_exit:
  mov r7, #1
  swi 0

/******************************************************************************/
/* D A T A                                                                    */
/******************************************************************************/
	
        .data

output:
	.space 1    /* the character to print */
        .ascii "\n" /* newline to print after it */

