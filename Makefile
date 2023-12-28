# Compiler and flags
CC = clang
CFLAGS = -std=c17 -pedantic -Wall

# Directories
SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin

# Source files
SRC = $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

# Executable name
EXE = AF

# Targets
all: $(BIN_DIR)/$(EXE)
#mkdir -p $(BIN_DIR)
$(BIN_DIR)/$(EXE): $(OBJ)
	mkdir $(BIN_DIR)
	$(CC) -o $@ $^ $(CFLAGS)

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c $(SRC_DIR)/graphique.h

$(OBJ_DIR)/graphique.o: $(SRC_DIR)/graphique.c $(SRC_DIR)/graphique.h $(SRC_DIR)/bataille.h $(SRC_DIR)/fichier.h

$(OBJ_DIR)/bataille.o: $(SRC_DIR)/bataille.c $(SRC_DIR)/bataille.h $(SRC_DIR)/abeille.h $(SRC_DIR)/frelon.h

$(OBJ_DIR)/fichier.o: $(SRC_DIR)/fichier.c $(SRC_DIR)/fichier.h $(SRC_DIR)/bataille.h

$(OBJ_DIR)/abeille.o: $(SRC_DIR)/abeille.c $(SRC_DIR)/abeille.h $(SRC_DIR)/plateau.h

$(OBJ_DIR)/frelon.o: $(SRC_DIR)/frelon.c $(SRC_DIR)/frelon.h $(SRC_DIR)/plateau.h

$(OBJ_DIR)/plateau.o: $(SRC_DIR)/plateau.c $(SRC_DIR)/plateau.h

# Rule to build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

# Clean up
clean:
	rmdir /s /q $(BIN_DIR)
	rmdir /s /q $(OBJ_DIR)

# Full clean, including executable
mrproper: clean
	rm -rf $(BIN_DIR)


# Install the executable
# mkdir -p $(BIN_DIR)
install: $(BIN_DIR)/$(EXE)
	
	mv $(BIN_DIR)/$(EXE) $(BIN_DIR)

# Uninstall the executable
uninstall: mrproper
	rm -f $(BIN_DIR)/$(EXE)

.PHONY: all clean mrproper install uninstall
