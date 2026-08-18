/*
 * MyOS - ARM64 Interrupt / GICv3 Initialization
 */

.section .text
.align 4

.global interrupt_vectors_init

.extern exception_vectors


/*
 * QEMU virt / GICv3
 *
 * GICD base :
 *     0x08000000
 *
 * GICR base :
 *     0x080A0000
 *
 * Virtual timer PPI :
 *     INTID 27
 */

.equ GICD_BASE,       0x08000000
.equ GICR_BASE,       0x080A0000

.equ GICD_CTLR,       0x0000
.equ GICD_ISENABLER0, 0x0100

.equ GICR_WAKER,      0x0014

.equ ICC_PMR_EL1,     S3_0_C4_C6_0
.equ ICC_IGRPEN1_EL1, S3_0_C12_C12_7
.equ ICC_SRE_EL1,     S3_0_C12_C12_5


/*
 * interrupt_vectors_init
 */
interrupt_vectors_init:

    /*
     * ------------------------------------------------
     * VBAR_EL1
     * ------------------------------------------------
     */

    ldr x0, =exception_vectors
    msr vbar_el1, x0

    dsb sy
    isb


    /*
     * ------------------------------------------------
     * GIC Distributor
     * ------------------------------------------------
     */

    movz x1, #(GICD_BASE >> 16)

    /*
     * Enable Group 1 interrupts.
     *
     * GICD_CTLR bit 1 = EnableGrp1NS
     */
    mov w2, #2
    str w2, [x1, #GICD_CTLR]

    dsb sy


    /*
     * ------------------------------------------------
     * GIC Redistributor
     * ------------------------------------------------
     */

    movz x3, #(GICR_BASE >> 16)

    /*
     * Wake redistributor.
     *
     * Clear ProcessorSleep.
     */
    ldr w4, [x3, #GICR_WAKER]

    bic w4, w4, #4

    str w4, [x3, #GICR_WAKER]

    dsb sy


    /*
     * ------------------------------------------------
     * GIC CPU interface
     * ------------------------------------------------
     */

    /*
     * Enable System Register interface.
     */
    mov w5, #1
    msr ICC_SRE_EL1, x5

    isb


    /*
     * Priority mask.
     *
     * 0xFF = accept all priorities.
     */
    mov w5, #0xFF
    msr ICC_PMR_EL1, x5


    /*
     * Enable Group 1 interrupts.
     */
    mov w5, #1
    msr ICC_IGRPEN1_EL1, x5

    isb


    /*
     * ------------------------------------------------
     * ARM Generic Timer IRQ
     * ------------------------------------------------
     *
     * Virtual Timer = PPI 27
     *
     * PPI INTID 27
     *
     * Enable bit :
     *
     *   ISENABLER0 bit 27
     */

    movz x6, #(GICD_BASE >> 16)

    /*
     * Enable IRQ 27.
     */
    mov w7, #(1 << 27)

    str w7, [x6, #GICD_ISENABLER0]

    dsb sy
    isb


    /*
     * ------------------------------------------------
     * CPU IRQ unmask
     * ------------------------------------------------
     */

    /*
     * Clear DAIF.I
     */
    msr daifclr, #2

    dsb sy
    isb


    ret
