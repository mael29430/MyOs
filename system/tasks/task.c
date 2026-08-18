/*
 * MyOS Task Manager
 *
 * Gestion minimale des tâches.
 */

#include "../config.h"

typedef void (*task_entry_t)(void);


typedef enum
{
    TASK_UNUSED = 0,
    TASK_READY,
    TASK_RUNNING,
    TASK_STOPPED
} task_state_t;


typedef struct
{
    unsigned long id;

    task_state_t state;

    task_entry_t entry;

} task_t;


static task_t tasks[MYOS_MAX_TASKS];

static unsigned long current_task = 0;


/*
 * Initialise le gestionnaire
 * des tâches.
 */

void task_init(void)
{
    unsigned long i;

    for (i = 0; i < MYOS_MAX_TASKS; i++)
    {
        tasks[i].id = i;
        tasks[i].state = TASK_UNUSED;
        tasks[i].entry = (task_entry_t)0;
    }

    current_task = 0;
}


/*
 * Crée une tâche.
 */

int task_create(task_entry_t entry)
{
    unsigned long i;

    for (i = 0; i < MYOS_MAX_TASKS; i++)
    {
        if (tasks[i].state == TASK_UNUSED)
        {
            tasks[i].entry = entry;
            tasks[i].state = TASK_READY;

            return (int)i;
        }
    }

    return -1;
}


/*
 * Arrête une tâche.
 */

void task_stop(unsigned long id)
{
    if (id >= MYOS_MAX_TASKS)
    {
        return;
    }

    tasks[id].state = TASK_STOPPED;
}


/*
 * Retourne la tâche courante.
 */

unsigned long task_current(void)
{
    return current_task;
}


/*
 * Scheduler coopératif minimal.
 */

void task_schedule(void)
{
    unsigned long i;

    for (i = 0; i < MYOS_MAX_TASKS; i++)
    {
        if (tasks[i].state == TASK_READY)
        {
            current_task = i;

            tasks[i].state = TASK_RUNNING;

            if (tasks[i].entry)
            {
                tasks[i].entry();
            }

            tasks[i].state = TASK_STOPPED;
        }
    }
}
