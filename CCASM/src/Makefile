CC = clang
CFLAGS = -Wall -Wextra -Wunreachable-code

all:
	$(CC) $(CFLAGS) -c errors.c -o build/errors.o
	$(CC) $(CFLAGS) -c lexer.c -o build/lexer.o
	$(CC) $(CFLAGS) -c preprocessor.c -o build/preprocessor.o

	$(CC) $(CFLAGS) main.c build/errors.o build/lexer.o build/preprocessor.o -o main
