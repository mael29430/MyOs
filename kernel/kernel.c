/*
 * MyOS Kernel
 *
 * Noyau principal du système.
 */

#include "../system/config.h"


/*
 * Fonctions du système
 */

void console_init(void);
void console_write(const char *text);

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
     * Initialisation de la console
     */

    console_init();

    /*
     * Premier message de MyOS
     */

    console_write("MyOS Kernel Started\n");


    /*
     * Initialisation des services système
     */

    memory_init();

    storage_init();

    task_init();

    syscall_init();

    device_init();


    /*
     * Message indiquant que
     * l'initialisation est terminée.
     */

    console_write("MyOS Services Initialized\n");


    /*
     * Boucle principale du kernel.
     */

    while (1)
    {
        /*
         * Kernel actif.
         */
    }
}
