	.global _start

/*****************************************************************************/
/* Subroutines                                                               */
/*****************************************************************************/

/**
 * prints a string to STDOUT, followed by a linefeed.
 * r1 should contain the starting address of the string
 * r2 should contain the length of the string to print
 */
println:
  /* write the string */
  mov r7, #4 @    write...
  mov r0, #1 @ ...to STDOUT...
  swi 0

  /* print a new line */
  mov r7, #4
  mov r0, #1
  mov r2, #1
  ldr r1, =newline
  swi 0

  bx r14 /* jump back to the next instruction */

/**
 * Inverts the character in r3 if it's a letter.
 */
invert_char:

  /* < 'A'? not a letter */
  cmp r3, #'A'
  blt invert_char_return

  /* > 'z'? not a letter */
  cmp r3, #'z'
  bgt invert_char_return

  /* r3 >= 'A' && r3 <= 'Z' */
  cmp r3, #'Z'
  ble invert_char_lowercaserize

  /* r3 >= 'a' && r3 <= 'z' */
  cmp r3, #'a'
  bge invert_char_capitalize

  /* subtract 32 to get to upper case */
  invert_char_capitalize:
    sub r3, #32
    bal invert_char_return

  /* add 32 to get to lower case */
  /* I really wish there was an opposite for capitalize */
  invert_char_lowercaserize:
    add r3, #32
    bal invert_char_return

  /* jump back to the next instruction after whatever called us */
  invert_char_return:
    bx r14

/*****************************************************************************/
/* Main                                                                      */
/*****************************************************************************/

_start:
  ldr  r1, =string

  /* 
   * r3: current character
   * r2: current length
   */
  mov  r3, #0
  mov  r2, #0

  /* iterate over the string until we hit 0x00 */
  /* pre-test loop */
  loop:
    ldrb r3, [ r1, r2 ] /* load one byte in r3 */
    cmp  r3, #0
    beq  loop_break

    /* invert the current character, then jump back here */
    bl   invert_char
    
    /* update char and move to next one */
    /* the missing b here sure was fun to debug (no it wasn't) */
    strb r3, [ r1, r2 ]
    add  r2, #1

    bal  loop

  loop_break:

  /* print the string from the beginning again */
  bl println

_exit:
  mov r7, #1
  swi 0

/*****************************************************************************/
/* Data Section                                                              */
/*****************************************************************************/
	.data

newline:
	.ascii "\n"

string:
	.asciz "This IS a ~ sAMPLE string 4 U 2 use."

