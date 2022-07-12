
#include "server_functions.h"

int handle_client_iniciative(char *buf, int client_socket, char *outcoming_buf) {

    /** Buffer pro prevod cisla na retezec a opacne. */
    char small_buf[SMALL_BUF_LENGTH] = {0};

    char temp_buf[MAX_BUF_LENGTH] = { 0 };

    /** Indexy pro cteni z bufferu. */
    int passed, willPass;

    int client_id;

    /** Tohle by se nemelo stat. */
    if (!buf) {
        printf("prazdny buffer\n");
        return EXIT_FAILURE;
    }

    if (client_socket < 4) {
        printf("maly socket\n");
        return EXIT_FAILURE;
    }

    memset(outcoming_buf, 0, MAX_BUF_LENGTH);

    passed = 0;
    willPass = strlen(LOGIN);

    /** Zprava pto prihlaseni na server. */
    if (strncmp(LOGIN, buf + passed, willPass) == 0) {
        if ((*s).num_of_clients < (*s).max_clients) {

            passed += willPass;
            willPass = FORMAT_INT_NUMBER_LENGTH;
            strncpy(small_buf, buf + passed, willPass);
            int nick_length = strtol(small_buf, NULL, 10);
            if (nick_length > MIN_NICK_LEN && nick_length <= MAX_NICK_LEN) {
                passed += willPass;
                strncpy(temp_buf, buf + passed, nick_length);

                /** Pridam klienta do spojoveho seznamu. */
                clientll_add(&(*s).clients, new_client((++(*s).highest_id), client_socket, temp_buf, LOBBY_ID), s->max_clients);

                /** Zvetsim pocet klientu na serveru. */
                ++(*s).num_of_clients;
                /** Zvetsim pocet klientu v lobby. */
                increase_clients(LOBBY_ID);

                strcat(temp_buf, format_int_number(PERM_CONFIRMED)); /** Povoleni pristupu. */
                strcat(temp_buf, format_int_number(clientll_get_by_socket((*s).clients, client_socket)->id));
                strcat(temp_buf, format_int_number((*s).num_of_rooms));

                /** Slozim posilanou zpravu. */
                fill_buf_with_message(outcoming_buf, LOGIN, temp_buf);

                return EXIT_SUCCESS;
            } else { /** Nevalidni nick, odpojim klienta. */
                strcat(temp_buf, format_int_number(PERM_DENIED)); /** Zakaz pristupu. */

                fill_buf_with_message(outcoming_buf, LOGIN, temp_buf);

                return EXIT_WARNING;
            }
        } else { /** Na serveru uz neni misto, odpojim klienta ale stejne mu poslu odpoved. */
            strcat(temp_buf, format_int_number(PERM_DENIED)); /** Zakaz pristupu. */

            fill_buf_with_message(outcoming_buf, LOGIN, temp_buf);

            return EXIT_WARNING;
        }
        /** Zprava pro pripojeni do herni mistnosti. */
    } else if (strncmp(JOIN, buf + passed, willPass) == 0) {
        /** Cislo mistnosti, kam klient vstoupi. */
        int room, remaining_time;

        client_id = clientll_get_by_socket((*s).clients, client_socket)->id;

        passed += willPass;
        willPass = FORMAT_INT_NUMBER_LENGTH;

        /** ID klienta. */
        strncpy(small_buf, buf + passed, willPass);
        if (client_id == strtol(small_buf, NULL, 10)) {
            memset(small_buf, 0, strlen(small_buf));

            passed += willPass;
            willPass = FORMAT_INT_NUMBER_LENGTH;
            /** Cislo mistnosti, do ktere chce klient vstoupit. */
            strncpy(small_buf, buf + passed, willPass);

            if (strtol(small_buf, NULL, 10) <= (*s).num_of_rooms && strtol(small_buf, NULL, 10) != 0) {
                decrease_clients(LOBBY_ID);
                room = strtol(small_buf, NULL, 10);
                if (increase_clients(room) == EXIT_SUCCESS) {
                    room_info(room);
                    /** Presunu klienta do herni misntosti. */
                    move_to_room(client_id, room);

                    /** Slozim posilanou zpravu. */
                    strcat(temp_buf, format_int_number(client_id));
                    strcat(temp_buf, format_int_number(PERM_CONFIRMED)); /** Povoleni pristupu. */
                    remaining_time = (int) (time(NULL) - (*s).rooms[room].start_time);
                    if(remaining_time < ROOM_COOLDOWN) {
                        strcat(temp_buf, format_int_number(ROOM_COOLDOWN - remaining_time));
                    } else {
                        strcat(temp_buf, format_int_number(0));
                    }
                    strcat(temp_buf, player_info(client_id));
                    strcat(temp_buf, others_info(client_id));

                    fill_buf_with_message(outcoming_buf, JOIN, temp_buf);

                    return EXIT_SUCCESS;
                } else { /** Mistnost uz je plna */
                    printf("mistnost je plna\n");
                    strcat(temp_buf, format_int_number(client_id));
                    strcat(temp_buf, format_int_number(ROOM_FULL)); /** Zakaz pristupu. */

                    fill_buf_with_message(outcoming_buf, JOIN, temp_buf);

                    return EXIT_WARNING;
                }
            } else { /** Takova mistnost neexistuje nebo je to lobby, kde klient uz je. */
                strcat(temp_buf, format_int_number(client_id));
                strcat(temp_buf, format_int_number(PERM_DENIED)); /** Zakaz pristupu. */

                fill_buf_with_message(outcoming_buf, JOIN, temp_buf);

                return EXIT_WARNING;
            }
        } else { /** Klient poslal spatne ID. */
            strcat(temp_buf, format_int_number(client_id));
            strcat(temp_buf, format_int_number(PERM_DENIED));

            fill_buf_with_message(outcoming_buf, JOIN, temp_buf);

            return EXIT_WARNING;
        }

        /** Zprava pro oznameni pohybu. */
    } else if (strncmp(MOVE, buf + passed, willPass) == 0) {
        double temp_x_pos, temp_y_pos, temp_radius;
        int room_id;

        passed += willPass;
        willPass = FORMAT_INT_NUMBER_LENGTH;

        client_id = clientll_get_by_socket((*s).clients, client_socket)->id;
        room_id = clientll_get_by_socket((*s).clients, client_socket)->room;

        /** ID klienta. */
        strncpy(small_buf, buf + passed, willPass);
        if (client_id == strtol(small_buf, NULL, 10)) {
            memset(small_buf, 0, strlen(small_buf));

            passed += willPass;
            willPass = FORMAT_FLOAT_NUMBER_LENGTH;
            strncpy(small_buf, buf + passed, willPass);
            temp_x_pos = strtod(small_buf, NULL);
            memset(small_buf, 0, strlen(small_buf));

            passed += willPass;
            willPass = FORMAT_FLOAT_NUMBER_LENGTH;
            strncpy(small_buf, buf + passed, willPass);
            temp_y_pos = strtod(small_buf, NULL);
            memset(small_buf, 0, strlen(small_buf));

            passed += willPass;
            willPass = FORMAT_FLOAT_NUMBER_LENGTH;
            strncpy(small_buf, buf + passed, willPass);
            temp_radius = strtod(small_buf, NULL);
            memset(small_buf, 0, strlen(small_buf));

            if (temp_x_pos >= 0 && temp_x_pos <= GAME_WIDTH && temp_y_pos >= 0 && temp_y_pos <= GAME_HEIGHT &&
                temp_radius >= PLAYER_SPAWN_RADIUS && temp_radius <= PLAYER_MAX_RADIUS) {
                /** Aktualizuje se stav klienta, por ostatni. */
                update_player(client_id, temp_radius, temp_x_pos, temp_y_pos);

                strcat(temp_buf, format_int_number(client_id));
                if((*s).rooms[room_id].room_state == ROOM_STATE_RUNNING && (*s).rooms[room_id].num_of_players == 1) {
                    (*s).rooms[room_id].room_state = ROOM_STATE_WON;
                    strcat(temp_buf, format_int_number(WINNER_CODE));
                    end_game_in_room(room_id);

                } else {
                    strcat(temp_buf, format_int_number(PERM_CONFIRMED)); /** Povoleni pristupu. */
                    strcat(temp_buf, others_info(client_id));
                }

                fill_buf_with_message(outcoming_buf, MOVE, temp_buf);

                return EXIT_SUCCESS;
            } else { /** Klient poslal spatne souradnice nebo polomer. */
                strcat(temp_buf, format_int_number(client_id));
                strcat(temp_buf, format_int_number(PERM_DENIED)); /** Zakaz pristupu. */

                fill_buf_with_message(outcoming_buf, MOVE, temp_buf);

                return EXIT_WARNING;
            }
        } else { /** Klient poslal spatne ID. */
            strcat(temp_buf, format_int_number(client_id));
            strcat(temp_buf, format_int_number(PERM_DENIED)); /** Zakaz pristupu. */

            fill_buf_with_message(outcoming_buf, MOVE, temp_buf);

            return EXIT_WARNING;
        }
        /** Klient poslal zpravu, ze zabil jineho hrace. */
    } else if (strncmp(KILL, buf + passed, HEAD_LENGTH) == 0) {
        int id_of_dead, room_id;

        client_id = clientll_get_by_socket((*s).clients, client_socket)->id;
        room_id = clientll_get_by_socket((*s).clients, client_socket)->room;

        if((*s).rooms[room_id].room_state == ROOM_STATE_RUNNING) {
            passed += willPass;
            willPass = FORMAT_INT_NUMBER_LENGTH;
            memset(small_buf, 0, strlen(small_buf));
            strncpy(small_buf, buf + passed, willPass);
            if (client_id == strtol(small_buf, NULL, 10)) {

                passed += willPass;
                willPass = FORMAT_INT_NUMBER_LENGTH;
                memset(small_buf, 0, strlen(small_buf));
                strncpy(small_buf, buf + passed, willPass);
                id_of_dead = strtol(small_buf, NULL, 10);

                /** Tady ho oznacim jako mrtvyho, potom ho odstranim. */
                if(clientll_client_with_id_exist((*s).clients, id_of_dead) == EXIT_SUCCESS) {
                    kill_player(id_of_dead);
                    decrease_clients(room_id);
                    increase_clients(LOBBY_ID);

                    strcat(temp_buf, format_int_number(client_id));
                    if((*s).rooms[room_id].num_of_players == 1) {
                        (*s).rooms[room_id].room_state = ROOM_STATE_WON;
                        strcat(temp_buf, format_int_number(WINNER_CODE));
                        move_to_lobby(client_id);
                        end_game_in_room(room_id);
                    } else {
                        strcat(temp_buf, format_int_number(PERM_CONFIRMED));
                    }

                    fill_buf_with_message(outcoming_buf, KILL, temp_buf);

                    return EXIT_SUCCESS;
                }
            }
        }

        strcat(temp_buf, format_int_number(client_id));
        strcat(temp_buf, format_int_number(PERM_DENIED));

        fill_buf_with_message(outcoming_buf, KILL, temp_buf);

        return EXIT_WARNING;
    } else if (strncmp(EAT_FOOD, buf + passed, HEAD_LENGTH) == 0) {
        int feed_id, room_id;

        client_id = clientll_get_by_socket((*s).clients, client_socket)->id;

        passed += willPass;
        willPass = FORMAT_INT_NUMBER_LENGTH;
        strncpy(small_buf, buf + passed, willPass);
        if (client_id == strtol(small_buf, NULL, 10)) {

            passed += willPass;

            willPass = FORMAT_INT_NUMBER_LENGTH;
            memset(small_buf, 0, strlen(small_buf));
            strncpy(small_buf, buf + passed, willPass);
            feed_id = strtol(small_buf, NULL, 10);

            room_id = clientll_get_by_socket((*s).clients, client_socket)->room;

            if (respawn_feed_in_room(room_id, feed_id) == 0) {
                strcat(temp_buf, format_int_number(client_id));
                if((*s).rooms[room_id].room_state == ROOM_STATE_RUNNING && (*s).rooms[room_id].num_of_players == 1) {
                    (*s).rooms[room_id].room_state = ROOM_STATE_WON;
                    strcat(temp_buf, format_int_number(WINNER_CODE));
                    end_game_in_room(room_id);
                } else {
                    strcat(temp_buf, format_int_number(PERM_CONFIRMED));
                }

                fill_buf_with_message(outcoming_buf, EAT_FOOD, temp_buf);

                return EXIT_SUCCESS;
            }
        }

        strcat(temp_buf, format_int_number(client_id));
        strcat(temp_buf, format_int_number(PERM_DENIED));

        fill_buf_with_message(outcoming_buf, EAT_FOOD, temp_buf);

        return EXIT_WARNING;
    } else if(strncmp(DISCONNECT, buf + passed, HEAD_LENGTH) == 0) {

        client_id = clientll_get_by_socket((*s).clients, client_socket)->id;

        passed += willPass;
        willPass = FORMAT_INT_NUMBER_LENGTH;
        strncpy(small_buf, buf + passed, willPass);
        if (client_id == strtol(small_buf, NULL, 10)) {
            memset(small_buf, 0, strlen(small_buf));

            strcat(temp_buf, format_int_number(client_id));
            strcat(temp_buf, format_int_number(PERM_CONFIRMED));

            fill_buf_with_message(outcoming_buf, DISCONNECT, temp_buf);

            return EXIT_DISCONNECT;
        }

        strcat(temp_buf, format_int_number(client_id));
        strcat(temp_buf, format_int_number(PERM_DENIED));

        fill_buf_with_message(outcoming_buf, DISCONNECT, temp_buf);

        return EXIT_WARNING;
    } else if(strncmp(RECONNECT, buf + passed, HEAD_LENGTH) == 0) {
        int room, remaining_time;

        passed += willPass;
        willPass = FORMAT_INT_NUMBER_LENGTH;
        strncpy(small_buf, buf + passed, willPass);
        client_id = strtol(small_buf, NULL, 10);
        if (clientll_client_with_id_exist((*s).clients, client_id) == EXIT_SUCCESS) {
            memset(small_buf, 0, strlen(small_buf));

            /** Zmenim socket. */
            reconnect_client(client_id, client_socket);

            if(clientll_get_by_id((*s).clients, client_id)->room == LOBBY_ID) {
                strcat(temp_buf, format_int_number(PERM_CONFIRMED)); /** Povoleni pristupu. */
                strcat(temp_buf, format_int_number(clientll_get_by_socket((*s).clients, client_socket)->id));
                strcat(temp_buf, format_int_number((*s).num_of_rooms));
            } else {
                strcat(temp_buf, format_int_number(client_id));
                strcat(temp_buf, format_int_number(PERM_CONFIRMED));
                room = clientll_get_by_id((*s).clients, client_id)->room;
                remaining_time = (int) (time(NULL) - (*s).rooms[room].start_time);
                if(remaining_time < ROOM_COOLDOWN) {
                    strcat(temp_buf, format_int_number(ROOM_COOLDOWN - remaining_time));
                } else {
                    strcat(temp_buf, format_int_number(0));
                }
                strcat(temp_buf, player_info(client_id));
                strcat(temp_buf, others_info(client_id));
            }

            fill_buf_with_message(outcoming_buf, RECONNECT, temp_buf);

            return EXIT_SUCCESS;
        }

        strcat(temp_buf, format_int_number(client_id));
        strcat(temp_buf, format_int_number(PERM_DENIED));

        fill_buf_with_message(outcoming_buf, RECONNECT, temp_buf);

        return EXIT_WARNING;
    } else if(strncmp(PING, buf + passed, HEAD_LENGTH) == 0) {

        client_id = clientll_get_by_socket((*s).clients, client_socket)->id;

        passed += willPass;
        willPass = FORMAT_INT_NUMBER_LENGTH;
        strncpy(small_buf, buf + passed, willPass);
        if (client_id == strtol(small_buf, NULL, 10)) {

            memset(small_buf, 0, strlen(small_buf));

            strcat(temp_buf, format_int_number(client_id));

            fill_buf_with_message(outcoming_buf, PING, temp_buf);

            return EXIT_PING;
        } else  {

            strcat(temp_buf, format_int_number(client_id));
            strcat(temp_buf, format_int_number(PERM_DENIED));

            fill_buf_with_message(outcoming_buf, PING, temp_buf);

            return EXIT_WARNING;
        }
    } else if(is_server_iniciative(buf) == true) {
        return handle_server_iniciative(buf, 0, client_socket, outcoming_buf);
    } else {
        return EXIT_GO_AWAY;
    }
}

