
#ifndef SERVER_LINKEDLIST_H
#define SERVER_LINKEDLIST_H

#include "../client.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * Definice struktury node pro spojovy seznam.
 */
typedef struct the_client_list_node {
    client *data;                       /** Data daneho nodu. */
    struct the_client_list_node *next;  /** Dalsi prvek v seznamu. */
} client_list_node;

/**
 * Prida hrace do spojoveho seznamu.
 * @param list Ukazatel na ukazatel na strukturu spojovy seznam.
 * @param c Ukazatel na klienta, prideno do spojoveho seznamu.
 * @param limit Maximalni pocet klientu.
 */
void clientll_add(client_list_node **list, client *c, int limit);

/**
 * Odebere hrace ze spojoveho seznamu.
 * @param list Ukazatel na ukazatel na strukturu spojovy seznam.
 * @param id ID klienta.
 */
void clientll_remove_client(client_list_node **list, int id);

/**
 * Spocita pocet hracu ve spojovem seznamu.
 * @param list Ukazatel na ukazatel na strukturu spojovy seznam.
 * @return Pocet hracu ve spojovem seznamu, 0 jinak.
 */
int clientll_count_clients(client_list_node *list);

/**
 * Vrati klienta na danem socketu.
 * @param list Ukazatel na ukazatel na strukturu spojovy seznam.
 * @param client_socket Socket, na kterem klient komunikuje.
 * @return Ukazatel na strukturu klienta, jinak NULL.
 */
client * clientll_get_by_socket(client_list_node *list, int client_socket);

/**
 * Vrati klienta s danym id.
 * @param list Ukazatel na ukazatel na strukturu spojovy seznam.
 * @param id ID klienta.
 * @return Ukazatel na strukturu klienta, jinak NULL.
 */
client * clientll_get_by_id(client_list_node *list, int id);

/**
 * Zjisti, jetsli nejaky klient na danem socketu posloucha.
 * @param list Ukazatel na ukazatel na strukturu spojovy seznam.
 * @param client_socket Socket, na kterem klient komunikuj
 * @return 0, pokud klient existuje, 1 jinak.
 */
int clientll_client_on_socket_exist(client_list_node *list, int client_socket);

/**
 * Zjisti, jestli se na server nachazi klient s danym id.
 * @param list Ukazatel na ukazatel na strukturu spojovy seznam.
 * @param id ID klienta.
 * @return 0, pokud klient existuje, 1 jinak.
 */
int clientll_client_with_id_exist(client_list_node *list, int id);

/**
 * Uvolni pamet, kterou pouzival spojovy seznam.
 * @param list Ukazatel na ukazatel na spojovy seznam.
 */
void clientll_free(client_list_node **list);

#endif
