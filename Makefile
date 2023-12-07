CC = clang
CFLAGS =-std=c17 -pedantic -Wall
LDFLAGS = 
OBJ = fichier.o game.o algo.o option.o main.o
EXE = JO
REG = src/

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
	
main.o: $(REG)main.c $(REG)option.h

option.o: $(REG)option.c $(REG)option.h $(REG)algo.h

algo.o: $(REG)algo.c $(REG)algo.h $(REG)game.h $(REG)fichier.h

fichier.o: $(REG)fichier.c $(REG)fichier.h

game.o: $(REG)game.c $(REG)game.h

%.o: $(REG)%.c
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o

mrproper: clean
	rm -f $(EXE)

install: $(EXE)
	mkdir bin
	mv $(EXE) bin/$(EXE)
	make mrproper

uninstall: mrproper
	rm -f bin/$(EXE)
	rm -rf bin
