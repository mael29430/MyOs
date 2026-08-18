/*
 * MyOS Kernel
 *
 * Noyau principal du système.
 */

#include "../system/config.h"

void console_init(void);
void console_write(const char *text);

void memory_init(void);
void storage_init(void);
void task_init(void);
void syscall_init(void);
void device_init(void);


void kernel_main(void)
{
    console_init();

    console_write("MyOS Kernel Started\n");

    memory_init();
    storage_init();
    task_init();
    syscall_init();
    device_init();

    console_write("MyOS Services Initialized\n");

    while (1)
    {
    }
}
