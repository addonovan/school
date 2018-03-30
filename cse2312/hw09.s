	.global _start

/*
 * Registers:
 * r0-r7 are general purpose
 * r8    subroutine return value 1
 * r9    subroutine return value 2
 * r10   subroutine param 1
 * r11   subroutine param 2
 * r12   subroutine call stack pointer (see comment at call_stack label)
 */

/******************************************************************************/
/* S U B R O U T I N E S                                                      */
/******************************************************************************/

/**
 * Reads a character from standard input.
 * 
 * Inputs:
 *  r10: none
 *  r11: none
 * 
 * Outputs:
 *  r8: character from stdin
 *  r9: none
 */
getc:
  /* keep the state of the outside program */
  stmfd r12!, { r0-r7 }

  /*
   * r0: file pointer (stdin = 0)
   * r1: string address (=string)
   * r2: number of characters to read in (1 char + newline = 2)
   * r7: syscall value (read = 3)
   */

  mov   r7, #3       /*    read...       */
  mov   r0, #0       /* ...from stdin... */
  mov   r2, #2       /* ...2 chars...    */
  ldr   r1, =string  /* ...into input    */
  swi   0

  /* grab the first character out of it */
  ldrb  r8, [ r1 ]

  /* restore the outside state */
  ldmfd r12!, { r0-r7 }

  /* jump back */
  bx r14

/**
 * Pops the stack and prints the output a given number
 * of times.
 *
 * Inputs:
 *  r10: number of characters to print
 *  r11: none
 * 
 * Outputs:
 *  r8: none
 *  r9: none
 */
pop_stack:
  /* push the current working registers onto the stack */
  stmfd r12!, { r0-r7 }

  /*
   * r0: file pointer (stdout = 1)
   * r1: string address (=string)
   * r2: number of characters to print (1)
   * r3: character to print
   * r4: the remainining number of chars to pop and print
   * r7: syscall value (write = 4)
   */

  /* 
   * copy parameter to new register
   * just because I don't like the idea of having side effects other than the
   * output registers
   */
  mov r4, r10

  pop_stack_loop:
    
    /* continue until we've printed the number of chars we want */
    cmp r4, #0
    ble pop_stack_return
    
    /* pop the character off the stack */
    ldmfd sp!, { r3 }
    
    /* insert character into the string */
    ldr   r1, =string
    strb  r3, [ r1 ]

    /* write the string */
    mov r7, #4  /*    write...             */
    mov r2, #1  /* ...1 character...       */
    mov r0, #1  /* ...to stdout...         */
                /* ...starting at `string` */
    swi 0

    /* we've written one character */
    sub r4, #1

    bal pop_stack_loop


  pop_stack_return:
    /* print out a newline */
    mov r7, #4        /*    write...              */
    mov r2, #1        /* ...1 character...        */
    mov r0, #1        /* ...to stdout...          */
    ldr r1, =newline  /* ...starting at `newline` */
    swi 0

    /* return the call stack to its correct registers */
    ldmfd r12!, {r0-r7}

    /* jump back */
    bx r14

/******************************************************************************/
/* M A I N                                                                    */
/******************************************************************************/

_start:
  /* set up call stack */
  ldr   r12, =call_stack

  /* 
   * r0 - current character count
   * r1 - loop counter (0-3)
   * r2 - current character
   */

  /* initialize loop counters */
  mov   r0, #0
  mov   r1, #0

  loop:
    
    /* reads the next character */
    bl    getc
    mov   r2, r8

    /* is the current character a q? */
    cmp   r2, #'q'
    beq   _exit

    /* push the character onto the stack */
    stmfd sp!, { r2 }

    /* increment our loop counters */
    add   r0, #1
    add   r1, #1

    /* if we're on a fourth loop: */
    cmp   r1, #4
    bne   loop

    /* pop & print 2 characters */
    mov   r10, #2
    bl    pop_stack

    /* reset r0 % 4 to zero */
    mov   r1, #0

    /* we popped two characters */
    sub   r0, #2

    bal   loop 

_exit:
  /* before quitting, print all characters on the stack */
  mov r10, r0
  bl pop_stack

  mov r7, #1
  swi 0

/******************************************************************************/
/* D A T A                                                                    */
/******************************************************************************/

	.data

/* 
 * This is only used to keep registers separated. Because there's no
 * nested calls, I used as little memory as is required to hold all registers
 */

/* Space for the registers to stay during subroutines */
/* 32 = 4 ( bytes/register ) * 8 ( r0-r7 ) */
call_stack:
  .space 32

/* 2 characters, for reading in one character from the command line. */
string:
  .ascii " \n"

/* New line character */
newline:
  .ascii "\n"

