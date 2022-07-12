cmake_minimum_required(VERSION 3.20)
project(server C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(server src/main/main.c src/client/client.c src/client/client.h src/server/config_server/config_server.c src/server/config_server/config_server.h src/server/server.c src/server/server.h src/game/feed/feed.c src/game/feed/feed.h src/headers/dec.h src/game/room/room.c src/game/room/room.h src/client/list/linkedlist.c src/client/list/linkedlist.h src/server/server_def.h src/headers/glob.h src/client/client_functions/client_functions.c src/client/client_functions/client_functions.h src/server/server_functions/server_functions.c src/server/server_functions/server_functions.h)

target_link_libraries(server PRIVATE Threads::Threads)