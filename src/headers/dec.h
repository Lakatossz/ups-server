
#ifndef SERVER_DEC_H
#define SERVER_DEC_H

/**
 * Konstanty pro nastaveni serveru, jak po herni
 * strance (roomky), i sitove strance (port).
 */
#define MAX_PORT 65635
#define MIN_PORT 1024

#define MAX_HOSTNAME_LENGTH 127
#define MIN_HOSTNAME_LENGTH 4

#define MAX_ROOMS 4
#define MIN_ROOMS 1

#define MAX_PLAYERS 16
#define MIN_PLAYERS 1

#define DEFAULT_MAX_PLAYERS MAX_PLAYERS
#define DEFAULT_MAX_ROOMS MAX_ROOMS
#define MAX_IN_ROOM 4
#define DEFAULT_PORT 10001

/**
 * Cas, po kterem server klienta povazuje za
 * odpojeneho (pri dlouhodobe nekomunikaci).
 */
#define TIMEOUT 30000

/**
 * Cas, po kterem server klienta povazuje za docasne
 * odpojeneho (pri dlouhodobe nekomunikaci).
 */
#define TEMP_TIMEOUT 10000

/**
 * 16 milisec
 */
#define LAST_MESSAGE_TIME 16

/**
 * Konstanty pro samotnou hru.
 */
#define GAME_WIDTH 2000
#define GAME_HEIGHT 2000
#define NUM_OF_FEEDS 50
#define PLAYER_MAX_RADIUS 99999
#define PLAYER_SPAWN_RADIUS 10
#define FEED_SPAWN_RADIUS 10

/**
 * Konstanty pro navratove hodnoty erroru.
 */
// G - greater, L - lesser
#define ERR_HOST_G 0
#define ERR_HOST_L 1
#define ERR_PORT_G 2
#define ERR_PORT_L 3
#define ERR_ROOM_G 4
#define ERR_ROOM_L 5
#define ERR_PLAY_G 6
#define ERR_PLAY_L 7
#define ERR_SOCK 8
#define ERR_BIND 9
#define ERR_CONN 10
#define ERR_PARAM 11

#define ERR_PARAM_PRINT "Neplatne paramtery, koncim."

/**
 * Konstanty pro komunikaci.
 */
#define MAGIC "GAME"
#define LOGIN "logi"
#define JOIN "join"
#define KILL "kill"
#define MOVE "move"
#define EAT_FOOD "eatf"
#define DISCONNECT "disc"
#define RECONNECT "recn"
#define LOBBY "lobb"
#define ROOM "room"
#define BACKTOLOBBY "bckl"
#define PING "ping"

#define FIRST_READ 8
#define MIN_MESS_LENGTH 8

/**
 * Kodova zprava pro vyherce hry.
 */
#define WINNER_CODE 99

/**
 * Zprava pro hrace, ktery umrel.
 */
#define LOSE_CODE 66

/**
 * Konstanty pro zpracovani zpravy.
 */
#define MIN_NICK_LEN 1
#define MAX_NICK_LEN 32
#define MAX_BAD_ANSWERS 40
#define MAX_BUF_LENGTH 4096
#define SMALL_BUF_LENGTH 16
#define HEAD_LENGTH 4
#define FORMAT_INT_NUMBER_LENGTH 2
#define FORMAT_FLOAT_NUMBER_LENGTH 7

/**
 * Kodova zprava pro potvrzeni nebo zamitnuti zpravy.
 */
#define PERM_CONFIRMED 00
#define PERM_DENIED 11

/**
 * Kodova zprava pro oznameni, ze mistnost je plna.
 */
#define ROOM_FULL 22

/**
 * Cislo mistnosti, urcene pro lobby.
 */
#define LOBBY_ID 0

/**
 * Stavy herni mistnosti.
 */
#define ROOM_STATE_EMPTY 0
#define ROOM_STATE_WAITING 1
#define ROOM_STATE_RUNNING 2
#define ROOM_STATE_WON 3

/**
 * Cooldown, pro cekani na napojeni ostatnich hracu.
 */
#define ROOM_COOLDOWN 30

/**
 * Navratova hodnota
 */
#define EXIT_WARNING 2
#define EXIT_DISCONNECT 3
#define EXIT_PING 4
#define EXIT_GO_AWAY 5
#define EXIT_UPDATE_OK 6

/**
 * Definovane barvy kulicek.
 */
#define RED 0
#define ORANGE 1
#define YELLOW 2
#define GREEN 3
#define BLUE 4
#define PURPLE 5

#define MAX_COLOR PURPLE

/**
 * Definice datoveho typu bool.
 */
typedef enum {
    true = 1, false = 0
} bool;

/**
 * Funkce pro ukonceni s navratovou hodnotou a vypis dane chyby.
 */
void die(const char* chr, int err);

#endif
