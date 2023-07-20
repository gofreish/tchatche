CC = gcc
CFLAGS = -Wall
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

all: $(BIN_DIR)/serveur

$(OBJ_DIR)/connection.o: $(SRC_DIR)/cmd/connection.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJ_DIR)/public_msg.o: $(SRC_DIR)/cmd/public_msg.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJ_DIR)/outils.o: $(SRC_DIR)/outils.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(BIN_DIR)/serveur: $(OBJ_DIR)/outils.o $(OBJ_DIR)/connection.o $(OBJ_DIR)/public_msg.o $(OBJ_DIR)/main.o
	$(CC) $^ -o $@

clean:
	rm -f $(OBJ_DIR)/*.o