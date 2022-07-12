
#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <time.h>

#include "../headers/glob.h"
#include "server_def.h"
#include "../client/client.h"
#include "../client/client_functions/client_functions.h"
#include "server_functions/server_functions.h"

/**
 * Buffer ktery slouzi k ukladani posilanych zprav.
 */
extern char outcoming_buf[MAX_BUF_LENGTH];

/**
 * Funkce, ktera se stara o beh server. K paralelizaci
 * komunikace s klienty pouziva select.
 */
void run_server();

#endif
