/*
 * MyOS Console
 *
 * Premier système d'affichage
 * du kernel.
 */

#define UART0_BASE 0x09000000UL

volatile unsigned int* const UART0 =
    (volatile unsigned int*)UART0_BASE;


void console_init(void)
{
    /*
     * Initialisation future
     * de la console MyOS.
     */
}


void console_write(const char* text)
{
    while (*text)
    {
        *UART0 = *text;
        text++;
    }
}
