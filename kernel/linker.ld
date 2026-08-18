ENTRY(_start)

SECTIONS
{
    . = 0x400000;

    .text :
    {
        *(.text*)
    }

    .rodata :
    {
        *(.rodata*)
    }

    .data :
    {
        *(.data*)
    }

    .bss :
    {
        . = ALIGN(16);
        __bss_start = .;

        *(.bss*)
        *(COMMON)

        . = ALIGN(16);
        __bss_end = .;
    }

    . = ALIGN(16);

    __kernel_end = .;
}
