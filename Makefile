CC = gcc
CFLAGS = -lpthread -Wall -Wextra -pedantic -g
LDFLAGS = $(CFLAGS)

BUILD_DIR = build
BIN = server.exe
OBJ = main.o server.o server_functions.o config_server.o room.o client.o client_functions.o linkedlist.o feed.o

all: clean $(BUILD_DIR) $(BIN)

$(BIN): $(BUILD_DIR)/main.o $(BUILD_DIR)/server.o $(BUILD_DIR)/server_functions.o $(BUILD_DIR)/config_server.o $(BUILD_DIR)/room.o $(BUILD_DIR)/client.o $(BUILD_DIR)/client_functions.o $(BUILD_DIR)/linkedlist.o $(BUILD_DIR)/feed.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/main.o: src/main/main.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR)/server.o: src/server/server.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR)/server_functions.o: src/server/server_functions/server_functions.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR)/config_server.o: src/server/config_server/config_server.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR)/room.o: src/game/room/room.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR)/client.o: src/client/client.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR)/client_functions.o: src/client/client_functions/client_functions.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR)/linkedlist.o: src/client/list/linkedlist.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR)/feed.o: src/game/feed/feed.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR):
	mkdir $@

clean:
	rm -f ./build/*.o
