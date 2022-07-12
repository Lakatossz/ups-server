
#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include "../headers/dec.h"

/**
 * Definice struktury klienta.
 */
typedef struct theclient
{
    char nickname[MAX_NICK_LEN]; /** Nickname. */
    int id;                /** ID klienta. */
    int socket_number;     /** Cislo socketu, na kterem klient posloucha. */
    double radius;         /** Polomer hrace. */
    double x_pos;          /** X-ova souradnice hrace. */
    double y_pos;          /** Y-ova souradnice hrace. */
    int room;              /** Mistnost ve ktere se hrac nachazi. */
    int color;             /** Barva kulicky hrace. */
    int bad_counter;       /** Pocet nevaildnich zprav od hrace. */
    long long last_time;   /** Posledni cas, kdy se klient ozval. */
    bool alive;            /** Stav ziti. 1 - Zivy, 0 - Mrtvy. */
    bool connected;        /** Stav klienta. 1 - Pripojen, 0 - Docasne odpojen. */
} client;

/**
 * Vytvori noveho klienta.
 * @param id Id hrace.
 * @param socket_number Cislo socketu, na kterem klient posloucha.
 * @param nickname Hracova prezdivka.
 * @param room_index Cislo mistnosti, do ktere se klient pripoji (0. mistnost je lobby).
 * @return Struktura klienta.
 */
client * new_client(int id, int socket_number, const char *nickname, int room);

/**
 * Odstrani zaznam klienta ze serveru.
 * @param poor Ukazatel na ukazatel na odstranovaneho klienta.
 */
void destroy_client(client **poor);

/**
 * Priradi klientovy souradnice, polomer a abarvu, pro vstup do mistnosti room.
 * @param c Ukazatel na klienta.
 * @param room Id mistnosti.
 */
void spawn_client(client *c, int room);

/**
 * Vrati aktualni cas v sekundach.
 */
long long time_in_milli();

#endif
