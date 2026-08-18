/*
 * MyOS - ARM64 Generic Timer
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
static inline void timer_set_compare(uint64_t value)
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
 * Programmer le prochain tick.
 */
static void timer_program_next(void)
{
    uint64_t frequency = timer_read_frequency();

    if (frequency == 0)
        return;

    uint64_t interval =
        (frequency * TIMER_INTERVAL_MS) / 1000ULL;

    uint64_t now = timer_read_counter();

    timer_set_compare(now + interval);
}

/*
 * Initialisation du timer.
 */
void timer_init(void)
{
    uint64_t frequency = timer_read_frequency();

    if (frequency == 0)
        return;

    timer_program_next();
    timer_enable();
}

/*
 * Appelé lorsqu'un tick du timer est traité.
 *
 * On reprogramme le prochain événement.
 */
void timer_tick(void)
{
    timer_program_next();
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
    uint64_t frequency = timer_read_frequency();

    if (frequency == 0)
        return 0;

    return (timer_read_counter() * 1000ULL) / frequency;
}

/*
 * Temps écoulé en secondes.
 */
uint64_t timer_seconds(void)
{
    uint64_t frequency = timer_read_frequency();

    if (frequency == 0)
        return 0;

    return timer_read_counter() / frequency;
}
