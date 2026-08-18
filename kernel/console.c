/*
 * MyOS Console
 *
 * Console série minimale pour QEMU.
 */

#define UART_BASE 0x09000000UL

#define UART_DR (*(volatile unsigned int *)(UART_BASE + 0x00))


/*
 * Initialise la console.
 *
 * Pour QEMU "virt", l'UART est déjà
 * suffisamment configuré pour notre
 * premier test.
 */

void console_init(void)
{
}


/*
 * Écrit un caractère sur la console.
 */

static void console_putc(char c)
{
    UART_DR = (unsigned int)c;
}


/*
 * Écrit une chaîne de caractères.
 */

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
