/*
 * MyOS Kernel
 *
 * Premier noyau expérimental ARM64.
 */


#include "../system/config.h"


void console_init(void);

void memory_init(void);



/*
 * Point d'entrée du kernel
 */

void kernel_main(void)
{

    /*
     * Initialisation des services
     */

    console_init();

    memory_init();



    /*
     * Plus tard :
     * - stockage
     * - réseau
     * - pilotes
     * - interface graphique
     */


    while (1)
    {

        /*
         * Le kernel reste actif.
         */

    }

}
