.text
.globl main

main:
    li a0, 14
    li a7, 1
    ecall
    li a0, 10
    li a7, 11
    ecall
    li a7, 10
    ecall