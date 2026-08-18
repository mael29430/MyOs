/*
 * MyOS - ARM64 Interrupt Manager
 */

#include <stdint.h>

extern void task_tick(void);
extern uint64_t task_switch(void);

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
 *
 * frame contient le contexte CPU sauvegardé
 * par vectors.S.
 */
void interrupt_handler(void *frame)
{
    /*
     * Pour l'instant, le frame est uniquement
     * préparé pour le futur changement de contexte.
     */
    (void)frame;

    interrupt_count++;

    task_tick();

    /*
     * Sélectionne la prochaine tâche.
     *
     * IMPORTANT :
     * on ne restaure pas encore son contexte.
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
