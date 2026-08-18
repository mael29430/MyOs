/*
 * MyOS Timer
 *
 * Timer système ARM64.
 */

static volatile unsigned long timer_ticks = 0;


/*
 * Initialise le timer.
 *
 * Cette première version utilise
 * un compteur logiciel.
 */

void timer_init(void)
{
    timer_ticks = 0;
}


/*
 * Incrémente le compteur.
 *
 * Le véritable interrupt timer sera
 * connecté dans une version suivante.
 */

void timer_tick(void)
{
    timer_ticks++;
}


/*
 * Retourne le nombre de ticks.
 */

unsigned long timer_get_ticks(void)
{
    return timer_ticks;
}
