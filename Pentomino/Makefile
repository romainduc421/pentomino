CC = gcc
CFLAGS = -W -Wall -ansi -std=c99 -g
LIBS = 
LDFLAGS = `sdl2-config --cflags --libs`
INCLUDES =
EXEC = main
SRC = main.c sdl_functions.c
OBJ = $(SRC:.c=.o)

all: $(EXEC)
main : $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LIBS) $(LDFLAGS)
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<
clean:
	rm -rf *.o *~ main
mrproper: clean
	rm -rf $(EXEC)
