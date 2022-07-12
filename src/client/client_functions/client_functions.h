
#ifndef SERVER_CLIENT_FUNCTIONS_H
#define SERVER_CLIENT_FUNCTIONS_H

#include "../client.h"
#include "../list/linkedlist.h"
#include "../../headers/glob.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * Aktualizuje stav hrace.
 * @param id ID klienta.
 * @param radius Polomer kulicky.
 * @param x_pos X-ova souradnice.
 * @param y_pos Y-ova souradnice.
 */
void update_player(int id, double radius, double x_pos, double y_pos);

/**
 * Presune klienta do herni mistnosti a "spawne" ho.
 * @param id ID klienta.
 * @param room ID mistnosti, do ktere bude presunut
 */
void move_to_room(int id, int room);

/**
 * Presune klienta do lobby.
 * @param id ID klienta.
 */
void move_to_lobby(int id);

/**
 * Oznaci klienta jako neziveho.
 * @param id ID klienta.
 */
void kill_player(int id);

/**
 * Inkremetuje pocet nevaildnich zprav klienta, pokud to jde.
 * @param id ID klienta.
 * @return 0 pokud, ma jeste sanci, 1 jinak a bude odpojen.
 */
int increase_bad_counter_of_player(int id);

/**
 * Zmeni klientovi socket na kterem posloucha.
 * @param id ID klienta.
 * @param new_socket Cislo noveho socketu
 */
void reconnect_client(int id, int new_socket);


/**
 * Pokud se klient nahle odpoji a neprepoji se do 30 sec. je odpojen.
 * @param c Klient, ktery je odpojen.
 */
int temp_disconnection(client *c);

#endif
