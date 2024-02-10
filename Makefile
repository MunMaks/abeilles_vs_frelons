CC = clang
CFLAGS = -std=c17 -pedantic -Wall -Wfatal-errors
LDFLAGS = -lMLV

SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin


SRC = $(wildcard $(SRC_DIR)/*.c)

OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

EXE = AF


all: $(BIN_DIR)/$(EXE)

$(BIN_DIR)/$(EXE): $(OBJ)
	mkdir $(BIN_DIR)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)


$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c $(SRC_DIR)/graphique.h

$(OBJ_DIR)/graphique.o: $(SRC_DIR)/graphique.c $(SRC_DIR)/graphique.h $(SRC_DIR)/bataille.h

$(OBJ_DIR)/bataille.o: $(SRC_DIR)/bataille.c $(SRC_DIR)/bataille.h $(SRC_DIR)/plateau.h

$(OBJ_DIR)/plateau.o: $(SRC_DIR)/plateau.c $(SRC_DIR)/plateau.h $(SRC_DIR)/ajoute_supp.h

$(OBJ_DIR)/ajoute_supp.o: $(SRC_DIR)/ajoute_supp.c $(SRC_DIR)/ajoute_supp.h


%.o: $(SRC_DIR)%.c
	$(CC) -c $< $(CFLAGS)


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir --parents $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)


clean:
	rm -rf $(OBJ_DIR)/

mrproper: clean
	rm -rf $(BIN_DIR)/

uninstall: mrproper
	rm -f $(BIN_DIR)/$(EXE)

.PHONY: all clean mrproper install uninstall
