CSE 2312 Computer Organization and Assembly Language Programming
===

Professor: [Darin Brezeale](http://omega.uta.edu/~darin/)

> Computer organization from the viewpoint of software, including: the memory 
> hierarchy, instruction set architectures, memory addressing, input-output, 
> integer and floating-point representation and arithmetic. The relationship of
> higher-level programming languages to the operating system and to instruction
> set architecture are explored. Some programming in an assembly language
-- [UTA Course Catalog](http://catalog.uta.edu/coursedescriptions/cse/) 

This repository only contains the assembly programming assignments from the
course. Programs are written for ARMv7 processors and were tested on Raspberry
Pi 2s.

hw02.s
---
This was a hand-holdy assignment to follow the directions provided and make
sure that our environments were set up, we could assemble our programs, and
run in GDB to validate.

The comments in the file describe the directions were were given exactly.


hw03.s
---
This introduced branching and their uses in conditional statements. The program
was essentially this C program:
```c
int main()
{
  int r0 = 0;
  int x = 10;

  if ( x > 5 )
    r0 += 1

  if ( x >= 10 )
    r0 += 5
  else
    r0 += 3

  if ( 9 < x && x <= 10 )
    r0 += 7

  return r0;
}
```

In this file only, I took advantage of the `@` syntax for EOL comments in ARM7,
and used it to put braces around conditionals. I thought it was neat and
resembled ASP.NET's cshtml files, but later dropped it.


hw04a.s
---
Given a hardcoded array of 15 signed 32-bit integers, find the minimum and
maximum values.


hw04b.s
---
Given a hardcoded null-terminated string invert the case of each alphabetic
character, then print the resulting string to stdout.


hw05.s
---
Read 5 single-digit base-10 integers from the stdin, then exit with the status
code being equal to the sum of the inputted characters.


hw07.s
---
Given a hardcoded 7x4 matrix of signed 32-bit integers, calculate the sum
of each row of the matrix, then exit with the status code equal to the largest
of these sums.


hw09.s
---
Read characters from stdin, pushing them onto the stack as they are entered.  
Every 4 characters, pop 2 items off the stack and print them to stdout.  
If the user enters a `'q'`, then pop and print all characters off of the stack,
then exit.


hw10a.s
---
Given a hardcoded, null-terminated string, interpret each character as a binary
coded decimal. Create a string containing all of these numbers and print it to
stdout after the entire input string has been decoded.


hw10b.s
---
Using subroutines, find the GCD between three sets of 2 integer pairs then print
them as a string to stdout.


hw11.s
---
Given a hardcoded array of the following structure:
```c
struct Student 
{
  char name[ 12 ]; // 11 characters + \0
  int num;
};
```
The values of the array are the members of my class with a random number
assigned to them.

Read 2 numbers from stdin, a and b. Print the name of any student where
`a <= student.num <= b` holds.

