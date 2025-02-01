FLAGS=-Wall -Werror -Wextra
CC=gcc
BOUT=../build/


cipher: cipher.o

	$(CC) cipher.o -o $(BOUT)cipher

logging_cipher: cipher.o logger.o

	$(CC) cipher.o logger.o -o $(BOUT)logging_cipher

cipher.o:

	$(CC) $(FLAGS) cipher.c -c

logger.o:

	$(CC) $(FLAGS) logger.c -c

rebuild: clean cipher

clean:
	rm -rf *.o