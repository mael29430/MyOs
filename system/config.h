#ifndef MYOS_CONFIG_H
#define MYOS_CONFIG_H

/*
 * MyOS configuration
 */

/*
 * Mémoire QEMU virt.
 *
 * Pour notre première plateforme AArch64,
 * QEMU fournit une zone RAM suffisante
 * pour le kernel et ses structures.
 */

#define MYOS_MEMORY_START 0x41000000UL
#define MYOS_MEMORY_END   0x42000000UL

#define MYOS_PAGE_SIZE 4096UL

#define MYOS_MAX_TASKS 16

#define MYOS_MAX_FILES 32

#define MYOS_KERNEL_VERSION_MAJOR 0
#define MYOS_KERNEL_VERSION_MINOR 1

#endif
