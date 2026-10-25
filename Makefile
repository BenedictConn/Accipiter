CC = gcc
CLAGS = -Wall -Wextra -g 
LDFLAGS = -mwindows
LDLIBS = -lgdi32

Accipiter: Accipiter.c
	$(CC) $(CFLAGS) $(LDFLAGS) Accipiter.c -o Accipiter $(LDLIBS)

clean:
	rm -f Accipiter