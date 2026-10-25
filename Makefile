CC = gcc
CLAGS = -Wall -Wextra -g

Accipiter: Accipiter.c
	$(CC) $(CFLAGS) Accipiter.c -o Accipiter

clean:
	rm -f Accipiter