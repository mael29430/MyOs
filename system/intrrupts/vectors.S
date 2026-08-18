/*
 * MyOS - ARM64 Exception Vectors
 *
 * Exception frame :
 *
 *   +0    x0
 *   +8    x1
 *   ...
 *   +240  x30
 *   +248  ELR_EL1
 *   +256  SPSR_EL1
 *
 * Frame size = 272 bytes
 */

.section .text
.align 11

.global exception_vectors
.global exception_vector_entry

.extern interrupt_handler


exception_vectors:

    /*
     * Current EL using SP0
     */
    b exception_vector_entry
    .space 124

    /*
     * Current EL using SPx
     */
    b exception_vector_entry
    .space 124

    /*
     * Lower EL using AArch64
     */
    b exception_vector_entry
    .space 124

    /*
     * Lower EL using AArch32
     */
    b exception_vector_entry
    .space 124


exception_vector_entry:

    /*
     * Réserve le frame.
     */
    sub sp, sp, #272

    /*
     * Sauvegarde x0-x30.
     */
    stp x0,  x1,  [sp, #0]
    stp x2,  x3,  [sp, #16]
    stp x4,  x5,  [sp, #32]
    stp x6,  x7,  [sp, #48]
    stp x8,  x9,  [sp, #64]
    stp x10, x11, [sp, #80]
    stp x12, x13, [sp, #96]
    stp x14, x15, [sp, #112]
    stp x16, x17, [sp, #128]
    stp x18, x19, [sp, #144]
    stp x20, x21, [sp, #160]
    stp x22, x23, [sp, #176]
    stp x24, x25, [sp, #192]
    stp x26, x27, [sp, #208]
    stp x28, x29, [sp, #224]

    str x30, [sp, #240]

    /*
     * Sauvegarde PC.
     */
    mrs x0, elr_el1
    str x0, [sp, #248]

    /*
     * Sauvegarde SPSR.
     */
    mrs x0, spsr_el1
    str x0, [sp, #256]

    /*
     * x0 = frame courant.
     */
    mov x0, sp

    /*
     * Le handler retourne dans x0
     * l'adresse du frame à restaurer.
     */
    bl interrupt_handler

    /*
     * x0 = nouveau frame.
     *
     * On change maintenant de pile d'exception.
     */
    mov sp, x0

    /*
     * Restaurer ELR_EL1.
     */
    ldr x0, [sp, #248]
    msr elr_el1, x0

    /*
     * Restaurer SPSR_EL1.
     */
    ldr x0, [sp, #256]
    msr spsr_el1, x0

    /*
     * Restaurer les registres.
     */
    ldp x0,  x1,  [sp, #0]
    ldp x2,  x3,  [sp, #16]
    ldp x4,  x5,  [sp, #32]
    ldp x6,  x7,  [sp, #48]
    ldp x8,  x9,  [sp, #64]
    ldp x10, x11, [sp, #80]
    ldp x12, x13, [sp, #96]
    ldp x14, x15, [sp, #112]
    ldp x16, x17, [sp, #128]
    ldp x18, x19, [sp, #144]
    ldp x20, x21, [sp, #160]
    ldp x22, x23, [sp, #176]
    ldp x24, x25, [sp, #192]
    ldp x26, x27, [sp, #208]

    ldp x28, x29, [sp, #224]

    ldr x30, [sp, #240]

    /*
     * Retour à la tâche.
     */
    add sp, sp, #272

    eret
