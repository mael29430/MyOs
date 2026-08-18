/*
 * MyOS - ARM64 Exception Vectors
 */

.section .text
.align 11

.global exception_vectors
.global exception_vector_entry

.extern interrupt_handler

exception_vectors:

    /*
     * Current EL with SP0
     */
    b exception_vector_entry
    .space 128 - 4

    /*
     * Current EL with SPx
     */
    b exception_vector_entry
    .space 128 - 4

    /*
     * Lower EL using AArch64
     */
    b exception_vector_entry
    .space 128 - 4

    /*
     * Lower EL using AArch32
     */
    b exception_vector_entry
    .space 128 - 4


exception_vector_entry:

    /*
     * Sauvegarde minimale des registres utilisés.
     */
    stp x0, x1, [sp, #-16]!

    bl interrupt_handler

    ldp x0, x1, [sp], #16

    eret
