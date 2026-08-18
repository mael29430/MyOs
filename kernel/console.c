/*
 * MyOS Console
 *
 * Console série PL011 pour QEMU virt.
 */

#define UART_BASE 0x09000000UL

#define UART_DR     (*(volatile unsigned int *)(UART_BASE + 0x00))
#define UART_FR     (*(volatile unsigned int *)(UART_BASE + 0x18))
#define UART_IBRD   (*(volatile unsigned int *)(UART_BASE + 0x24))
#define UART_FBRD   (*(volatile unsigned int *)(UART_BASE + 0x28))
#define UART_LCR_H  (*(volatile unsigned int *)(UART_BASE + 0x2C))
#define UART_CR     (*(volatile unsigned int *)(UART_BASE + 0x30))

#define UART_FR_TXFF    (1 << 5)

#define UART_LCR_H_WLEN_8  (3 << 5)
#define UART_LCR_H_FEN     (1 << 4)

#define UART_CR_UARTEN     (1 << 0)
#define UART_CR_TXE        (1 << 8)
#define UART_CR_RXE        (1 << 9)


void console_init(void)
{
    /*
     * Désactiver temporairement l'UART.
     */
    UART_CR = 0;

    /*
     * Configuration 115200 bauds.
     */
    UART_IBRD = 1;
    UART_FBRD = 40;

    /*
     * 8 bits, FIFO activé.
     */
    UART_LCR_H = UART_LCR_H_WLEN_8 | UART_LCR_H_FEN;

    /*
     * Activer UART + transmission + réception.
     */
    UART_CR = UART_CR_UARTEN | UART_CR_TXE | UART_CR_RXE;
}


static void console_putc(char c)
{
    /*
     * Attendre que le transmetteur soit disponible.
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
