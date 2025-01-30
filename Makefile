CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
LIBS = -lncurses  
SRC = src/main.c
OUT = build/VaultEdit

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LIBS)

clean:
	rm -f $(OUT)
