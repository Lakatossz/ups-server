
#include "linkedlist.h"

void clientll_add(client_list_node **list, client *c, int limit)
{
    client_list_node  *temp = NULL;

    if(!list || clientll_count_clients(*list) >= limit) {
        return;
    }

    temp = (client_list_node *)malloc(sizeof(client_list_node));
    if(!temp) {
        return;
    }

    (*temp).data = c;
    (*temp).next = *list;

    *list = temp;
}

void clientll_remove_client(client_list_node **list, int id)
{
    client_list_node *temp = NULL, *prev;

    if(!list || !*list || id < 1) {
        return;
    }

    temp = *list;

    if((*temp).data->id == id) {
        printf("Klient %s byl odpojen ze serveru.\n", (**list).data->nickname);
        destroy_client(&((*temp).data));
        *list = (*temp).next;
        free(temp);
        temp = NULL;
    } else {
        while(temp != NULL && (*temp).data->id != id) {
            prev = temp;
            temp = (*temp).next;
        }

        printf("Klient %s byl odpojen ze serveru.\n", (**list).data->nickname);

        if(temp == NULL) {
            return;
        }

        (*prev).next = (*temp).next;
        destroy_client(&((*temp).data));
        free(temp);
        temp = NULL;
    }
}

int clientll_count_clients(client_list_node *list)
{
    int count = 0;
    client_list_node *temp = NULL;

    if(!list) {
        return 0;
    } else {
        temp = list;

        while(temp) {
            ++count;
            temp = (*temp).next;
        }
        return count;
    }
}

client * clientll_get_by_socket(client_list_node *list, int client_socket)
{
    client_list_node *temp = NULL;

    if(!list) {
        return NULL;
    }

    temp = list;

    while((*temp).data->socket_number != client_socket) {
        if((*temp).next == NULL) {
            return NULL;
        } else {
            temp = (*temp).next;
        }
    }

    return (*temp).data;
}

client * clientll_get_by_id(client_list_node *list, int id)
{
    client_list_node *temp = NULL;

    if(!list || id < 0) {
        return NULL;
    }

    temp = list;

    while((*temp).data->id != id) {
        if((*temp).next == NULL) {
            return NULL;
        } else {
            temp = (*temp).next;
        }
    }

    return (*temp).data;
}

int clientll_client_on_socket_exist(client_list_node *list, int client_socket)
{
    client_list_node *temp = NULL;

    if(!list || client_socket < 4) {
        return EXIT_FAILURE;
    }

    temp = list;

    while(temp) {
        if((*temp).data->socket_number == client_socket) {
            return EXIT_SUCCESS;
        } else {
            temp = (*temp).next;
        }
    }
    return EXIT_FAILURE;
}

int clientll_client_with_id_exist(client_list_node *list, int id)
{
    client_list_node *temp = NULL;

    if(!list || id < 1) {
        return EXIT_FAILURE;
    }

    temp = list;

    while(temp) {
        if((*temp).data->id == id) {
            return EXIT_SUCCESS;
        } else {
            temp = (*temp).next;
        }
    }
    return EXIT_FAILURE;
}

void clientll_free(client_list_node **list)
{
    client_list_node *next;

    if(!list || !*list) {
        return;
    }

    while(*list) {
        next = (**list).next;
        free(*list);
        *list = next;
    }

    *list = NULL;
}
