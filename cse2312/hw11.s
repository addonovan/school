	.global _start

/*
 * r0-r7   General Purpose
 * r8-r11  Subroutine input/outputs
 */

/******************************************************************************/
/* S U B R O U T I N E S                                                      */
/******************************************************************************/

/**
 * Reads a specified number of characters from stdin into the given string.
 *
 * @param[in]   r8  Location to write input to
 * @param[in]   r9  Number of characters to read in
 */
fgets:
  stmfd sp!, { r0 - r7 }

  mov   r7, #3      /* =read */
  mov   r0, #0      /* =stdin  */
  add   r2, r9, #1  /* (characters to read + 1 newline) */
  mov   r1, r8      /* location to write to */
  swi   0

  ldmfd sp!, { r0 - r7 }
  bx lr

/**
 * Writes the specified characters to stdout
 *
 * @param[in]   r8  Location of string to print
 * @param[in]   r9  Number of characters to print
 */
fputs:
  stmfd sp!, { r0 - r7 }

  mov   r7, #4 /* =write */
  mov   r0, #1 /* =stdout */
  mov   r2, r9 /* # chars to print */
  mov   r1, r8 /* location of string to print */
  swi   0

  /* now print a new line */
  mov   r7, #4
  mov   r0, #1
  mov   r2, #1
  ldr   r1, =newline
  swi   0

  ldmfd sp!, { r0 - r7 }
  bx    lr

/**
 * Converts an ASCII sequence of characters to an integral value
 *
 * @param[in]   r8  Location of ASCII sequence to read
 *                  This should be line-feed terminated (i.e. end with \n)
 * @param[out]  r10 The integral value of the ASCII sequence  
 */
atoi:
  stmfd sp!, { r0 - r7 }

  /*
   * r0   Character to process
   * r1   Numeric value of `r0`
   * r2   Constant 10 (dec)
   * r3   Temporary value from multiplication
   * r4   Copy of r8 (for us to mutate)
   */

  /* set our output to zero */
  mov r10, #0
  mov r2, #10
  mov r4, r8

  atoi_loop:
    /* get the next character from the string */
    ldrb  r0, [ r4 ], #1 
    cmp   r0, #'\n'

    /* predicated commands simplify the logic here imo */
    subne r1, r0, #'0' /* convert to numeric value */
    mulne r3, r10, r2   /* shift number 1 digit to the left */
    movne r10, r3       /* move the output from the mul */
    addne r10, r1       /* add our digit */

    /* only repeat if this wasn't the end of the number */
    bne   atoi_loop
  
  ldmfd sp!, { r0 - r7 }
  bx lr



/******************************************************************************/
/* M A I N                                                                    */
/******************************************************************************/

_start:
  /*
   * r0   The lower bound (user input)
   * r1   The upper bound (user input)
   * r2   Iteration counter
   * r3   Pointer to current name
   * r4   Pointer to current number
   * r5   Current number
   */

  /* prep for fgets call */
  ldr r8, =input
  mov r9, #3

  /* read lower bound into r0 */
  bl  fgets
  bl  atoi
  mov r0, r10

  /* read upper bound into r1 */
  bl  fgets
  bl  atoi
  mov r1, r10

  mov r2, #0

  /* names start immediately, scores start 12 bytes after that */
  ldr r3, =array
  add r4, r3, #12

  /* when we go to print, we'll want to print 11 chars */
  mov r9, #11

  loop:

    /* quit the loop once we've hit 77 records */
    cmp r2, #77
    bge _exit

    /* we will be processing a record */
    add r2, #1

    /* load the current score  */
    ldr r5, [ r4 ], #16

    /* save the current name into r8 (just in case) */
    mov r8, r3
    add r3, #16 /* skip score to get to next name */

    /* if score <= min_score, continue */
    cmp r5, r0
    blt loop

    /* if score >= max_score, continue */
    cmp r5, r1
    bgt loop

    /* print the current name */
    bl  fputs
    bal loop

