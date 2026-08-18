/*
 * MyOS - ARM64 Generic Timer
 *
 * Virtual timer for AArch64 / QEMU virt.
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
 * Lire la fréquence du compteur.
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
 * Activer le timer virtuel.
 *
 * CNTV_CTL_EL0 :
 * bit 0 = ENABLE
 * bit 1 = IMASK
 */
static inline void timer_enable(void)
{
    uint64_t control = 1ULL;

    __asm__ volatile(
        "msr cntv_ctl_el0, %0"
        :
        : "r"(control)
    );

    __asm__ volatile("isb");
}


/*
 * Désactiver le timer.
 */
static inline void timer_disable(void)
{
    uint64_t control = 0ULL;

    __asm__ volatile(
        "msr cntv_ctl_el0, %0"
        :
        : "r"(control)
    );

    __asm__ volatile("isb");
}


/*
 * Programme la prochaine échéance du timer.
 */
static void timer_schedule_next(void)
{
    uint64_t frequency = timer_read_frequency();

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

    uint64_t now =
        timer_read_counter();

    timer_write_compare(now + interval);
}


/*
 * Initialise le timer ARM64.
 */
void timer_init(void)
{
    uint64_t frequency =
        timer_read_frequency();

    if (frequency == 0)
    {
        return;
    }

    timer_disable();

    timer_schedule_next();

    timer_enable();
}


/*
 * Réarme le timer après une interruption.
 *
 * Cette fonction doit être appelée par
 * le gestionnaire d'interruptions après
 * avoir traité le tick.
 */
void timer_tick(void)
{
    timer_schedule_next();
}


/*
 * Retourne le compteur ARM64.
 */
uint64_t timer_ticks(void)
{
    return timer_read_counter();
}


/*
 * Retourne la fréquence du compteur.
 */
uint64_t timer_frequency(void)
{
    return timer_read_frequency();
}


/*
 * Retourne le temps écoulé en millisecondes.
 */
uint64_t timer_milliseconds(void)
{
    uint64_t frequency =
        timer_read_frequency();

    if (frequency == 0)
    {
        return 0;
    }

    return
        (timer_read_counter() * 1000ULL)
        / frequency;
}


/*
 * Retourne le temps écoulé en secondes.
 */
uint64_t timer_seconds(void)
{
    uint64_t frequency =
        timer_read_frequency();

    if (frequency == 0)
    {
        return 0;
    }

    return
        timer_read_counter()
        / frequency;
}
