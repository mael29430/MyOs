/*
 * MyOS - ARM64 Task Manager
 *
 * Task scheduler and context switching test.
 */

#include <stdint.h>

#define MAX_TASKS 16
#define TASK_STACK_SIZE 4096

#define TASK_UNUSED   0
#define TASK_READY    1
#define TASK_RUNNING  2
#define TASK_BLOCKED  3

#define EXCEPTION_FRAME_SIZE 272


/*
 * ARM64 exception frame.
 *
 * +0    x0
 * +8    x1
 * ...
 * +240  x30
 * +248  ELR_EL1
 * +256  SPSR_EL1
 */
typedef struct
{
    uint64_t x[31];
    uint64_t elr_el1;
    uint64_t spsr_el1;
} exception_frame_t;


typedef struct
{
    uint64_t id;
    uint64_t state;
    uint64_t ticks;

    uint8_t stack[TASK_STACK_SIZE];

    exception_frame_t *frame;

} task_t;


static task_t tasks[MAX_TASKS];

static uint64_t task_count = 0;
static uint64_t current_task = MAX_TASKS;

static uint64_t scheduler_ticks = 0;


/*
 * Compteurs de test.
 */
static volatile uint64_t task1_counter = 0;
static volatile uint64_t task2_counter = 0;


/*
 * Console.
 */
extern void console_write(const char *text);


/*
 * Tâche 1.
 */
static void task1_entry(void)
{
    for (;;)
    {
        task1_counter++;

        if ((task1_counter & 0xFFFFF) == 0)
        {
            console_write("[TASK 1]\n");
        }

        __asm__ volatile("wfe");
    }
}


/*
 * Tâche 2.
 */
static void task2_entry(void)
{
    for (;;)
    {
        task2_counter++;

        if ((task2_counter & 0xFFFFF) == 0)
        {
            console_write("[TASK 2]\n");
        }

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
        tasks[i].frame = 0;
    }

    task_count = 0;
    current_task = MAX_TASKS;
    scheduler_ticks = 0;

    task1_counter = 0;
    task2_counter = 0;
}


/*
 * Prépare le contexte initial d'une tâche.
 */
static void task_prepare_frame(
    task_t *task,
    void (*entry)(void)
)
{
    uint64_t stack_top =
        (uint64_t)&task->stack[TASK_STACK_SIZE];

    stack_top &= ~((uint64_t)0xF);

    uint64_t frame_address =
        stack_top - EXCEPTION_FRAME_SIZE;

    frame_address &= ~((uint64_t)0xF);

    task->frame =
        (exception_frame_t *)frame_address;

    uint64_t *frame =
        (uint64_t *)task->frame;

    for (uint64_t i = 0; i < 34; i++)
    {
        frame[i] = 0;
    }

    /*
     * PC initial.
     */
    task->frame->elr_el1 =
        (uint64_t)entry;

    /*
     * AArch64 / EL1h.
     */
    task->frame->spsr_el1 = 0x345;
}


/*
 * Création interne d'une tâche.
 */
static uint64_t task_create_entry(
    void (*entry)(void)
)
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

            task_prepare_frame(
                &tasks[i],
                entry
            );

            task_count++;

            return tasks[i].id;
        }
    }

    return 0;
}


/*
 * Création d'une tâche classique.
 */
uint64_t task_create(void)
{
    return task_create_entry(task1_entry);
}


/*
 * Création des deux tâches de test.
 */
void task_create_test_tasks(void)
{
    task_create_entry(task1_entry);
    task_create_entry(task2_entry);
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
 * Choisit la prochaine tâche.
 */
uint64_t task_schedule(void)
{
    if (task_count == 0)
    {
        return 0;
    }

    uint64_t start;

    if (current_task >= MAX_TASKS)
    {
        start = MAX_TASKS - 1;
    }
    else
    {
        start = current_task;
    }

    for (uint64_t offset = 1;
         offset <= MAX_TASKS;
         offset++)
    {
        uint64_t index =
            (start + offset) % MAX_TASKS;

        if (tasks[index].state == TASK_READY)
        {
            if (current_task < MAX_TASKS &&
                tasks[current_task].state == TASK_RUNNING)
            {
                tasks[current_task].state =
                    TASK_READY;
            }

            current_task = index;

            tasks[current_task].state =
                TASK_RUNNING;

            return tasks[current_task].id;
        }
    }

    if (current_task < MAX_TASKS)
    {
        return tasks[current_task].id;
    }

    return 0;
}


/*
 * Tâche actuellement sélectionnée.
 */
uint64_t task_current(void)
{
    if (current_task >= MAX_TASKS)
    {
        return 0;
    }

    return tasks[current_task].id;
}


/*
 * Appelé à chaque tick du timer.
 */
void task_tick(void)
{
    scheduler_ticks++;

    if (current_task < MAX_TASKS)
    {
        tasks[current_task].ticks++;
    }
}


/*
 * Nombre de ticks.
 */
uint64_t task_get_ticks(void)
{
    return scheduler_ticks;
}


/*
 * Copie un exception frame.
 */
static void task_copy_frame(
    exception_frame_t *destination,
    const exception_frame_t *source
)
{
    const uint64_t *src =
        (const uint64_t *)source;

    uint64_t *dst =
        (uint64_t *)destination;

    for (uint64_t i = 0; i < 34; i++)
    {
        dst[i] = src[i];
    }
}


/*
 * Commutation de contexte.
 */
void *task_switch_frame(void *frame)
{
    /*
     * Première activation.
     */
    if (current_task >= MAX_TASKS)
    {
        task_schedule();

        if (current_task < MAX_TASKS)
        {
            return tasks[current_task].frame;
        }

        return frame;
    }

    /*
     * Sauvegarde de la tâche courante.
     */
    task_copy_frame(
        tasks[current_task].frame,
        (const exception_frame_t *)frame
    );

    /*
     * Sélection de la prochaine tâche.
     */
    uint64_t old_task =
        current_task;

    uint64_t next =
        task_schedule();

    if (next == 0)
    {
        return frame;
    }

    /*
     * Si on a réellement changé de tâche,
     * on affiche périodiquement le changement.
     */
    if (current_task != old_task)
    {
        if (tasks[current_task].id == 1)
        {
            console_write("[TASK 1]\n");
        }
        else if (tasks[current_task].id == 2)
        {
            console_write("[TASK 2]\n");
        }
    }

    /*
     * Frame de la nouvelle tâche.
     */
    return tasks[current_task].frame;
}


/*
 * Compteur tâche 1.
 */
uint64_t task1_get_counter(void)
{
    return task1_counter;
}


/*
 * Compteur tâche 2.
 */
uint64_t task2_get_counter(void)
{
    return task2_counter;
}
