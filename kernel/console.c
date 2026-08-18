/*
 * MyOS Console
 *
 * Première console texte de MyOS.
 */

#define UART_BASE 0x09000000UL

static volatile unsigned int *const UART =
    (volatile unsigned int *)UART_BASE;


void console_init(void)
{
    /*
     * Initialisation minimale de la console.
     */
}


void console_write(const char *text)
{
    while (*text)
    {
        *UART = (unsigned int)*text;
        text++;
    }
}
