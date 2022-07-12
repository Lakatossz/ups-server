
#include "config_server.h"

void die(const char* chr, int err) {
    printf("chyba: %s\r\n", chr);
    exit(err);
}

server * create_server()
{
    int skt, res, param = 1;

    server *new_server = (server *) malloc(sizeof(server));
    if(!new_server) {
        return NULL;
    }

    skt = socket(AF_INET, SOCK_STREAM, 0);
    if(skt < 0) {
        die("Cannot make socket.\n", ERR_SOCK);
        return NULL;
    }

    (*new_server).socket = skt;

    memset(&(*new_server).addr, 0, sizeof(struct sockaddr_in));

    (*new_server).addr.sin_family = AF_INET;
    (*new_server).addr.sin_port = htons(DEFAULT_PORT);
    (*new_server).addr.sin_addr.s_addr = INADDR_ANY;


    res = setsockopt((*new_server).socket, SOL_SOCKET, SO_REUSEADDR, (const char *)&param, sizeof(int));
    if(res == -1) {
        printf("setsockopt ERR\n");
        return NULL;
    }

    (*new_server).clients = NULL;

    (*new_server).highest_id = 0;
    (*new_server).num_of_clients = 0;
    (*new_server).max_clients = DEFAULT_MAX_PLAYERS;
    (*new_server).num_of_rooms = DEFAULT_MAX_ROOMS;
    (*new_server).running = true;

    printf("Server has been created.\n");

    return new_server;
}

void set_server(server **s, int max_players, int num_of_rooms, int port, char *address)
{
    int skt, res, param = 1;

    if(!s || !*s) {
        return;
    }

    skt = socket(AF_INET, SOCK_STREAM, 0);
    if(skt < 0) {
        die("Cannot make socket.\n", ERR_SOCK);
        return;
    }

    (*s)->socket = skt;

    res = setsockopt((*s)->socket, SOL_SOCKET, SO_REUSEADDR, (const char *)&param, sizeof(int));
    if(res == -1) {
        return;
    }

    (*s)->addr.sin_family = AF_INET;
    (*s)->addr.sin_port = htons(port);
    if(strlen(address) != 0) {
        (*s)->addr.sin_addr.s_addr = inet_addr(address);
    }

    (*s)->max_clients = max_players;
    (*s)->num_of_rooms = num_of_rooms;

    printf("Server has been set.\n");
}

void destroy_server(server **s)
{
    if(!s || !*s) {
        return;
    }

    memset((*s), 0, sizeof(server));

    if((**s).clients) {
        clientll_free(&(**s).clients);
        (**s).clients = NULL;
    }

    if((**s).rooms) {
        destroy_rooms(&(**s).rooms, (**s).num_of_rooms);
        (**s).rooms = NULL;
    }

    free(*s);
    *s = NULL;
}
