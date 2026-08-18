/*
 * MyOS Memory Manager
 *
 * Gestionnaire mémoire physique minimal.
 */

#include "../config.h"

typedef unsigned long size_t;


/*
 * Début et fin de la zone utilisée
 * par notre gestionnaire mémoire.
 */

static unsigned long memory_current =
    MYOS_MEMORY_START;


/*
 * Nombre de pages disponibles.
 */

static unsigned long memory_pages_total = 0;

static unsigned long memory_pages_used = 0;


/*
 * Initialise le gestionnaire mémoire.
 */

void memory_init(void)
{
    memory_current = MYOS_MEMORY_START;

    memory_pages_total =
        (MYOS_MEMORY_END - MYOS_MEMORY_START)
        / MYOS_PAGE_SIZE;

    memory_pages_used = 0;
}


/*
 * Alloue une page de 4096 octets.
 */

void *memory_page_alloc(void)
{
    unsigned long address;

    if (memory_current + MYOS_PAGE_SIZE >
        MYOS_MEMORY_END)
    {
        return (void *)0;
    }

    address = memory_current;

    memory_current += MYOS_PAGE_SIZE;

    memory_pages_used++;

    return (void *)address;
}


/*
 * Retourne le nombre total de pages.
 */

unsigned long memory_total_pages(void)
{
    return memory_pages_total;
}


/*
 * Retourne le nombre de pages utilisées.
 */

unsigned long memory_used_pages(void)
{
    return memory_pages_used;
}


/*
 * Retourne le nombre de pages libres.
 */

unsigned long memory_free_pages(void)
{
    return memory_pages_total -
           memory_pages_used;
}
