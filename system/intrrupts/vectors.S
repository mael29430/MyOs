/*
 * MyOS - ARM64 Exception Vectors
 *
 * Sauvegarde complète des registres généraux lors
 * d'une exception avant d'appeler le gestionnaire C.
 */

.section .text
.align 11

.global exception_vectors
.global exception_vector_entry

.extern interrupt_handler


/*
 * ARM64 exception vector table
 *
 * Chaque entrée occupe 128 octets.
 */

exception_vectors:

    /* Current EL using SP0 */
    b exception_vector_entry
    .space 124

    /* Current EL using SPx */
    b exception_vector_entry
    .space 124

    /* Lower EL using AArch64 */
    b exception_vector_entry
    .space 124

    /* Lower EL using AArch32 */
    b exception_vector_entry
    .space 124


/*
 * Exception entry
 *
 * On sauvegarde x0-x29 et x30.
 *
 * Taille :
 *   31 registres × 8 octets = 248 octets
 *
 * On ajoute 8 octets pour garder la pile alignée
 * sur 16 octets.
 */

exception_vector_entry:

    sub sp, sp, #256

    /*
     * Sauvegarde x0-x29.
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

    /*
     * x28 / x29
     */
    stp x28, x29, [sp, #224]

    /*
     * x30 = link register.
     * Le dernier emplacement reste disponible
     * pour garder le cadre aligné.
     */
    str x30, [sp, #240]

    /*
     * Appel du gestionnaire C.
     */
    bl interrupt_handler

    /*
     * Restauration x0-x29.
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

    /*
     * Restauration de x30.
     */
    ldr x30, [sp, #240]

    /*
     * Libération du cadre.
     */
    add sp, sp, #256

    /*
     * Retour depuis l'exception.
     */
    eret
