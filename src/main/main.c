
#include "../headers/glob.h"
#include "../server/config_server/config_server.h"
#include "../server/server.h"

server *s;

int main(int argc, char *argv[])
{
    int num_of_rooms = DEFAULT_MAX_ROOMS;
    int max_players = DEFAULT_MAX_PLAYERS;
    int port = DEFAULT_PORT;
    char address[MAX_HOSTNAME_LENGTH] = { 0 };
    int par;

    bool edit_server = false;

    /** Hostname na server *
    strcat(address, DEFAULT_ADDRESS);*/

    for(par = 0; par < argc; ++par)
    {
        switch(argv[par][1]) {
            case 'a':
                ++par;
                if(strlen(argv[par]) < MIN_HOSTNAME_LENGTH) {
                    printf("If you want to change hostname of server it has to contain some character (At least more than %d)!\n", MIN_HOSTNAME_LENGTH);
                    exit(ERR_HOST_G);
                } else if(strlen(argv[par]) > MAX_HOSTNAME_LENGTH) {
                    printf("Given hostname is too long, it has to be max %d characters length!\n", MAX_HOSTNAME_LENGTH);
                    exit(ERR_HOST_L);
                } else {
                    memset(address, 0, strlen(address));
                    strncpy(address, argv[par], strlen(argv[par]));
                    printf("You have set hostname of server to: %s\n", address);
                    edit_server = true;
                }
                break;
            case 'p':
                if((int) strtol(argv[++par], NULL, 10) < MIN_PORT) {
                    printf("Port must be a number greater than %d!\n", MIN_PORT);
                    exit(ERR_PORT_G);
                } else if((int) strtol(argv[par], NULL, 10) > MAX_PORT) {
                    printf("Port must be a number lesser then %d!\n", MAX_PORT);
                    exit(ERR_PORT_L);
                } else {
                    port = (int) strtol(argv[par], NULL, 10);
                    printf("You have set port of server to: %d\n", port);
                    edit_server = true;
                }
                break;
            case 'r':
                if((int) strtol(argv[++par], NULL, 10) < MIN_ROOMS) {
                    printf("If you want to run a game you need at least %d room!\n", MIN_ROOMS);
                    exit(ERR_ROOM_G);
                } else if((int) strtol(argv[par], NULL, 10) > MAX_ROOMS) {
                    printf("Server cannot handle more than %d rooms!\n", MAX_ROOMS);
                    exit(ERR_ROOM_L);
                } else {
                    num_of_rooms = (int) strtol(argv[par], NULL, 10);
                    printf("You have set number of rooms on the server set to: %d\n", num_of_rooms);
                    edit_server = true;
                }
                break;
            case 'm':
                if((int) strtol(argv[++par], NULL, 10) <= MIN_PLAYERS) {
                    printf("You need at least %d player on the server to play!\n", MIN_PLAYERS);
                    exit(ERR_PLAY_G);
                } else if((int) strtol(argv[par], NULL, 10) >= MAX_PLAYERS) {
                    printf("Server cannot handle more than %d players!\n", MAX_PLAYERS);
                    exit(ERR_PLAY_L);
                } else {
                    max_players = (int) strtol(argv[par], NULL, 10);
                    printf("You have set max number of players on set to: %d\n", max_players);
                    edit_server = true;
                }
                break;
            default:;
        }
    }

    s = create_server();

    if(edit_server == true) {
        set_server(&s, max_players, num_of_rooms, port, address);
    }

    run_server(s);

    destroy_server(&s);

    return EXIT_SUCCESS;
}

