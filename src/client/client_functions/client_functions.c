
#include "client_functions.h"
#include "../../server/server_functions/server_functions.h"

void update_player(int id, double radius, double x_pos, double y_pos)
{
    if(id < 1 || radius < PLAYER_SPAWN_RADIUS || radius > PLAYER_MAX_RADIUS || x_pos < 0 || x_pos > GAME_WIDTH || y_pos < 0 || y_pos > GAME_HEIGHT) {
        return;
    }

    client *c = clientll_get_by_id((*s).clients, id);
    (*c).radius = radius;
    (*c).x_pos = x_pos;
    (*c).y_pos = y_pos;
    (*c).connected = true;
}

void move_to_room(int id, int room)
{
    if(id < 1 || room < 1 || room > (*s).num_of_rooms) {
        return;
    }
    spawn_client(clientll_get_by_id((*s).clients, id), room);
}

void move_to_lobby(int id)
{
    if(id < 1) {
        return;
    }
    clientll_get_by_id((*s).clients, id)->room = LOBBY_ID;
}

void kill_player(int id)
{
    if(id < 1) {
        return;
    }
    clientll_get_by_id((*s).clients, id)->alive = false;
    move_to_lobby(id);
}

int increase_bad_counter_of_player(int client_socket)
{
    if(client_socket < 4) {
        return EXIT_FAILURE;
    }

    client *c = clientll_get_by_socket((*s).clients, client_socket);

    if((*c).bad_counter < MAX_BAD_ANSWERS) {
        (*c).bad_counter++;
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

void reconnect_client(int id, int new_socket)
{
    if(id < 1 || new_socket < 4) {
        return;
    }

    int old_socket = clientll_get_by_id((*s).clients, id)->socket_number;

    clientll_get_by_id((*s).clients, id)->socket_number = new_socket;
    clientll_get_by_id((*s).clients, id)->connected = true;

    printf("Hrac s id %d nyni posloucha na socketu %d.\n", id, clientll_get_by_id((*s).clients, id)->socket_number);

    close_socket(old_socket);
    FD_CLR(old_socket, &(*s).master);
    printf("Zavrel jsem socket, protoze se klient prepojil na jiny.\n");
}

int temp_disconnection(client *c)
{
    if(!c) {
        return EXIT_FAILURE;
    }

    if((*c).connected == false && time_in_milli() - (*c).last_time > TIMEOUT) {
        return EXIT_DISCONNECT;
    } else if((*c).connected == true) {
        (*c).connected = false;
        printf("Klient s id %d je ve stavu \"nedostupny\".\n", (*c).id);
    }
    return EXIT_SUCCESS;
}
