/*
 * MyOS Kernel
 *
 * Premier noyau expérimental ARM64.
 */


/* Fonction de la console */

void console_init(void);


/*
 * Point d'entrée du kernel
 */

void kernel_main(void)
{

    /*
     * Initialisation
     * des services système
     */

    console_init();


    /*
     * Boucle principale
     * du kernel
     */

    while (1)
    {

        /*
         * Le kernel reste actif.
         */

    }

}
