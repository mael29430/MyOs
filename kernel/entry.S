.global _start

.section .text

_start:

    /*
     * Point d'entrée de MyOS.
     */

    /*
     * Mettre la pile au sommet
     * de l'espace réservé.
     */

    ldr x0, =stack_top
    mov sp, x0

    /*
     * Appeler le kernel principal.
     */

    bl kernel_main

    /*
     * kernel_main ne devrait jamais
     * retourner.
     */

halt:
    wfe
    b halt


.section .bss

/*
 * Alignement de la pile sur 4096 octets.
 */

.align 12

stack:

    /*
     * 16 Ko de pile.
     */

    .skip 16384

stack_top:
