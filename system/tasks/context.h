/*
 * MyOS - Generic Task Context
 */

#ifndef MYOS_TASK_CONTEXT_H
#define MYOS_TASK_CONTEXT_H

#include <stdint.h>

typedef struct
{
    uint64_t registers[31];
    uint64_t pc;
    uint64_t status;

} task_context_t;

void task_context_init(
    task_context_t *context,
    void (*entry)(void),
    uint64_t stack
);

void task_context_save(task_context_t *context);
void task_context_restore(task_context_t *context);

#endif
