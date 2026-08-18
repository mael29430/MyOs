/*
 * MyOS Kernel
 *
 * Noyau principal du système.
 */

#include "../system/config.h"


/*
 * Console
 */

void console_init(void);

void console_write(const char *text);


/*
 * Mémoire
 */

void memory_init(void);

unsigned long memory_total_pages(void);

unsigned long memory_free_pages(void);


/*
 * Interruptions
 */

void interrupt_init(void);


/*
 * Timer
 */

void timer_init(void);


/*
 * Tâches
 */

void task_init(void);


/*
 * Syscalls
 */

void syscall_init(void);


/*
 * Périphériques
 */

void device_init(void);


/*
 * Stockage
 */

void storage_init(void);


/*
 * Système de fichiers
 */

void filesystem_init(void);


/*
 * Kernel principal
 */

void kernel_main(void)
{
    /*
     * Console
     */

    console_init();

    console_write(
        "\n"
        "================================\n"
        "          MyOS Kernel\n"
        "================================\n"
        "\n"
    );


    /*
     * Mémoire
     */

    memory_init();

    console_write(
        "[ OK ] Memory manager\n"
    );


    /*
     * Interruptions
     */

    interrupt_init();

    console_write(
        "[ OK ] Interrupt manager\n"
    );


    /*
     * Timer
     */

    timer_init();

    console_write(
        "[ OK ] System timer\n"
    );


    /*
     * Tâches
     */

    task_init();

    console_write(
        "[ OK ] Task manager\n"
    );


    /*
     * Périphériques
     */

    device_init();

    console_write(
        "[ OK ] Device manager\n"
    );


    /*
     * Stockage
     */

    storage_init();

    console_write(
        "[ OK ] Storage manager\n"
    );


    /*
     * Système de fichiers
     */

    filesystem_init();

    console_write(
        "[ OK ] File system\n"
    );


    /*
     * Syscalls
     */

    syscall_init();

    console_write(
        "[ OK ] System calls\n"
    );


    /*
     * Kernel prêt.
     */

    console_write(
        "\n"
        "MyOS Kernel is running.\n"
        "Platform: AArch64 / QEMU virt\n"
        "\n"
    );


    /*
     * Boucle principale.
     */

    while (1)
    {
        /*
         * Kernel actif.
         */
    }
}
