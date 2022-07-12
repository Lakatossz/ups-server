

#ifndef SERVER_SERVER_DEF_H
#define SERVER_SERVER_DEF_H

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>

#include "../client/list/linkedlist.h"
#include "../game/feed/feed.h"
#include "../game/room/room.h"

/**
 * Definice struktury server.
 */
typedef struct the_server
{
    int socket;                 /** Socket serveru. */
    fd_set master;              /** Filedescriptor serveru. */
    struct sockaddr_in addr;    /** Struktura sockaddr_in. */
    client_list_node *clients;  /** Ukazatel na spojovy seznam klientu. */
    int highest_id;             /** Nejvyssi ID klientu slouzi pro urceni ID dalsimu klientovi. */
    int num_of_clients;         /** Pocet klientu na serveru. */
    int max_clients;            /** Maximalni pocet klientu, kteri mohou byt v jeden cas na serveru. */
    room *rooms;                /** Ukazatel na pole mistnosti. */
    int num_of_rooms;           /** Pocet mistnosti. */
    bool running;               /** Urcuje, jetsli server bezi. */
} server;

#endif
