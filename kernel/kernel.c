/*
 * MyOS Console
 *
 * Console série PL011 pour QEMU virt.
 */

#define UART_BASE 0x09000000UL

#define UART_DR (*(volatile unsigned int *)(UART_BASE + 0x00))
#define UART_FR (*(volatile unsigned int *)(UART_BASE + 0x18))

#define UART_FR_TXFF (1 << 5)


void console_init(void)
{
    /*
     * Le PL011 de QEMU virt est déjà initialisé
     * suffisamment pour notre premier affichage.
     */
}


static void console_putc(char c)
{
    /*
     * Attendre que le FIFO de transmission
     * ne soit pas plein.
     */

    while (UART_FR & UART_FR_TXFF)
    {
    }

    UART_DR = (unsigned int)c;
}


void console_write(const char *text)
{
    while (*text)
    {
        if (*text == '\n')
        {
            console_putc('\r');
        }

        console_putc(*text);

        text++;
    }
}
