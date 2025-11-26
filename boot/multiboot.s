.set MAGIC, 0x1BADB002
.set FLAGS, 3

.section .multiboot
.long MAGIC, FLAGS, -(MAGIC + FLAGS)

.bss
.skip 16384
stack:

.text
.global _start
.extern main

_start:
    movl $stack, %esp

    call main

    cli
    hlt