int handle_server_iniciative(char *buf, int act_sock, int curr_sock, char *outcoming_buf)
{
    /** Buffer pro prevod cisla na retezec. */
    char small_buf[16] = { 0 };

    int passed, willPass;
    int client_id, client_socket;

    if(!buf || act_sock == curr_sock) {
        return EXIT_FAILURE;
    }

    if(act_sock == 0) {
        client_socket = curr_sock;
    } else {
        client_socket = act_sock;
    }

    client_id = clientll_get_by_socket((*s).clients, client_socket)->id;

    passed = 0;
    willPass = strlen(LOBBY);

    if(strncmp(LOBBY, buf + passed, willPass) == 0) {
        passed += willPass;
        willPass = FORMAT_INT_NUMBER_LENGTH;

        strncpy(small_buf, buf + passed, willPass);
        if(client_id == strtol(small_buf, NULL, 10)) {
            memset(small_buf, 0, strlen(small_buf));
            return EXIT_UPDATE_OK;
        }
    } else if(strncmp(ROOM, buf + passed, willPass) == 0) {
        passed += willPass;
        willPass = FORMAT_INT_NUMBER_LENGTH;

        strncpy(small_buf, buf + passed, willPass);
        if(client_id == strtol(small_buf, NULL, 10)) {
            memset(small_buf, 0, strlen(small_buf));
            return EXIT_UPDATE_OK;
        }
    } else if(strncmp(BACKTOLOBBY, buf + passed, willPass) == 0) {
        passed += willPass;
        willPass = FORMAT_INT_NUMBER_LENGTH;

        strncpy(small_buf, buf + passed, willPass);
        if(client_id == strtol(small_buf, NULL, 10)) {
            memset(small_buf, 0, strlen(small_buf));
            return EXIT_UPDATE_OK;
        }
    } else if(is_client_iniciative(buf)) {
        printf("je to request\n");
        return handle_client_iniciative(buf, client_socket, outcoming_buf);
    }
    return EXIT_WARNING;
}

