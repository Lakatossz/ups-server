
#include "room.h"
#include "../../headers/glob.h"

room * create_rooms(int num_of_rooms, int max_clients)
{
    if(num_of_rooms < 1 || max_clients < 1) {
        return NULL;
    }

    room *new_room = (room *)malloc((num_of_rooms + 1) * sizeof(room));
    if(!new_room) {
        return NULL;
    }

    new_room[0].id = 0;
    new_room[0].capacity = (*s).max_clients;
    new_room[0].num_of_players = 0;
    new_room[0].feeds = NULL;
    new_room[0].num_of_feeds = 0;
    new_room[0].start_time = 0;
    new_room[0].room_state = ROOM_STATE_EMPTY;

    for(int i = 1; i <= num_of_rooms; ++i) {
        new_room[i].id = i;
        new_room[i].capacity = MAX_IN_ROOM;
        new_room[i].num_of_players = 0;
        new_room[i].feeds = NULL;
        new_room[i].num_of_feeds = 0;
        new_room[i].start_time = 0;
        new_room[i].room_state = ROOM_STATE_EMPTY;
    }
    return new_room;
}

void spawn_feeds_in_room(int room_index)
{
    if(room_index < 1) {
        return;
    }

    if((*s).rooms[room_index].num_of_players == 0) {
        (*s).rooms[room_index].num_of_feeds = NUM_OF_FEEDS;
        (*s).rooms[room_index].feeds = create_feeds();
        printf("Spawnul jsem feed v mistnosti %d\n", room_index);
    }
}

void clear_feeds_in_room(int room_index)
{
    if(room_index < 1) {
        return;
    }

    if((*s).rooms[room_index].num_of_players == 0) {
        destroy_feeds(&(*s).rooms[room_index].feeds);
        (*s).rooms[room_index].num_of_feeds = 0;
        printf("Odstranil jsem feed v mistnosti %d\n", room_index);
    }
}

int increase_clients(int room_index)
{
    if(room_index < 0 || room_index > (*s).num_of_rooms) {
        return EXIT_FAILURE;
    }

    if(room_index > 0) {
        if((*s).rooms[room_index].num_of_players == 0 && (*s).rooms[room_index].room_state == ROOM_STATE_EMPTY) {
            (*s).rooms[room_index].room_state = ROOM_STATE_WAITING;
            (*s).rooms[room_index].start_time = time(NULL);
            spawn_feeds_in_room(room_index);
        }

        if((*s).rooms[room_index].num_of_players == 1 && (*s).rooms[room_index].room_state == ROOM_STATE_WAITING) {
            if((time(NULL) - (*s).rooms[room_index].start_time) > ROOM_COOLDOWN) {
                (*s).rooms[room_index].room_state = ROOM_STATE_RUNNING;
            }
        }
    }

    if((*s).rooms[room_index].num_of_players == 1 && ((*s).rooms[room_index].room_state == ROOM_STATE_RUNNING || (*s).rooms[room_index].room_state == ROOM_STATE_WON)) {
        return EXIT_FAILURE;
    }

    if((*s).rooms[room_index].capacity > (*s).rooms[room_index].num_of_players) {
        ++(*s).rooms[room_index].num_of_players;
        printf("Pocet klientu v mistnosti %d po pricteni: %d\n", room_index, (*s).rooms[room_index].num_of_players);
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}

void decrease_clients(int room_index)
{
    if(room_index < 0 || room_index > (*s).num_of_rooms) {
        return;
    }

    if((*s).rooms[room_index].num_of_players > 0) {
        --(*s).rooms[room_index].num_of_players;
    }

    if(room_index > 0) {
        if((*s).rooms[room_index].num_of_players == 0) {
            (*s).rooms[room_index].room_state = ROOM_STATE_EMPTY;
            clear_feeds_in_room(room_index);
            printf("Hra v mistnosti %d skoncila.\n", room_index);
        } else {
            printf("Pocet klientu v mistnosti %d po odecteni: %d\n", room_index, (*s).rooms[room_index].num_of_players);
        }
    }
}

int respawn_feed_in_room(int room_index, int feed_index)
{
    if(room_index < 1 || room_index > (*s).num_of_rooms || feed_index < 0 || feed_index > NUM_OF_FEEDS) {
        return EXIT_FAILURE;
    }

    respawn_feed(&(*s).rooms[room_index].feeds, feed_index);
    return EXIT_SUCCESS;
}

void room_info(int room_index)
{
    printf("ID mistnosti: %d\n", (*s).rooms[room_index].id);
    printf("kapacita: %d\n", (*s).rooms[room_index].capacity);
    printf("pocet hracu: %d\n", (*s).rooms[room_index].num_of_players);
    printf("pocet potravy: %d\n", (*s).rooms[room_index].num_of_feeds);
}

void cooldown_passed(int room_index)
{
    if(room_index < 1 || room_index > (*s).num_of_rooms || (*s).rooms[room_index].num_of_players == 0) {
        return;
    }

    time_t new_time = time(NULL);

    if((*s).rooms[room_index].num_of_players == 0 && (*s).rooms[room_index].room_state != ROOM_STATE_EMPTY) {
        (*s).rooms[room_index].room_state = ROOM_STATE_EMPTY;
    }

    if(((*s).rooms[room_index].room_state == ROOM_STATE_WAITING || (*s).rooms[room_index].num_of_players == 1) && new_time - (*s).rooms[room_index].start_time > ROOM_COOLDOWN - 1) {
        (*s).rooms[room_index].room_state = ROOM_STATE_RUNNING;
        printf("Hra v mistnosti %d prave zacala!\n", room_index);
    }
}

void end_game_in_room(int room_index)
{
    if(room_index < 0) {
        return;
    }
    decrease_clients(room_index);
    increase_clients(LOBBY_ID);
}

void destroy_rooms(room ** poor, int num_of_rooms)
{
    if(!poor || !*poor || num_of_rooms <= 0) {
        return;
    }

    for(int i = 0; i < num_of_rooms; ++i) {
        (*poor)[i].id = 0;
        (*poor)[i].capacity = 0;
        (*poor)[i].num_of_players = 0;
        if((*poor)[i].feeds != NULL) {
            free((*poor)[i].feeds);
            (*poor)[i].feeds = NULL;
        }
        (*poor)[i].room_state = 0;
    }

    free(*poor);
    *poor = NULL;
}


