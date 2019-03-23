.global main

.text
main:
    movq    $msg, %rdi
    call    puts
    
    movl    $0, %edi
    call    exit

.data
msg:
    .asciz  "Hello, world, from puts!"