int handle_message(int client_socket, char *outcoming_buf)
{
    int client_room_id, client_id;

    char temp_buf[MAX_BUF_LENGTH] = { 0 };

    if(client_socket < 4) {
        return EXIT_FAILURE;
    }

    memset(outcoming_buf, 0, MAX_BUF_LENGTH);

    client_id = clientll_get_by_socket((*s).clients, client_socket)->id;
    client_room_id = clientll_get_by_socket((*s).clients, client_socket)->room;

    if (client_room_id == 0) {
        strcat(temp_buf, format_int_number(client_id));
        fill_buf_with_message(outcoming_buf, LOBBY, temp_buf);
    } else {
        if ((*s).rooms[client_room_id].room_state == ROOM_STATE_RUNNING || (*s).rooms[client_room_id].room_state == ROOM_STATE_WAITING) {
            strcat(temp_buf, format_int_number(client_id));
            strcat(temp_buf, others_info(client_id));
            fill_buf_with_message(outcoming_buf, ROOM, temp_buf);

        } else if ((*s).rooms[client_room_id].room_state == ROOM_STATE_WON) {
            /** Zprava pro vyherce. */
            decrease_clients(client_room_id);
            move_to_room(client_id, LOBBY_ID);
            increase_clients(LOBBY_ID);
            strcat(temp_buf, format_int_number(client_id));
            if(clientll_get_by_id((*s).clients, client_id)->alive == false) {
                strcat(temp_buf, format_int_number(LOSE_CODE));
            } else {
                strcat(temp_buf, format_int_number(WINNER_CODE));
                end_game_in_room(client_room_id);
            }
            fill_buf_with_message(outcoming_buf, BACKTOLOBBY, temp_buf);
        }
    }
    return EXIT_SUCCESS;
}

