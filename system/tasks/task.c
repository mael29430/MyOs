/*
 * MyOS - Task Manager
 *
 * Première version du scheduler ARM64.
 */

#include <stdint.h>

#define MAX_TASKS 16

typedef enum
{
    TASK_UNUSED = 0,
    TASK_READY,
    TASK_RUNNING,
    TASK_BLOCKED
} task_state_t;

typedef struct
{
    uint64_t id;
    task_state_t state;
    uint64_t ticks;
} task_t;

static task_t tasks[MAX_TASKS];

static uint64_t task_count = 0;
static uint64_t current_task = 0;

/*
 * Initialise le gestionnaire de tâches.
 */
void task_init(void)
{
    for (uint64_t i = 0; i < MAX_TASKS; i++)
    {
        tasks[i].id = 0;
        tasks[i].state = TASK_UNUSED;
        tasks[i].ticks = 0;
    }

    task_count = 0;
    current_task = 0;
}

/*
 * Crée une nouvelle tâche.
 */
uint64_t task_create(void)
{
    if (task_count >= MAX_TASKS)
    {
        return 0;
    }

    for (uint64_t i = 0; i < MAX_TASKS; i++)
    {
        if (tasks[i].state == TASK_UNUSED)
        {
            tasks[i].id = i + 1;
            tasks[i].state = TASK_READY;
            tasks[i].ticks = 0;

            task_count++;

            return tasks[i].id;
        }
    }

    return 0;
}

/*
 * Met une tâche en état READY.
 */
void task_ready(uint64_t id)
{
    for (uint64_t i = 0; i < MAX_TASKS; i++)
    {
        if (tasks[i].id == id)
        {
            tasks[i].state = TASK_READY;
            return;
        }
    }
}

/*
 * Bloque une tâche.
 */
void task_block(uint64_t id)
{
    for (uint64_t i = 0; i < MAX_TASKS; i++)
    {
        if (tasks[i].id == id)
        {
            tasks[i].state = TASK_BLOCKED;
            return;
        }
    }
}

/*
 * Sélectionne la prochaine tâche prête.
 */
uint64_t task_schedule(void)
{
    if (task_count == 0)
    {
        return 0;
    }

    for (uint64_t offset = 1; offset <= MAX_TASKS; offset++)
    {
        uint64_t index =
            (current_task + offset) % MAX_TASKS;

        if (tasks[index].state == TASK_READY)
        {
            current_task = index;
            tasks[index].state = TASK_RUNNING;

            return tasks[index].id;
        }
    }

    return tasks[current_task].id;
}

/*
 * Retourne l'identifiant de la tâche actuelle.
 */
uint64_t task_current(void)
{
    return tasks[current_task].id;
}

/*
 * Appelé périodiquement par le timer.
 */
void task_tick(void)
{
    if (task_count == 0)
    {
        return;
    }

    tasks[current_task].ticks++;

    /*
     * Pour l'instant, on ne change pas encore
     * réellement le contexte CPU.
     *
     * Cette partie viendra avec le contexte ARM64.
     */
}
