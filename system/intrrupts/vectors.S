/*
 * MyOS - ARM64 Exception Vectors
 */

.section .text
.align 11

.global exception_vectors
.global exception_vector_entry

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
     * Pour l'instant, on retourne simplement de
     * l'exception sans modifier le contexte.
     *
     * Le vrai gestionnaire d'interruptions sera
     * branché ensuite.
     */

    eret
