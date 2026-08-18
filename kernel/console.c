/*
 * MyOS Console
 *
 * Console série pour QEMU virt.
 */

#define UART_BASE 0x09000000UL

#define UARTDR 0x00
#define UARTFR 0x18

#define UARTFR_TXFF (1 << 5)

static volatile unsigned int* const uart =
    (volatile unsigned int*)UART_BASE;


void console_init(void)
{
    /*
     * QEMU virt fournit déjà l'UART.
     * Il n'y a pas besoin d'une initialisation
     * matérielle complexe pour notre premier test.
     */
}


void console_write(const char* text)
{
    while (*text)
    {
        /*
         * Attendre que le FIFO de transmission
         * ne soit pas plein.
         */

        while (uart[UARTFR / 4] & UARTFR_TXFF)
        {
        }

        /*
         * Envoyer le caractère.
         */

        uart[UARTDR / 4] = (unsigned int)*text;

        text++;
    }
}
