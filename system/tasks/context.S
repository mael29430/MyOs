/*
 * MyOS - ARM64 Task Context
 *
 * Sauvegarde et restauration des registres callee-saved.
 *
 * Layout C :
 *
 *   +0   x19
 *   +8   x20
 *   +16  x21
 *   +24  x22
 *   +32  x23
 *   +40  x24
 *   +48  x25
 *   +56  x26
 *   +64  x27
 *   +72  x28
 *   +80  sp
 *   +88  x30
 */

.section .text
.align 4

.global task_context_save
.global task_context_restore


/*
 * int task_context_save(task_context_t *context)
 *
 * x0 = adresse du contexte
 *
 * Retourne 0.
 */
task_context_save:

    stp x19, x20, [x0, #0]
    stp x21, x22, [x0, #16]
    stp x23, x24, [x0, #32]
    stp x25, x26, [x0, #48]
    stp x27, x28, [x0, #64]

    mov x1, sp
    str x1, [x0, #80]

    mov x1, x30
    str x1, [x0, #88]

    mov x0, #0

    ret


/*
 * void task_context_restore(task_context_t *context)
 *
 * x0 = adresse du contexte
 */
task_context_restore:

    ldp x19, x20, [x0, #0]
    ldp x21, x22, [x0, #16]
    ldp x23, x24, [x0, #32]
    ldp x25, x26, [x0, #48]
    ldp x27, x28, [x0, #64]

    ldr x1, [x0, #80]
    mov sp, x1

    ldr x30, [x0, #88]

    ret
