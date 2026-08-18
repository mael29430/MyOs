.global _start

.section .text

_start:

    /*
     * Point d'entrée de MyOS.
     */

    /*
     * Initialisation de la pile.
     */

    ldr x0, =stack_top
    mov sp, x0

    /*
     * Appel du kernel principal en C.
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

/*
 * Alignement de la pile sur 4096 octets.
 */

.align 12

stack:

    /*
     * Réserve 16 Ko pour la pile.
     */

    .skip 16384

stack_top:
