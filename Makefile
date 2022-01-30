# Jack Tarantino --  jtarant3
# Weina Dai -- wdai11

CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra
all: render_tone render_song render_echo

render_tone: io.o wave.o render_tone.o
	$(CC) -o render_tone io.o wave.o render_tone.o -lm

render_song: io.o wave.o render_song.o
	$(CC) -o render_song io.o wave.o render_song.o -lm

render_echo: io.o wave.o render_echo.o
	$(CC) -o render_echo io.o wave.o render_echo.o -lm

io.o: io.c io.h
	$(CC) $(CFLAGS) -c io.c -lm

wave.o: wave.c wave.h io.h
	$(CC) $(CFLAGS) -c wave.c -lm

render_tone.o: render_tone.c io.h wave.h
	$(CC) $(CFLAGS) -c render_tone.c -lm

render_song.o: render_song.c io.h wave.h
	$(CC) $(CFLAGS) -c render_song.c -lm

render_echo.o: render_echo.c io.h wave.h
	$(CC) $(CFLAGS) -c render_echo.c -lm

clean:
	rm -f *.o render_tone render_song render_echo
