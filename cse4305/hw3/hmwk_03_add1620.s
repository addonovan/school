# Donovan, Austin D 
# add1620
# 2019-03-28

.global main

.text

main:
    # set up stack, idk doesn't work without it
    subq $24, %rsp 
   
    # scanf("%d", &i); 
    movq $input_format, %rdi
    movq $value, %rsi
    call scanf

    # printf("%d\n", i * i);
    movq $output_format, %rdi
    movl value, %esi
    imull %esi, %esi
    call printf

    # exit(i);
    movl value, %edi # edi because exit-codes are 1 byte 
    call exit

.data

input_format:   .asciz "%d"
output_format:  .asciz "%d\n"
value:          .word 0