void close_socket(int socket)
{
    (*s).num_of_clients--;
    close(socket);
    FD_CLR(socket, &(*s).master);
}

void fill_buf_with_message(char *buffer, char *state, char *message)
{
    strcpy(buffer, MAGIC);
    strcat(buffer, format_long_int_number(strlen(state) + strlen(message)));
    strcat(buffer, state);
    strcat(buffer, message);
    strcat(buffer, "\n");
}

char * format_int_number(int number)
{
    static char buf[3] = { 0 };
    sprintf(buf, "%02d", number);
    return buf;
}

char * format_long_int_number(int number)
{
    static char buf[5] = { 0 };
    sprintf(buf, "%04d", number);
    return buf;
}

char * format_float_number(double number)
{
    static char buf[8] = { 0 };
    sprintf(buf, "%07.02f", number);
    return buf;
}

char * player_info(const int id)
{
    static char buf[MAX_BUF_LENGTH] = { 0 };

    client *c = clientll_get_by_id((*s).clients, id);

    memset(buf, 0, MAX_BUF_LENGTH);
    strcat(buf, format_float_number((*c).x_pos));
    strcat(buf, format_float_number((*c).y_pos));
    strcat(buf, format_float_number((*c).radius));
    strcat(buf, format_int_number((*c).color));
    return buf;
}

