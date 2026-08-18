#include <stdint.h>

#define MAX_TASKS 16
#define TASK_STACK_SIZE 4096

#define TASK_UNUSED   0
#define TASK_READY    1
#define TASK_RUNNING  2
#define TASK_BLOCKED  3

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

    uint64_t elr_el1;
    uint64_t spsr_el1;
} task_context_t;

typedef struct
{
    uint64_t id;
    uint64_t state;
    uint64_t ticks;

    task_context_t context;

    uint8_t stack[TASK_STACK_SIZE];
} task_t;

static task_t tasks[MAX_TASKS];

static uint64_t task_count = 0;
static uint64_t current_task = 0;
static uint64_t scheduler_ticks = 0;


/*
 * Point d'entrée d'une tâche de test.
 *
 * Pour l'instant les tâches restent dans leur boucle
 * afin de pouvoir tester leur changement plus tard.
 */
static void task_idle_entry(void)
{
    for (;;)
    {
        __asm__ volatile("wfe");
    }
}


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

        tasks[i].context.elr_el1 = 0;
        tasks[i].context.spsr_el1 = 0;
    }

    task_count = 0;
    current_task = 0;
    scheduler_ticks = 0;
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
            uint64_t stack_top =
                (uint64_t)&tasks[i].stack[TASK_STACK_SIZE];

            /*
             * Alignement ARM64 de la pile sur 16 octets.
             */
            stack_top &= ~((uint64_t)0xF);

            tasks[i].id = i + 1;
            tasks[i].state = TASK_READY;
            tasks[i].ticks = 0;

            /*
             * Contexte initial.
             */
            tasks[i].context.sp = stack_top;
            tasks[i].context.x30 =
                (uint64_t)task_idle_entry;

            tasks[i].context.elr_el1 =
                (uint64_t)task_idle_entry;

            /*
             * État EL1 avec les interruptions activées
             * au retour d'exception.
             */
            tasks[i].context.spsr_el1 = 0;

            task_count++;

            if (task_count == 1)
            {
                current_task = i;
                tasks[i].state = TASK_RUNNING;
            }

            return tasks[i].id;
        }
    }

    return 0;
}


/*
 * Met une tâche en READY.
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
 * Sélectionne la prochaine tâche READY.
 */
uint64_t task_schedule(void)
{
    if (task_count == 0)
    {
        return 0;
    }

    for (uint64_t offset = 1;
         offset <= MAX_TASKS;
         offset++)
    {
        uint64_t index =
            (current_task + offset) % MAX_TASKS;

        if (tasks[index].state == TASK_READY)
        {
            if (tasks[current_task].state == TASK_RUNNING)
            {
                tasks[current_task].state = TASK_READY;
            }

            current_task = index;
            tasks[current_task].state = TASK_RUNNING;

            return tasks[current_task].id;
        }
    }

    return tasks[current_task].id;
}


/*
 * Retourne la tâche courante.
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
 * Tick du scheduler.
 */
void task_tick(void)
{
    scheduler_ticks++;

    if (task_count == 0)
    {
        return;
    }

    tasks[current_task].ticks++;
}


/*
 * Nombre total de ticks.
 */
uint64_t task_get_ticks(void)
{
    return scheduler_ticks;
}


/*
 * Sauvegarde le contexte courant.
 */
extern int task_context_save(task_context_t *context);

int task_save_current_context(void)
{
    if (task_count == 0)
    {
        return -1;
    }

    return task_context_save(
        &tasks[current_task].context
    );
}


/*
 * Restaure le contexte courant.
 */
extern void task_context_restore(task_context_t *context);

void task_restore_current_context(void)
{
    if (task_count == 0)
    {
        return;
    }

    task_context_restore(
        &tasks[current_task].context
    );
}


/*
 * Sélectionne la prochaine tâche.
 */
uint64_t task_switch(void)
{
    uint64_t old_task = task_current();

    uint64_t new_task = task_schedule();

    if (new_task == 0)
    {
        return 0;
    }

    if (old_task == new_task)
    {
        return new_task;
    }

    return new_task;
}
