/*
 * MyOS - ARM64 Interrupt Manager
 */

#include <stdint.h>

/*
 * Gestionnaire de tâches.
 */
extern void task_tick(void);

/*
 * Compteur global d'interruptions.
 */
static volatile uint64_t interrupt_count = 0;


/*
 * Initialise le gestionnaire d'interruptions.
 */
void interrupt_init(void)
{
    interrupt_count = 0;
}


/*
 * Gestionnaire appelé par les vecteurs ARM64.
 *
 * Pour l'instant :
 *   1. compte l'interruption ;
 *   2. informe le gestionnaire de tâches.
 *
 * Le changement de contexte réel viendra
 * dans l'étape suivante.
 */
void interrupt_handler(void)
{
    interrupt_count++;

    task_tick();
}


/*
 * Retourne le nombre d'interruptions reçues.
 */
uint64_t interrupt_get_count(void)
{
    return interrupt_count;
}
