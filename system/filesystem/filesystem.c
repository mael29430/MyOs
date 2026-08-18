/*
 * MyOS File System
 *
 * Système de fichiers minimal.
 */

#include "../config.h"


typedef enum
{
    FILE_UNUSED = 0,
    FILE_CREATED
} file_state_t;


typedef struct
{
    unsigned long id;

    const char *name;

    file_state_t state;

} myos_file_t;


static myos_file_t files[MYOS_MAX_FILES];

static unsigned long file_count = 0;


/*
 * Initialise le système de fichiers.
 */

void filesystem_init(void)
{
    unsigned long i;

    file_count = 0;

    for (i = 0; i < MYOS_MAX_FILES; i++)
    {
        files[i].id = i;

        files[i].name = (const char *)0;

        files[i].state = FILE_UNUSED;
    }
}


/*
 * Crée un fichier logique.
 */

int filesystem_create(const char *name)
{
    if (file_count >= MYOS_MAX_FILES)
    {
        return -1;
    }

    files[file_count].name = name;

    files[file_count].state = FILE_CREATED;

    file_count++;

    return (int)(file_count - 1);
}


/*
 * Retourne le nombre de fichiers.
 */

unsigned long filesystem_count(void)
{
    return file_count;
}
