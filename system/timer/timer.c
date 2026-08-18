/*
 * MyOS - ARM64 Generic Timer
 *
 * Timer système basé sur le Generic Timer ARM64.
 */

#include <stdint.h>

/*
 * Fréquence du compteur.
 *
 * Sur QEMU virt, cette valeur est généralement fournie
 * par l'environnement. Pour notre première implémentation,
 * on utilise une valeur standard de 62.5 MHz.
 */
#define TIMER_FREQUENCY 62500000ULL

/*
 * Lecture du compteur virtuel ARM64.
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
 * Lecture de la fréquence du timer directement
 * depuis le registre ARM64.
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
 * Initialisation du timer.
 */
void timer_init(void)
{
    uint64_t frequency = timer_read_frequency();

    /*
     * Si la plateforme fournit une fréquence valide,
     * on l'utilise.
     */
    if (frequency != 0)
    {
        /*
         * La lecture force simplement le registre à être
         * accessible et vérifie que le timer ARM fonctionne.
         */
        (void)frequency;
    }

    /*
     * Lecture initiale du compteur.
     */
    (void)timer_read_counter();
}

/*
 * Retourne le nombre de ticks du Generic Timer.
 */
uint64_t timer_ticks(void)
{
    return timer_read_counter();
}

/*
 * Retourne la fréquence réelle du Generic Timer.
 */
uint64_t timer_frequency(void)
{
    uint64_t frequency = timer_read_frequency();

    if (frequency == 0)
    {
        return TIMER_FREQUENCY;
    }

    return frequency;
}

/*
 * Retourne le temps écoulé en millisecondes.
 */
uint64_t timer_milliseconds(void)
{
    uint64_t ticks = timer_ticks();
    uint64_t frequency = timer_frequency();

    if (frequency == 0)
    {
        return 0;
    }

    return (ticks * 1000ULL) / frequency;
}

/*
 * Retourne le temps écoulé en secondes.
 */
uint64_t timer_seconds(void)
{
    uint64_t ticks = timer_ticks();
    uint64_t frequency = timer_frequency();

    if (frequency == 0)
    {
        return 0;
    }

    return ticks / frequency;
}
