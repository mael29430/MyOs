/*
 * MyOS - ARM64 Interrupt Initialization
 */

.section .text
.align 4

.global interrupt_vectors_init
.extern exception_vectors

interrupt_vectors_init:

    /*
     * Charger l'adresse de notre table de vecteurs
     * dans VBAR_EL1.
     */
    ldr x0, =exception_vectors
    msr vbar_el1, x0

    /*
     * Synchroniser l'écriture du registre système.
     */
    dsb sy
    isb

    ret
