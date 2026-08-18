/*
 * MyOS Interrupt Manager
 *
 * Architecture ARM64.
 */

static volatile unsigned long
interrupts_enabled = 0;


/*
 * Initialise le système d'interruptions.
 *
 * Pour cette première version,
 * nous préparons uniquement l'architecture.
 */

void interrupt_init(void)
{
    interrupts_enabled = 0;
}


/*
 * Active les interruptions ARM64.
 */

void interrupt_enable(void)
{
    asm volatile("msr daifclr, #2");
    interrupts_enabled = 1;
}


/*
 * Désactive les interruptions ARM64.
 */

void interrupt_disable(void)
{
    asm volatile("msr daifset, #2");
    interrupts_enabled = 0;
}


/*
 * Retourne l'état logiciel.
 */

unsigned long interrupt_status(void)
{
    return interrupts_enabled;
}
