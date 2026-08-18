/*
 * MyOS - ARM64 Exception Vectors
 *
 * Exception frame :
 *
 *   +0    x0
 *   +8    x1
 *   +16   x2
 *   +24   x3
 *   +32   x4
 *   +40   x5
 *   +48   x6
 *   +56   x7
 *   +64   x8
 *   +72   x9
 *   +80   x10
 *   +88   x11
 *   +96   x12
 *   +104  x13
 *   +112  x14
 *   +120  x15
 *   +128  x16
 *   +136  x17
 *   +144  x18
 *   +152  x19
 *   +160  x20
 *   +168  x21
 *   +176  x22
 *   +184  x23
 *   +192  x24
 *   +200  x25
 *   +208  x26
 *   +216  x27
 *   +224  x28
 *   +232  x29
 *   +240  x30
 *   +248  ELR_EL1
 *   +256  SPSR_EL1
 *
 * Total : 264 octets
 * Alignement : 16 octets
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
     * Réserve 272 octets.
     *
     * 264 octets de données
     * + 8 octets de marge pour garder SP aligné.
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
     * Sauvegarde du PC et de l'état CPU.
     */
    mrs x0, elr_el1
    str x0, [sp, #248]

    mrs x0, spsr_el1
    str x0, [sp, #256]

    /*
     * x0 = adresse du frame.
     */
    mov x0, sp

    /*
     * Gestionnaire C.
     */
    bl interrupt_handler

    /*
     * Restauration de ELR_EL1 et SPSR_EL1.
     */
    ldr x0, [sp, #248]
    msr elr_el1, x0

    ldr x0, [sp, #256]
    msr spsr_el1, x0

    /*
     * Restauration x0-x30.
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
     * Libère le frame.
     */
    add sp, sp, #272

    /*
     * Retour d'exception.
     */
    eret
