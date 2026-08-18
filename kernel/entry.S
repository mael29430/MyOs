.global _start

.section .text

_start:

    /*
     * Initialisation de la pile.
     * La pile sera placée à une adresse temporaire
     * dans la RAM de la machine virtuelle QEMU.
     */

    ldr x0, =stack_top
    mov sp, x0

    /*
     * Appel du kernel C.
     */

    bl kernel_main

    /*
     * Si kernel_main retourne,
     * on reste dans une boucle infinie.
     */

1:
    wfe
    b 1b


.section .bss

.align 12

stack:

    .skip 16384

stack_top:
