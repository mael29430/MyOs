/*
 * MyOS - ARM64 Generic Timer
 *
 * Virtual timer for AArch64 / QEMU virt.
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

    __asm__ volatile("isb");
}

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

static uint64_t timer_interval_ticks(void)
{
    uint64_t frequency = timer_read_frequency();

    if (frequency == 0)
    {
        return 0;
    }

    uint64_t interval =
        (frequency * TIMER_INTERVAL_MS) / 1000ULL;

    if (interval == 0)
    {
        interval = 1;
    }

    return interval;
}

static void timer_schedule_next(void)
{
    uint64_t interval = timer_interval_ticks();

    if (interval == 0)
    {
        return;
    }

    uint64_t now = timer_read_counter();

    timer_write_compare(now + interval);
}

void timer_init(void)
{
    uint64_t frequency = timer_read_frequency();

    if (frequency == 0)
    {
        return;
    }

    timer_disable();

    timer_schedule_next();

    timer_enable();
}

void timer_tick(void)
{
    timer_schedule_next();
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
    uint64_t frequency = timer_read_frequency();

    if (frequency == 0)
    {
        return 0;
    }

    return
        (timer_read_counter() * 1000ULL)
        / frequency;
}

uint64_t timer_seconds(void)
{
    uint64_t frequency = timer_read_frequency();

    if (frequency == 0)
    {
        return 0;
    }

    return
        timer_read_counter()
        / frequency;
}
