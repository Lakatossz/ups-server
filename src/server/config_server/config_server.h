
#ifndef SERVER_CONFIG_SERVER_H
#define SERVER_CONFIG_SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>

#include "../../headers/dec.h"
#include "../server_def.h"
#include "../glob.h"

/**
 * Vytvoreni serveru.
 * @return Vytvoreny prvek struktury server.
 */
server * create_server();

/**
 * Nastavi server, pokud je potreba.
 * @param s Ukazatel na ukazatel na strukturu serveru.
 * @param max_players Maximalni pocet klientu na serveru.
 * @param num_of_rooms Pocet hernich mistnosti serveru.
 * @param port Port, na kterem server posloucha.
 * @param address Hostname serveru.
 */
void set_server(server **s, int max_players, int num_of_rooms, int port, char *address);

/**
 * Odstrani server.
 * @param s Prvek struktury server, jenz bude odstranen.
 */
void destroy_server(server **s);

#endif
