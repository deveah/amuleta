
CC=clang
CFLAGS=-Wall -Wextra -ansi -g3 -c
LDFLAGS=-ltermbox
SOURCES=src/log.c src/tile.c src/game.c src/dungeon.c src/ui.c src/main.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=amuleta

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS)
