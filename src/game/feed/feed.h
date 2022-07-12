
#ifndef SERVER_FEED_H
#define SERVER_FEED_H

#include <stdlib.h>
#include <stdio.h>

#include "../../headers/dec.h"

/**
 * Definice struktury potarvy.
 */
typedef struct the_feed {
    int id;         /** ID. */
    double x_pos;   /** X-ova souradnice. */
    double y_pos;   /** Y-ova souradnice. */
    double radius;  /** Polomer. */
    int color;      /** Cislo barvy. */
} feed;

/**
 * Vytvori pole struktury feed o velikosti NUM_OF_FEEDS
 * @return Ukazatel na prvni prvek pole.
 */
feed * create_feeds();

/**
 * Odstrani pole potravy.
 * @param poor Ukazatel na ukazatel na prvni prvek pole.
 */
void destroy_feeds(feed **poor);

/**
 * Priradi potrave nove souradnice, a barvu.
 * @param feeds Ukazatel na ukazatel na pole potravy.
 * @param id Index potravy.
 */
void respawn_feed(feed **feeds, int id);

/**
 * Vygeneruje nove informace o potrave.
 * @return Prvek s generovanymi informacemi.
 */
feed spawn_feed();
#endif
