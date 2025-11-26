; legacy bios bootloader stub meant to be assembled with nasm
; it does not work because i decided to use multiboot instead because it's quicker
; i will eventually come back to this
bits 16
org 0x7C00

mov ah, 0x0E
mov al, "A"
int 0x10
jmp $

disk: db 0

times 446 - ($ - $$) db 0

; master boot record, idfk what any of this means anymore
db 0x00
db 0x00, 0x02, 0x00
db 0x00
db 0x00, 0x02, 0x00
dd 0x01
dd 0x01
times (16 * 3) db 0

db 0x55, 0xAA