_exit:
  mov r7, #1
  swi 0


/******************************************************************************/
/* D A T A                                                                    */
/******************************************************************************/

	.data

/** the user input, 3 characters for digits, 1 for newline */
input:
	.space 4

/** newline for when we need it */
newline:
	.ascii "\n"

array:
/*              12345678901   with extra byte for null                        */
	.asciz "Adam       "  
	.word 730
	.asciz "Ahmed      "
	.word 927
	.asciz "Alan       "
	.word 689
	.asciz "Alexander  "
	.word 536
	.asciz "Allea      "
	.word 656
	.asciz "Amgad      "
	.word 734
	.asciz "Andrew     "
	.word 289
	.asciz "Anh        "
	.word 526
	.asciz "Austin     "
	.word 51
	.asciz "Bashar     "
	.word 743
	.asciz "Brandon    "
	.word 384
	.asciz "Brian      "
	.word 179
	.asciz "Bryan      "
	.word 452
	.asciz "Caleb      "
	.word 987
	.asciz "Chas       "
	.word 505
	.asciz "Chase      "
	.word 853
	.asciz "Chelsea    "
	.word 703
	.asciz "Chimere    "
	.word 360
	.asciz "Christian  "
	.word 980
	.asciz "Christopher"
	.word 439
	.asciz "Connor     "
	.word 192
	.asciz "Daeyoung   "
	.word 383
	.asciz "Devi       "
	.word 81
	.asciz "Diptin     "
	.word 643
	.asciz "Donovan    "
	.word 377
	.asciz "Elias      "
	.word 874
	.asciz "Emmanuel   "
	.word 850
	.asciz "Esha       "
	.word 842
	.asciz "Ethan      "
	.word 161
	.asciz "Francis    "
	.word 565
	.asciz "Gandhali   "
	.word 391
	.asciz "Giannina   "
	.word 921
	.asciz "Grayson    "
	.word 34
	.asciz "Hayden     "
	.word 810
	.asciz "Ishwar     "
	.word 899
	.asciz "Jake       "
	.word 563
	.asciz "Jesee      "
	.word 334
	.asciz "Jimmy      "
	.word 264
	.asciz "Jonathan   "
	.word 310
	.asciz "Jordan     "
	.word 143
	.asciz "Jose       "
	.word 342
	.asciz "Joseph     "
	.word 218
	.asciz "Juan       "
	.word 14
	.asciz "Kent       "
	.word 910
	.asciz "Kopawid    "
	.word 50
	.asciz "Kyle       "
	.word 418
	.asciz "Kyrell     "
	.word 795
	.asciz "Lewis      "
	.word 724
	.asciz "Logan      "
	.word 632
	.asciz "Long       "
	.word 743
	.asciz "Luis       "
	.word 302
	.asciz "Luke       "
	.word 50
	.asciz "Madhula    "
	.word 84
	.asciz "Manswalu   "
	.word 520
	.asciz "Mapu       "
	.word 564
	.asciz "Matthew    "
	.word 695
	.asciz "Michael    "
	.word 650
	.asciz "Minh       "
	.word 11
	.asciz "Mohammed   "
	.word 593
	.asciz "Muraj      "
	.word 150
	.asciz "Nhi        "
	.word 934
	.asciz "Phu        "
	.word 951
	.asciz "Rajvi      "
	.word 255
	.asciz "Ray        "
	.word 16
	.asciz "Reese      "
	.word 663
	.asciz "Robert     "
	.word 234
	.asciz "Salvador   "
	.word 721
	.asciz "Savannah   "
	.word 52
	.asciz "Shreyasi   "
	.word 439
	.asciz "Silvia     "
	.word 714
	.asciz "Stephen    "
	.word 106
	.asciz "Sukrid     "
	.word 740
	.asciz "Thomas     "
	.word 546
	.asciz "Thuy       "
	.word 373
	.asciz "Toan       "
	.word 994
	.asciz "Vivek      "
	.word 324
	.asciz "Zachary    "
	.word 921
