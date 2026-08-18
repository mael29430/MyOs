/*
 * MyOS - ARM64 Interrupt Manager
 */

#include <stdint.h>

static volatile uint64_t interrupt_count = 0;

void interrupt_init(void)
{
    interrupt_count = 0;
}

void interrupt_handler(void)
{
    interrupt_count++;
}

uint64_t interrupt_get_count(void)
{
    return interrupt_count;
}
