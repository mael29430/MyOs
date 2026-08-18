/*
 * MyOS - ARM64 Interrupt Manager
 */

#include <stdint.h>

extern void task_tick(void);

/*
 * task_switch_frame() :
 *
 * - sauvegarde le frame de la tâche courante
 * - choisit la prochaine tâche
 * - retourne le frame à restaurer
 */
extern void *task_switch_frame(void *frame);

static volatile uint64_t interrupt_count = 0;

void interrupt_init(void)
{
    interrupt_count = 0;
}

void *interrupt_handler(void *frame)
{
    interrupt_count++;

    task_tick();

    /*
     * Le scheduler décide maintenant quel frame
     * doit être restauré par vectors.S.
     */
    return task_switch_frame(frame);
}

uint64_t interrupt_get_count(void)
{
    return interrupt_count;
}
