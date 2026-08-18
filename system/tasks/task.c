/*
 * MyOS - ARM64 Task Manager
 *
 * Préemption basée sur les exception frames ARM64.
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
 * Layout du frame créé par vectors.S
 *
 * +0    x0
 * +8    x1
 * ...
 * +240  x30
 * +248  ELR_EL1
 * +256  SPSR_EL1
 *
 * 264 octets utilisés.
 * Le frame occupe 272 octets pour conserver
 * l'alignement de pile ARM64.
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

    /*
     * Adresse du frame sauvegardé de cette tâche.
     */
    exception_frame_t *frame;

} task_t;


static task_t tasks[MAX_TASKS];

static uint64_t task_count = 0;

/*
 * MAX_TASKS signifie :
 * aucune tâche réelle n'est encore active.
 *
 * Le kernel courant est alors le "bootstrap".
 */
static uint64_t current_task = MAX_TASKS;

static uint64_t scheduler_ticks = 0;


/*
 * Compteurs simples pour vérifier que les tâches
 * sont réellement exécutées.
 */
static volatile uint64_t task1_counter = 0;
static volatile uint64_t task2_counter = 0;


/*
 * Première tâche de test.
 */
static void task1_entry(void)
{
    for (;;)
    {
        task1_counter++;

        __asm__ volatile("wfe");
    }
}


/*
 * Deuxième tâche de test.
 */
static void task2_entry(void)
{
    for (;;)
    {
        task2_counter++;

        __asm__ volatile("wfe");
    }
}


/*
 * Initialise le gestionnaire.
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
 * Prépare le frame initial d'une tâche.
 */
static void task_prepare_frame(
    task_t *task,
    void (*entry)(void)
)
{
    uint64_t stack_top =
        (uint64_t)&task->stack[TASK_STACK_SIZE];

    /*
     * Alignement ARM64.
     */
    stack_top &= ~((uint64_t)0xF);

    /*
     * Le frame est placé en bas de la pile.
     *
     * Après restauration :
     *
     *     SP = frame + 272
     *
     * ce qui donne une pile alignée.
     */
    uint64_t frame_address =
        stack_top - EXCEPTION_FRAME_SIZE;

    frame_address &= ~((uint64_t)0xF);

    task->frame =
        (exception_frame_t *)frame_address;

    /*
     * Efface le frame.
     */
    for (uint64_t i = 0; i < 34; i++)
    {
        ((uint64_t *)task->frame)[i] = 0;
    }

    /*
     * PC initial.
     */
    task->frame->elr_el1 =
        (uint64_t)entry;

    /*
     * Retour en EL1h, AArch64.
     *
     * M[3:0] = 0101 = EL1h
     *
     * DAIF :
     * D = 1
     * A = 1
     * I = 0  -> IRQ autorisées
     * F = 1
     *
     * => 0x345
     */
    task->frame->spsr_el1 = 0x345;

    /*
     * La première instruction de la tâche
     * utilisera une pile située juste après
     * son frame.
     */
}


/*
 * Crée une tâche avec un point d'entrée.
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
 * API publique conservée.
 */
uint64_t task_create(void)
{
    return task_create_entry(task1_entry);
}


/*
 * Crée les deux tâches de démonstration.
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
 * Sélectionne la prochaine tâche READY.
 */
uint64_t task_schedule(void)
{
    if (task_count == 0)
    {
        return 0;
    }

    /*
     * Premier passage depuis le bootstrap :
     * commencer par la tâche 0.
     */
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

    /*
     * Si aucune autre tâche n'est prête,
     * conserver la tâche actuelle.
     */
    if (current_task < MAX_TASKS)
    {
        return tasks[current_task].id;
    }

    return 0;
}


/*
 * Retourne l'ID de la tâche courante.
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
 * Tick scheduler.
 */
void task_tick(void)
{
    scheduler_ticks++;

    if (current_task < MAX_TASKS)
    {
        tasks[current_task].ticks++;
    }
}


uint64_t task_get_ticks(void)
{
    return scheduler_ticks;
}


/*
 * Copie un frame de 272 octets.
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

    /*
     * 272 / 8 = 34 mots.
     */
    for (uint64_t i = 0; i < 34; i++)
    {
        dst[i] = src[i];
    }
}


/*
 * Vrai changement de contexte.
 *
 * Retourne l'adresse du frame que vectors.S
 * doit restaurer.
 */
void *task_switch_frame(void *frame)
{
    /*
     * Aucun task actif :
     * on démarre la première tâche.
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
     * Sauvegarde de la tâche actuelle.
     */
    task_copy_frame(
        tasks[current_task].frame,
        (const exception_frame_t *)frame
    );

    /*
     * Choisit la suivante.
     */
    uint64_t next =
        task_schedule();

    if (next == 0)
    {
        return frame;
    }

    /*
     * Retourne directement le frame de
     * la nouvelle tâche.
     */
    return tasks[current_task].frame;
}


/*
 * Ces deux fonctions restent disponibles
 * pour le reste du kernel.
 */
uint64_t task1_get_counter(void)
{
    return task1_counter;
}


uint64_t task2_get_counter(void)
{
    return task2_counter;
}
