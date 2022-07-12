
#ifndef SERVER_ROOM_H
#define SERVER_ROOM_H

#include <stdlib.h>
#include <stdio.h>

#include "../../client/client.h"
#include "../feed/feed.h"

/**
 * Definice struktury mistnost.
 */
typedef struct the_room {
    int id;             /** Id mistnosti. */
    int capacity;       /** Maximalni pocet hracu mistnosti. */
    int num_of_players; /** Aktualni pocet hracu v mistnosti. */
    feed *feeds;        /** Potrava v roomce. */
    int num_of_feeds;   /** Aktualni pocet potravy v roomce. */
    time_t start_time;  /** Cas spusteni mistnosti. */
    int room_state;     /** Urcuje, jestli je cas v cooldownu. */
} room;

/**
 * Vytvori pole mistnosti.
 * @param num_of_rooms Pocet mistnosti (+ lobby).
 * @param max_clients Maximalni pocet klientu na serveru, tj. maximalni pocet hracu v lobby.
 * @return Ukazatel na pole struktury mistnost.
 */
room * create_rooms(int num_of_rooms, int max_clients);

/**
 * Spawne potravu v mistnosti.
 * @param room_index Index mistnosti.
 */
void spawn_feeds_in_room(int room_index);

/**
 * Odstrani potravu z mitsnosti.
 * @param rooms_index Index mistnosti.
 */
void clear_feeds_in_room(int rooms_index);

/**
 * Zvetsi pocet klientu v mistnosti.
 * @param room_index Index pole mistnosti.
 * @return 0 pokud probehlo vse v poradku, 1 jinak.
 */
int increase_clients(int room_index);

/**
 * Zmensi pocet klientu v mistnosti na idnexu.
 * @param room_index Index pole mistnosti.
 */
void decrease_clients(int room_index);

/**
 * Provede respwan potravy.
 * @param room_index Index misntosti.
 * @param feed_index Index potravy.
 * @return 0 pokud probehlo vse v poradku, 1 jinak.
 */
int respawn_feed_in_room(int room_index, int feed_index);

/**
 * Vypise informace o mistnosti.
 * @param room_index ndex mistnosti.
 */
void room_info(int room_index);

/**
 * Zmeni stav mistnosti, pokud ubehl pozadovany cas.
 * @param room_index Index mistnosti.
 */
void cooldown_passed(int room_index);

/**
 * Odebere posledniho hrace a tim ukonci hru, zaroven ho posle do lobby.
 * @param room_index Index mistnosti.
 */
void end_game_in_room(int room_index);

/**
 * Uvolni pole mistnosti.
 * @param poor Ukazatel na ukazatel na prvni prvek pole.
 * @param num_of_rooms Pocet mistnosti.
 */
void destroy_rooms(room ** poor, int num_of_rooms);

#endif
