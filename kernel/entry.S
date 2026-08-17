.global _start

.section .text

_start:

    /*
     * Point d'entrée de MyOS.
     *
     * Pour l'instant, on ne fait
     * qu'entrer dans le kernel.
     */

    bl kernel_main

1:
    wfe
    b 1b
