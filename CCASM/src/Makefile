CC = clang
CFLAGS = -Wall -Wextra -Wunreachable-code
DBGFLAGS = -Werror -g

all:
	$(CC) $(CFLAGS) -c parser.c -o build/parser.o
	$(CC) $(CFLAGS) -c utils.c -o build/utils.o
	$(CC) $(CFLAGS) -c errors.c -o build/errors.o
	$(CC) $(CFLAGS) -c lexer.c -o build/lexer.o
	$(CC) $(CFLAGS) -c preprocessor.c -o build/preprocessor.o

	$(CC) $(CFLAGS) main.c build/parser.o build/utils.o build/errors.o build/lexer.o build/preprocessor.o -o main

dbg:
	$(CC) $(CFLAGS) $(DBGFLAGS) -c parser.c -o build/parser.o
	$(CC) $(CFLAGS) $(DBGFLAGS) -c utils.c -o build/utils.o
	$(CC) $(CFLAGS) $(DBGFLAGS) -c errors.c -o build/errors.o
	$(CC) $(CFLAGS) $(DBGFLAGS) -c lexer.c -o build/lexer.o
	$(CC) $(CFLAGS) $(DBGFLAGS) -c preprocessor.c -o build/preprocessor.o

	$(CC) $(CFLAGS) $(DBGFLAGS) main.c build/parser.o build/utils.o build/errors.o build/lexer.o build/preprocessor.o -o main
