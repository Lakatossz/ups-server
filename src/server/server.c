
#include "server.h"

char outcoming_buf[MAX_BUF_LENGTH] = { 0 };

void run_server()
{
    int res;
    struct sockaddr_in incoming;
    socklen_t inlen;

    fd_set read_fds, write_fds;

    /** Nejvyssi socket. */
    int fdmax;
    /** Posledni pridany socket. */
    int newfd;

    /** Pro prochazeni cyklu. */
    int curr_sock, act_sock;

    int return_value, handler_return;
    int second_read_length;

    struct timeval timeout;

    char first_read_buf[SMALL_BUF_LENGTH] = { 0 };

    /** Buffer pro vstup ze socketu. */
    char incoming_buf[MAX_BUF_LENGTH] = { 0 };

    FD_ZERO(&(*s).master);
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);

    printf("-------------SERVER IS RUNNING----------\n");
    printf("IP address: %s\n", inet_ntoa((*s).addr.sin_addr));
    printf("Listening on port: %d\n", htons((*s).addr.sin_port));
    printf("Max players: %d\n", (*s).max_clients);
    printf("Num of rooms: %d\n", (*s).num_of_rooms);

    /** Nabinduju oscket, ne kterem bude server poslouchat. */
    res = bind((*s).socket, (struct sockaddr *) &((*s).addr), sizeof(struct sockaddr_in));
    if(res != 0) {
        perror("error:");
        die("Nelze nabindovat adresu.", ERR_BIND);
        return;
    }

    timeout.tv_sec = 0;
    timeout.tv_usec = 10000;
    res = setsockopt((*s).socket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));
    if(res == -1) {
        printf("set recv timeout ERR\n");
        return;
    }

    printf("Bind OK\n");

    /** Server zacina poslouchat. */
    res = listen((*s).socket, (*s).max_clients);
    if(res != 0) {
        die("Nemuze vytvorit frontu spojeni.", ERR_CONN);
        return;
    }

    printf("Listen OK\n");

    /** Vytvorim pocet roomek podle vstupnich parametru. */
    (*s).rooms = create_rooms((*s).num_of_rooms + 1, (*s).max_clients);
    if(!(*s).rooms) {
        return;
    }

    FD_SET((*s).socket, &(*s).master);

    /** Zatim nejvetsi cislo socketu je to, kde server posloucha. */
    fdmax = (*s).socket;

    printf("-------------SERVER IS READY------------\n");

    long long start_time = time_in_milli();
    /** Nekonecna smycka, dokud server bezi. */
    while((*s).running == true) {

        timeout.tv_sec = 0;
        timeout.tv_usec = 1000;

        read_fds = (*s).master;
        write_fds = (*s).master;

        return_value = select(FD_SETSIZE, &read_fds, &write_fds, NULL, &timeout);

        /** Posloucha, pokud se pripoji nejaky novy klient. */
        if(return_value == -1) {
            printf("Server-select() error!\n");
        } else if(return_value) {
            usleep(1);
            /** Kontrola stavu mistnosi, ve kterych probiha hra. */
            for(int i = 1; i <= (*s).num_of_rooms; ++i) {
                if((*s).rooms[i].room_state == ROOM_STATE_WAITING) {
                    cooldown_passed(i);
                }
            }

            /** Tedka projde vsechny sockety a bude obstaravat vsechny sockety. */
            for(curr_sock = (*s).socket; curr_sock <= fdmax; ++curr_sock) {
                /** Testuje, zda-li je fd soucasti setu. */
                if(FD_ISSET(curr_sock, &read_fds)) {
                    /** Socket serveru, tj. nekdo se pripojil. */
                    if(curr_sock == (*s).socket) {
                        inlen = sizeof(incoming);
                        if((newfd = accept((*s).socket, (struct sockaddr *)&incoming, &inlen)) == -1) {
                            printf("Server-accept() error\n");
                        } else {
                            /** Vse se podarilo, na serveru je novy klient. */
                            printf("Server-accept() is OK... \n");
                            FD_SET(newfd, &(*s).master);
                            if(newfd > fdmax) {
                                fdmax = newfd;
                            }
                            printf("Nove spojeni z ip adresy %s na socketu %d\n", inet_ntoa(incoming.sin_addr), newfd);
                        }
                    } else {
                        /** Cast, kde jsou obsluhovany ostatni sockety. */
                        memset(first_read_buf, 0, SMALL_BUF_LENGTH);
                        /** Posloucham na socketu. */
                        return_value = recv(curr_sock, first_read_buf, FIRST_READ, 0);
                        if(return_value < 1) {
                            /** Socket je zavreny. */
                            closed_stream(curr_sock, return_value);
                            continue;
                        } else {
                            if (strncmp(MAGIC, first_read_buf, HEAD_LENGTH) != 0) {
                                close_socket(curr_sock);
                                printf("Odpojil jsem klienta, spatna hlavicka.\n");
                                continue;
                            }
                            second_read_length = strtol(first_read_buf + HEAD_LENGTH, NULL, 10);
                            if(second_read_length > 0) {
                                memset(incoming_buf, 0, MAX_BUF_LENGTH);
                                return_value = recv(curr_sock, incoming_buf, second_read_length, 0);
                                if(return_value < 1) {
                                    /** Socket je zavreny. */
                                    closed_stream(curr_sock, return_value);
                                    continue;
                                } else {
                                    if(clientll_client_on_socket_exist((*s).clients, curr_sock) == EXIT_SUCCESS) {
                                        clientll_get_by_socket((*s).clients, curr_sock)->last_time = time_in_milli();
                                    }
                                    printf("(%lld) Prichozi zprava ze socketu %d: %s%s\n", time_in_milli() - start_time, curr_sock, first_read_buf, incoming_buf);
                                    handler_return = handle_client_iniciative(incoming_buf, curr_sock, outcoming_buf);
                                    if (handler_return == EXIT_FAILURE) {
                                        die(ERR_PARAM_PRINT, ERR_PARAM);
                                        return;
                                    } else if(handler_return == EXIT_UPDATE_OK) {
                                        continue;
                                    } else {
                                        if (strlen(outcoming_buf) > MIN_MESS_LENGTH) {
                                            return_value = send(curr_sock, outcoming_buf, strlen(outcoming_buf), 0);
                                            printf("(%lld) Odeslana zprava na socket %d (odpoved): %s", time_in_milli() - start_time, curr_sock, outcoming_buf);
                                            if (return_value == -1) {
                                                closed_stream(curr_sock, return_value);
                                                continue;
                                            } else {
                                                if (handler_return == EXIT_WARNING) {
                                                    if(clientll_client_on_socket_exist((*s).clients, curr_sock) == EXIT_SUCCESS) {
                                                        if (increase_bad_counter_of_player(curr_sock) == EXIT_FAILURE) {
                                                            disconnect_client(curr_sock, "Odpojil jsem klienta, protoze posilal, co nemel.");
                                                        } else {
                                                            printf("Klient odpovedel spatne, ale jeste mu dam sanci.\n");
                                                        }
                                                    } else {
                                                        close_socket(curr_sock);
                                                    }
                                                } else if (handler_return == EXIT_DISCONNECT) {
                                                    disconnect_client(curr_sock, "Odpojil jsem klienta na jeho zadost.");
                                                    continue;
                                                } else if (handler_return == EXIT_PING) {
                                                    continue;
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            if(clientll_get_by_socket((*s).clients, curr_sock) != NULL) {
                                for(act_sock = (*s).socket + 1; act_sock <= fdmax; ++act_sock) {
                                    if(FD_ISSET(act_sock, &write_fds)) {
                                        if(act_sock != curr_sock && clientll_client_on_socket_exist((*s).clients, act_sock) == EXIT_SUCCESS) {
                                            if(clientll_get_by_socket((*s).clients, act_sock)->room == clientll_get_by_socket((*s).clients, curr_sock)->room) {
                                                handler_return = handle_message(act_sock, outcoming_buf);
                                                if(handler_return == EXIT_FAILURE) {
                                                    printf("Spatne parametry pri zpracovani aktualizacni zpravy.\n");
                                                } else if(handler_return == EXIT_SUCCESS) {
                                                    if(strlen(outcoming_buf) > MIN_MESS_LENGTH) {
                                                        printf("(%lld) Odeslana zprava na socket %d (%s): %s", time_in_milli() - start_time, act_sock, clientll_get_by_socket((*s).clients, act_sock)->nickname, outcoming_buf);
                                                        return_value = send(act_sock, outcoming_buf, strlen(outcoming_buf), 0);
                                                        if(return_value == -1) {
                                                            closed_stream(act_sock, return_value);
                                                        } else {
                                                            memset(first_read_buf, 0, SMALL_BUF_LENGTH);
                                                            clientll_get_by_socket((*s).clients, act_sock)->last_time = time_in_milli();
                                                            return_value = recv(act_sock, first_read_buf, FIRST_READ, 0);
                                                            if (return_value < 1) {
                                                                closed_stream(act_sock, return_value);
                                                                continue;
                                                            } else {
                                                                if (strncmp(MAGIC, first_read_buf, HEAD_LENGTH) != 0) {
                                                                    disconnect_client(act_sock, "Odpojil jsem klienta, spatna hlavicka pri aktualizaci.");
                                                                    continue;
                                                                }
                                                                second_read_length = strtol(first_read_buf + HEAD_LENGTH, NULL, 10);
                                                                if(second_read_length > 0) {
                                                                    memset(incoming_buf, 0, MAX_BUF_LENGTH);
                                                                    return_value = recv(act_sock, incoming_buf, second_read_length, 0);
                                                                    if(return_value < 1) {

                                                                        closed_stream(act_sock, return_value);
                                                                        continue;
                                                                    } else {
                                                                        printf("(%lld) Odpoved klienta (%s) na aktualizaci:%s%s\n", time_in_milli() - start_time, clientll_get_by_socket((*s).clients, act_sock)->nickname, first_read_buf, incoming_buf);
                                                                        handler_return = handle_server_iniciative(incoming_buf, act_sock, curr_sock, outcoming_buf);

                                                                        if (handler_return == EXIT_FAILURE) {
                                                                            die(ERR_PARAM_PRINT, ERR_PARAM);
                                                                            return;
                                                                        } else if(handler_return == EXIT_UPDATE_OK) {
                                                                            continue;
                                                                        } else  {

                                                                            if(strlen(outcoming_buf) > MIN_MESS_LENGTH) {
                                                                                return_value = send(act_sock, outcoming_buf, strlen(outcoming_buf), 0);
                                                                                printf("(%lld) Odeslana zprava na socket %d (aktualizace): %s", time_in_milli() - start_time, act_sock, outcoming_buf);
                                                                                if(return_value == -1) {
                                                                                    closed_stream(act_sock, return_value);
                                                                                    continue;
                                                                                } else {
                                                                                    if(handler_return == EXIT_WARNING) {
                                                                                        if(increase_bad_counter_of_player(act_sock) == EXIT_FAILURE) {
                                                                                            disconnect_client(act_sock, "Odpojil jsem klienta, protoze posilal, co nemel.");
                                                                                        } else {
                                                                                            printf("Klient odpovedel spatne, ale jeste mu dam sanci (odpoved na aktualizaci).\n");
                                                                                        }
                                                                                    } else if(handler_return == EXIT_DISCONNECT) {
                                                                                        disconnect_client(act_sock, "Odpojil jsem klienta na jeho zadost.");
                                                                                    } else if(handler_return == EXIT_PING) {
                                                                                        continue;
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
