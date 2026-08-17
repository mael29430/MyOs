/*
 * MyOS Kernel
 *
 * Noyau principal du système.
 */


#include "../system/config.h"


void console_init(void);

void memory_init(void);

void storage_init(void);

void task_init(void);



/*
 * Point d'entrée du kernel
 */

void kernel_main(void)
{

    /*
     * Initialisation des services système
     */

    console_init();

    memory_init();

    storage_init();

    task_init();



    /*
     * Plus tard :
     * - pilotes matériels
     * - interface graphique
     * - applications
     */


    while (1)
    {

        /*
         * Boucle principale du kernel.
         */

    }

}
