/*
 * MyOS - ARM64 Interrupt Manager
 */

#include <stdint.h>

/*
 * Gestionnaire de tâches.
 */
extern void task_tick(void);
extern uint64_t task_switch(void);

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
 * Gestionnaire principal des interruptions.
 */
void interrupt_handler(void)
{
    interrupt_count++;

    /*
     * Informe le scheduler qu'un tick est arrivé.
     */
    task_tick();

    /*
     * Sélectionne éventuellement une autre tâche.
     *
     * La restauration du contexte CPU sera ajoutée
     * dans l'étape suivante.
     */
    task_switch();
}


/*
 * Retourne le nombre d'interruptions reçues.
 */
uint64_t interrupt_get_count(void)
{
    return interrupt_count;
}
