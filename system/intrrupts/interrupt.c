/*
 * MyOS - ARM64 Interrupt Manager
 */

#include <stdint.h>

/*
 * Scheduler
 */
extern void task_tick(void);
extern void *task_switch_frame(void *frame);

/*
 * Compteur d'interruptions.
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
 * Gestionnaire principal.
 *
 * vectors.S lui transmet l'adresse du
 * exception frame.
 *
 * Il retourne l'adresse du frame que
 * vectors.S devra restaurer.
 */
void *interrupt_handler(void *frame)
{
    interrupt_count++;

    /*
     * Mise à jour du scheduler.
     */
    task_tick();

    /*
     * Sauvegarde de la tâche actuelle
     * et sélection de la suivante.
     */
    return task_switch_frame(frame);
}


/*
 * Retourne le nombre d'interruptions.
 */
uint64_t interrupt_get_count(void)
{
    return interrupt_count;
}
