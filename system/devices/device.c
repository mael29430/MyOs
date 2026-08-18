/*
 * MyOS Device Manager
 *
 * Architecture générique des périphériques.
 */

#include "../config.h"


typedef enum
{
    DEVICE_UNUSED = 0,
    DEVICE_READY,
    DEVICE_ACTIVE,
    DEVICE_ERROR
} device_state_t;


typedef struct
{
    unsigned long id;

    const char *name;

    device_state_t state;

} device_t;


#define MYOS_MAX_DEVICES 16


static device_t devices[MYOS_MAX_DEVICES];

static unsigned long device_count = 0;


/*
 * Initialise le gestionnaire
 * de périphériques.
 */

void device_init(void)
{
    unsigned long i;

    device_count = 0;

    for (i = 0; i < MYOS_MAX_DEVICES; i++)
    {
        devices[i].id = i;
        devices[i].name = (const char *)0;
        devices[i].state = DEVICE_UNUSED;
    }
}


/*
 * Enregistre un périphérique.
 */

int device_register(const char *name)
{
    if (device_count >= MYOS_MAX_DEVICES)
    {
        return -1;
    }

    devices[device_count].name = name;

    devices[device_count].state = DEVICE_READY;

    device_count++;

    return (int)(device_count - 1);
}


/*
 * Retourne le nombre de périphériques.
 */

unsigned long device_count_get(void)
{
    return device_count;
}