char * others_info(const int id)
{
    static char buf[MAX_BUF_LENGTH] = { 0 };

    int room_of_player = clientll_get_by_id((*s).clients, id)->room;
    client_list_node *temp = (*s).clients;
    client *c = NULL;

    memset(buf, 0, MAX_BUF_LENGTH);

    while(temp != NULL) {
        if(room_of_player == (*temp).data->room && (*temp).data->id != id) {
            c = (*temp).data;
            strcat(buf, "pp");
            strcat(buf, format_int_number((*c).id));
            strcat(buf, format_int_number(strlen((*c).nickname)));
            strcat(buf, (*c).nickname);
            strcat(buf, format_float_number((*c).x_pos));
            strcat(buf, format_float_number((*c).y_pos));
            strcat(buf, format_float_number((*c).radius));
            strcat(buf, format_int_number((*c).color));
            strcat(buf, format_int_number((*c).connected));
        }
        temp = (*temp).next;
    }

    if((*s).rooms[room_of_player].feeds != NULL) {
        for(int i = 0; i < (*s).rooms[room_of_player].num_of_feeds; ++i) {
            strcat(buf, "ff");
            strcat(buf, format_int_number((*s).rooms[room_of_player].feeds[i].id));
            strcat(buf, format_float_number((*s).rooms[room_of_player].feeds[i].x_pos));
            strcat(buf, format_float_number((*s).rooms[room_of_player].feeds[i].y_pos));
            strcat(buf, format_float_number((*s).rooms[room_of_player].feeds[i].radius));
            strcat(buf, format_int_number((*s).rooms[room_of_player].feeds[i].color));
        }
    } else {
        printf("zadna potrava tu neni\n");
    }
    return buf;
}

