/*
 * MyOS Kernel
 *
 * Noyau principal du système.
 */

#include "../system/config.h"


void console_init(void);
void console_write(const char* text);

void memory_init(void);
void storage_init(void);
void task_init(void);
void syscall_init(void);
void device_init(void);


/*
 * Point d'entrée du kernel
 */

void kernel_main(void)
{
    /*
     * Initialisation des services système
     */

    console_init();

    console_write("\n");
    console_write("================\n");
    console_write("      MyOS\n");
    console_write("================\n");
    console_write("\n");
    console_write("Kernel demarre !\n");
    console_write("Initialisation du systeme...\n");


    memory_init();

    storage_init();

    task_init();

    syscall_init();

    device_init();


    console_write("Systeme MyOS initialise.\n");


    /*
     * Boucle principale du kernel.
     */

    while (1)
    {
    }
}
