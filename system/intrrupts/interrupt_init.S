/*
 * MyOS - ARM64 Interrupt Initialization
 */

.section .text
.align 4

.global interrupt_vectors_init
.extern exception_vectors

interrupt_vectors_init:

    /*
     * Installer la table des vecteurs ARM64.
     */
    ldr x0, =exception_vectors
    msr vbar_el1, x0

    dsb sy
    isb

    /*
     * Retour immédiat.
     *
     * Le GIC sera configuré séparément.
     */
    ret