bool is_client_iniciative(char *buffer) {
    if(!buffer) {
        return false;
    }

    int passed = 0;
    int willPass = strlen(BACKTOLOBBY);

    if(strncmp(LOGIN, buffer + passed, willPass) == 0 || strncmp(JOIN, buffer + passed, willPass) == 0 || strncmp(KILL, buffer + passed, willPass) == 0 || strncmp(MOVE, buffer + passed, willPass) == 0 || strncmp(EAT_FOOD, buffer + passed, willPass) == 0 || strncmp(DISCONNECT, buffer + passed, willPass) == 0 || strncmp(RECONNECT, buffer + passed, willPass) == 0 || strncmp(PING, buffer + passed, willPass) == 0) {
        return true;
    } else {
        return false;
    }
}

bool is_server_iniciative(char *buffer) {
    if(!buffer) {
        return false;
    }

    int passed = 0;
    int willPass = strlen(BACKTOLOBBY);

    if(strncmp(LOBBY, buffer + passed, willPass) == 0 || strncmp(ROOM, buffer + passed, willPass) == 0 || strncmp(BACKTOLOBBY, buffer + passed, willPass) == 0) {
        return true;
    } else {
        return false;
    }
}

void disconnect_client(int client_socket, char *reason) {
    if(!reason || client_socket < 4) {
        return;
    }

    decrease_clients(clientll_get_by_socket((*s).clients, client_socket)->room);
    if(clientll_client_on_socket_exist((*s).clients, client_socket) == EXIT_SUCCESS) {
        clientll_remove_client(&(*s).clients, clientll_get_by_socket((*s).clients, client_socket)->id);
    }
    close_socket(client_socket);
    printf("%s\n", reason);
}

void closed_stream(int client_sock, int return_value) {
    return_value = temp_disconnection(clientll_get_by_socket((*s).clients, client_sock));
    if (return_value == EXIT_DISCONNECT) {
        disconnect_client(client_sock, "Odpojil jsem klienta, znenadani zavrel socket (recv vratil -1) - po timeoutu.");
    } else if (return_value == EXIT_FAILURE) {
        die(ERR_PARAM_PRINT, ERR_PARAM);
    }
}
