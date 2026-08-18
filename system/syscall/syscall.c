/*
 * MyOS System Calls
 *
 * Interface entre les programmes
 * et le kernel.
 */

enum
{
    SYS_NONE = 0,
    SYS_WRITE = 1,
    SYS_EXIT = 2,
    SYS_MEMORY = 3,
    SYS_TIME = 4
};


/*
 * Initialise le système de syscalls.
 */

void syscall_init(void)
{
}


/*
 * Traite un appel système.
 */

unsigned long syscall_handle(
    unsigned long number,
    unsigned long arg0,
    unsigned long arg1,
    unsigned long arg2)
{
    (void)arg0;
    (void)arg1;
    (void)arg2;

    switch (number)
    {
        case SYS_NONE:
            return 0;

        case SYS_WRITE:
            return 0;

        case SYS_EXIT:
            return 0;

        case SYS_MEMORY:
            return 0;

        case SYS_TIME:
            return 0;

        default:
            return (unsigned long)-1;
    }
}
