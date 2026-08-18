/*
 * MyOS - Task Manager
 *
 * Gestion des tâches et préparation du contexte ARM64.
 */

#include <stdint.h>

#define MAX_TASKS 16

#define TASK_UNUSED   0
#define TASK_READY    1
#define TASK_RUNNING  2
#define TASK_BLOCKED  3

/*
 * Layout correspondant exactement à context.S :
 *
 *  0 : x19
 *  8 : x20
 * 16 : x21
 * 24 : x22
 * 32 : x23
 * 40 : x24
 * 48 : x25
 * 56 : x26
 * 64 : x27
 * 72 : x28
 * 80 : SP
 * 88 : X30
 */
typedef struct
{
    uint64_t x19;
    uint64_t x20;
    uint64_t x21;
    uint64_t x22;
    uint64_t x23;
    uint64_t x24;
    uint64_t x25;
    uint64_t x26;
    uint64_t x27;
    uint64_t x28;
    uint64_t sp;
    uint64_t x30;
} task_context_t;

typedef struct
{
    uint64_t id;
    uint64_t state;
    uint64_t ticks;

    task_context_t context;
} task_t;

static task_t tasks[MAX_TASKS];

static uint64_t task_count = 0;
static uint64_t current_task = 0;

extern int task_context_save(task_context_t *context);
extern void task_context_restore(task_context_t *context);


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

        tasks[i].context.x19 = 0;
        tasks[i].context.x20 = 0;
        tasks[i].context.x21 = 0;
        tasks[i].context.x22 = 0;
        tasks[i].context.x23 = 0;
        tasks[i].context.x24 = 0;
        tasks[i].context.x25 = 0;
        tasks[i].context.x26 = 0;
        tasks[i].context.x27 = 0;
        tasks[i].context.x28 = 0;
        tasks[i].context.sp = 0;
        tasks[i].context.x30 = 0;
    }

    task_count = 0;
    current_task = 0;
}


/*
 * Crée une tâche.
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

            tasks[i].context.x19 = 0;
            tasks[i].context.x20 = 0;
            tasks[i].context.x21 = 0;
            tasks[i].context.x22 = 0;
            tasks[i].context.x23 = 0;
            tasks[i].context.x24 = 0;
            tasks[i].context.x25 = 0;
            tasks[i].context.x26 = 0;
            tasks[i].context.x27 = 0;
            tasks[i].context.x28 = 0;
            tasks[i].context.sp = 0;
            tasks[i].context.x30 = 0;

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
            tasks[current_task].state = TASK_READY;

            current_task = index;

            tasks[current_task].state = TASK_RUNNING;

            return tasks[current_task].id;
        }
    }

    return tasks[current_task].id;
}


/*
 * Retourne la tâche actuelle.
 */
uint64_t task_current(void)
{
    if (task_count == 0)
    {
        return 0;
    }

    return tasks[current_task].id;
}


/*
 * Appelé par le timer.
 */
void task_tick(void)
{
    if (task_count == 0)
    {
        return;
    }

    tasks[current_task].ticks++;
}


/*
 * Sauvegarde le contexte de la tâche actuelle.
 *
 * Cette fonction sera utilisée lorsque les interruptions
 * permettront une vraie commutation de contexte.
 */
int task_save_current_context(void)
{
    if (task_count == 0)
    {
        return -1;
    }

    return task_context_save(&tasks[current_task].context);
}


/*
 * Restaure le contexte de la tâche actuelle.
 */
void task_restore_current_context(void)
{
    if (task_count == 0)
    {
        return;
    }

    task_context_restore(&tasks[current_task].context);
}
