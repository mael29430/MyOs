/*
 * MyOS Kernel
 *
 * Premier noyau expérimental ARM64.
 */


#include "../system/config.h"


void console_init(void);


/*
 * Point d'entrée du kernel
 */

void kernel_main(void)
{

    /*
     * Initialisation console
     */

    console_init();


    /*
     * Ici plus tard :
     * - mémoire
     * - stockage
     * - pilotes
     * - sécurité
     */


    while (1)
    {

        /*
         * Le kernel reste actif.
         */

    }

}
