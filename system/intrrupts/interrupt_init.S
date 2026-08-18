/*
 * MyOS - ARM64 GICv3 Interrupt Initialization
 *
 * QEMU:
 *     -machine virt,gic-version=3
 *
 * GIC Distributor:
 *     0x08000000
 *
 * GIC Redistributor:
 *     0x080A0000
 *
 * ARM Generic Virtual Timer:
 *     PPI / INTID 27
 */

.section .text
.align 4

.global interrupt_vectors_init

.extern exception_vectors


/*
 * GICv3 addresses
 */
.equ GICD_BASE,              0x08000000
.equ GICR_BASE,              0x080A0000

/*
 * GIC Distributor registers
 */
.equ GICD_CTLR,              0x0000

/*
 * GIC Redistributor registers
 */
.equ GICR_WAKER,             0x0014

/*
 * SGI/PPI register frame
 *
 * GICR_BASE + 0x10000
 */
.equ GICR_SGI_BASE,          0x10000
.equ GICR_ISENABLER0,        0x0100


/*
 * interrupt_vectors_init
 *
 * Configure:
 *
 *   1. ARM64 exception vectors
 *   2. GICv3 Distributor
 *   3. GICv3 Redistributor
 *   4. GIC CPU system-register interface
 *   5. Virtual timer PPI 27
 *   6. CPU IRQ unmask
 */
interrupt_vectors_init:


    /*
     * ------------------------------------------------
     * 1. ARM64 exception vectors
     * ------------------------------------------------
     */

    ldr x0, =exception_vectors
    msr vbar_el1, x0

    dsb sy
    isb


    /*
     * ------------------------------------------------
     * 2. GIC Distributor
     * ------------------------------------------------
     */

    movz x1, #0x0800, lsl #16

    /*
     * Enable Group 1 Non-secure interrupts.
     *
     * GICD_CTLR:
     *
     *     bit 1 = EnableGrp1NS
     */
    mov w2, #2
    str w2, [x1, #GICD_CTLR]

    dsb sy


    /*
     * ------------------------------------------------
     * 3. GIC Redistributor
     * ------------------------------------------------
     */

    movz x3, #0x080A, lsl #16


    /*
     * Wake the redistributor.
     *
     * GICR_WAKER:
     *
     *     bit 2 = ProcessorSleep
     */
    ldr w4, [x3, #GICR_WAKER]

    bic w4, w4, #4

    str w4, [x3, #GICR_WAKER]

    dsb sy


    /*
     * Wait until ChildrenAsleep becomes zero.
     *
     * GICR_WAKER bit 2 must clear before
     * using the redistributor.
     */
gic_wait_awake:

    ldr w4, [x3, #GICR_WAKER]

    tst w4, #4

    b.ne gic_wait_awake


    /*
     * ------------------------------------------------
     * 4. GIC CPU interface
     * ------------------------------------------------
     */

    /*
     * Enable the system-register interface.
     *
     * ICC_SRE_EL1.SRE = 1
     */
    mov x5, #1

    msr ICC_SRE_EL1, x5

    isb


    /*
     * Accept all interrupt priorities.
     *
     * ICC_PMR_EL1 = 0xFF
     */
    mov x5, #0xFF

    msr ICC_PMR_EL1, x5


    /*
     * Enable Group 1 interrupts.
     *
     * ICC_IGRPEN1_EL1 = 1
     */
    mov x5, #1

    msr ICC_IGRPEN1_EL1, x5

    isb


    /*
     * ------------------------------------------------
     * 5. Enable ARM Generic Virtual Timer
     * ------------------------------------------------
     *
     * Virtual timer:
     *
     *     INTID = 27
     *
     * PPI 27 is in ISENABLER0.
     *
     * Redistributor SGI/PPI frame:
     *
     *     GICR_BASE + 0x10000
     *
     * ISENABLER0:
     *
     *     +0x100
     *
     * Therefore:
     *
     *     GICR_BASE + 0x10100
     */

    add x6, x3, #GICR_SGI_BASE

    /*
     * Enable bit 27.
     *
     * 1 << 27
     */
    mov w7, #0x08000000

    str w7, [x6, #GICR_ISENABLER0]

    dsb sy
    isb


    /*
     * ------------------------------------------------
     * 6. Unmask IRQ exceptions at the CPU
     * ------------------------------------------------
     *
     * DAIF.I = 0
     *
     * #2 selects the IRQ mask.
     */
    msr daifclr, #2

    dsb sy
    isb


    /*
     * Initialization complete.
     */
    ret
