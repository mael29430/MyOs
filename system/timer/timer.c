/*
 * MyOS - ARM64 Generic Timer
 */

#include <stdint.h>

#define TIMER_INTERVAL_MS 10ULL

static inline uint64_t timer_read_counter(void)
{
    uint64_t value;

    __asm__ volatile(
        "mrs %0, cntvct_el0"
        : "=r"(value)
    );

    return value;
}

static inline uint64_t timer_read_frequency(void)
{
    uint64_t value;

    __asm__ volatile(
        "mrs %0, cntfrq_el0"
        : "=r"(value)
    );

    return value;
}

static inline void timer_write_compare(uint64_t value)
{
    __asm__ volatile(
        "msr cntv_cval_el0, %0"
        :
        : "r"(value)
    );
}

static inline void timer_enable(void)
{
    uint64_t value = 0;

    /*
     * Enable virtual timer.
     */
    __asm__ volatile(
        "msr cntv_ctl_el0, %0"
        :
        : "r"(value | 1ULL)
    );

    __asm__ volatile("isb");
}

void timer_init(void)
{
    uint64_t frequency = timer_read_frequency();

    if (frequency == 0)
    {
        return;
    }

    uint64_t interval =
        (frequency * TIMER_INTERVAL_MS) / 1000ULL;

    uint64_t now = timer_read_counter();

    timer_write_compare(now + interval);

    timer_enable();
}

uint64_t timer_ticks(void)
{
    return timer_read_counter();
}

uint64_t timer_frequency(void)
{
    return timer_read_frequency();
}

uint64_t timer_milliseconds(void)
{
    uint64_t frequency = timer_frequency();

    if (frequency == 0)
    {
        return 0;
    }

    return (timer_ticks() * 1000ULL) / frequency;
}

uint64_t timer_seconds(void)
{
    uint64_t frequency = timer_frequency();

    if (frequency == 0)
    {
        return 0;
    }

    return timer_ticks() / frequency;
}
