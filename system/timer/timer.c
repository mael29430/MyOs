/*
 * MyOS - ARM64 Generic Timer
 *
 * Periodic virtual timer.
 */

#include <stdint.h>

#define TIMER_INTERVAL_MS 10ULL


/*
 * Lire le compteur virtuel ARM64.
 */
static inline uint64_t timer_read_counter(void)
{
    uint64_t value;

    __asm__ volatile(
        "mrs %0, cntvct_el0"
        : "=r"(value)
    );

    return value;
}


/*
 * Lire la fréquence du timer.
 */
static inline uint64_t timer_read_frequency(void)
{
    uint64_t value;

    __asm__ volatile(
        "mrs %0, cntfrq_el0"
        : "=r"(value)
    );

    return value;
}


/*
 * Programmer la prochaine interruption.
 */
static inline void timer_write_compare(uint64_t value)
{
    __asm__ volatile(
        "msr cntv_cval_el0, %0"
        :
        : "r"(value)
    );

    __asm__ volatile("isb");
}


/*
 * Lire la valeur actuelle de comparaison.
 */
static inline uint64_t timer_read_compare(void)
{
    uint64_t value;

    __asm__ volatile(
        "mrs %0, cntv_cval_el0"
        : "=r"(value)
    );

    return value;
}


/*
 * Activer le timer virtuel.
 *
 * CNTV_CTL_EL0 :
 *
 * bit 0 = ENABLE
 * bit 1 = IMASK
 * bit 2 = ISTATUS (lecture seule)
 */
static inline void timer_enable(void)
{
    uint64_t value = 1ULL;

    /*
     * ENABLE = 1
     * IMASK  = 0
     */
    __asm__ volatile(
        "msr cntv_ctl_el0, %0"
        :
        : "r"(value)
    );

    __asm__ volatile("isb");
}


/*
 * Désactiver le timer.
 */
static inline void timer_disable(void)
{
    uint64_t value = 0;

    __asm__ volatile(
        "msr cntv_ctl_el0, %0"
        :
        : "r"(value)
    );

    __asm__ volatile("isb");
}


/*
 * Autoriser les IRQ au niveau CPU.
 *
 * DAIF :
 *
 * I = IRQ mask
 *
 * "daifclr, #2" efface le bit I.
 */
static inline void timer_enable_cpu_irq(void)
{
    __asm__ volatile(
        "msr daifclr, #2"
        :
        :
        : "memory"
    );

    __asm__ volatile("isb");
}


/*
 * Initialisation du timer.
 */
void timer_init(void)
{
    uint64_t frequency =
        timer_read_frequency();

    if (frequency == 0)
    {
        return;
    }

    /*
     * Nombre de ticks pour 10 ms.
     */
    uint64_t interval =
        (frequency * TIMER_INTERVAL_MS) / 1000ULL;

    if (interval == 0)
    {
        interval = 1;
    }

    /*
     * Compteur actuel.
     */
    uint64_t now =
        timer_read_counter();

    /*
     * Première échéance.
     */
    timer_write_compare(
        now + interval
    );

    /*
     * Active le timer.
     */
    timer_enable();

    /*
     * Autorise les IRQ au niveau CPU.
     */
    timer_enable_cpu_irq();
}


/*
 * Cette fonction peut être appelée par le
 * gestionnaire d'interruption pour programmer
 * la prochaine échéance.
 */
void timer_rearm(void)
{
    uint64_t frequency =
        timer_read_frequency();

    if (frequency == 0)
    {
        return;
    }

    uint64_t interval =
        (frequency * TIMER_INTERVAL_MS) / 1000ULL;

    if (interval == 0)
    {
        interval = 1;
    }

    uint64_t current =
        timer_read_counter();

    /*
     * Toujours programmer dans le futur.
     */
    timer_write_compare(
        current + interval
    );

    timer_enable();
}


/*
 * Nombre de ticks du compteur ARM64.
 */
uint64_t timer_ticks(void)
{
    return timer_read_counter();
}


/*
 * Fréquence du timer.
 */
uint64_t timer_frequency(void)
{
    return timer_read_frequency();
}


/*
 * Temps écoulé en millisecondes.
 */
uint64_t timer_milliseconds(void)
{
    uint64_t frequency =
        timer_frequency();

    if (frequency == 0)
    {
        return 0;
    }

    return
        (timer_ticks() * 1000ULL)
        / frequency;
}


/*
 * Temps écoulé en secondes.
 */
uint64_t timer_seconds(void)
{
    uint64_t frequency =
        timer_frequency();

    if (frequency == 0)
    {
        return 0;
    }

    return
        timer_ticks()
        / frequency;
}
