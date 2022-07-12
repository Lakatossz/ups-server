
#include "client.h"

client * new_client(int id, int socket_number, const char *nickname, int room)
{
    client *c = NULL;

    c = (client *) malloc(sizeof(client));
    if(!c) {
        return NULL;
    }

    strncpy(c->nickname, nickname, strlen(nickname));
    (*c).id = id;
    (*c).socket_number = socket_number;
    (*c).room = room;
    (*c).bad_counter = 0;
    (*c).last_time = time_in_milli();
    (*c).alive = false;
    (*c).connected = true;

    return c;
}

void destroy_client(client **poor)
{
    if(!poor || !*poor) {
        return;
    }

    if(*poor != NULL) {
        free(*poor);
        *poor = NULL;
    }
}

void spawn_client(client *c, int room) {
    if(!c) {
        return;
    }

    srand(time(NULL));

    (*c).x_pos = (double) (rand() % GAME_WIDTH + 1);
    (*c).y_pos = (double) (rand() % GAME_HEIGHT + 1);
    (*c).radius = PLAYER_SPAWN_RADIUS;
    (*c).color = rand() % (MAX_COLOR + 1);
    (*c).room = room;
    (*c).alive = true;
}

long long time_in_milli()
{
    struct timeval curr_t;
    gettimeofday(&curr_t, NULL);
    long long milliseconds = curr_t.tv_sec * 1000LL + curr_t.tv_usec / 1000;
    return milliseconds;
}
