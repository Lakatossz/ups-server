
#ifndef SERVER_SERVER_FUNCTIONS_H
#define SERVER_SERVER_FUNCTIONS_H

#include "../../headers/glob.h"
#include "../../headers/dec.h"
#include "../server_def.h"
#include "../../client/client.h"
#include "../../client/client_functions/client_functions.h"

/**
 * Strara se o zpracovani prichozi zpravy (zadost).
 * @param buf Prichozi zprava.
 * @param client_sock Cislo socketu, ze ktereho zprava prisla.
 * @param outcoming_buf Buffer, do ktereho se naplni dale posilana zprava.
 * @return Vrati 0 - pokud vse ok, 2 - pokud klient poslal nevalidni zpravu,
 * 3 - pokud se klient odpojuje, 4 - pokud slo o ping zpravu,
 * 1 - pokud doslo k chybe a musi se zastavit server.
 */
int handle_client_iniciative(char *buf, int client_socket, char *outcoming_buf);

/**
 * Starat se o zpracovani zpravy, ktera prisla jako aktualizace herniho stavu.
 * @param buf Prichozi zprava.
 * @param act_sock Cislo socketu, ze ktereho zprava prisla.
 * @param curr_sock Cislo socketu, ne ktery se bude pripadne posilat dodatecna odpoved,
 * pokud klient poslal zadost.
 * @param outcoming_buf Buffer, do ktereho se naplni pripadna posilana zprava.
 * @return Vrati 6 - pokud vse ok, 2 - pokud klient poslal nevalidni zpravu,
 * 1 - pokud doslo k chybe a musi se zastavit server.
 */
int handle_server_iniciative(char *buf, int act_sock, int curr_sock, char *outcoming_buf);

/**
 * Pripravy aktualizacni zpravu, ktera bude poslana na dany socket.
 * @param client_socket Socket, na ktery bude zprava poslana.
 * @param outcoming_buf Buffer, do ktereho se naplni posilana zprava.
 * @return 0 - pokud vse probehlo v poradku, 1 jinak.
 */
int handle_message(int client_socket, char *outcoming_buf);

/**
 * Radne zavre socket a dekrementuje pocet klientu na serveru.
 * @param socket Cislo zaviraneho socketu.
 */
void close_socket(int socket);

/**
 * Prida zprave hlavicku, delku a znacku.
 * @param buffer Buffer, do ktereho se naplni posilana zprava.
 * @param state Znacka, ktera bude pridana do zpravy.
 * @param message Pridavana zprava.
 */
void fill_buf_with_message(char *buffer, char *state, char *message);

/**
 * Zformatuje cele cislo na dve cifry (napr. 01).
 */
char * format_int_number(int number);

/**
 * Zformatuje cele cislo na ctyri cifry (napr. 0001).
 */
char * format_long_int_number(int number);

/**
 * Zformatuje realne cislo na 6 cifer (napr. 0001.10).
 */
char * format_float_number(double number);

/**
 * Vrati buffer, ve kterem jsou vsechny informace,
 * ktere budou potreba poslat o danem hraci s id.
 */
char * player_info(const int id);

/**
 * Vrati buffer, ve kterem jsou vsechny informace,
 * ktere budou potreba poslat o ostatnich hracich, nez je hrac s id.
 */
char * others_info(const int id);

/**
 * Zjisti, jetsli dana zprava obsahuje znacku zadosti ze strany klienta.
 * @return true - pokud ano, jinak false.
 */
bool is_client_iniciative(char *buffer);

/**
 * Zjisti, jetsli dana zprava obsahuje znacku odpovedi na aktualizaci.
 * @return true - pokud ano, jinak false.
 */
bool is_server_iniciative(char *buffer);

/**
 * Odebere klienta z mistnosti a radne ho odpoji.
 * @param client_socket Socket, na kterem klient posloucha.
 * @param reason Zprava, ktera bude pri odpojeni vypsana.
 */
void disconnect_client(int client_socket, char *reason);

/**
 * Pokud znenadani klient zavre socket, priramu stav disconnected,
 * po nejakem case ho povazuje za odpojeneho.
 * @param client_sock Socket, na kterem klient posloucha.
 */
void closed_stream(int client_sock, int return_value);

#endif
