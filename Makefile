CC = clang
CFLAGS = -std=c17 -pedantic -Wall -Wfatal-errors
LDFLAGS = -lMLV

SRC_DIR = src
OBJ_DIR = build


SRC = $(wildcard $(SRC_DIR)/*.c)

OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

EXE = AF


all: $(EXE)

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)


clean:
	rm -rf $(OBJ_DIR)/

mrproper: clean
	rm -f $(EXE)

uninstall: mrproper


.PHONY: all clean mrproper install uninstall
