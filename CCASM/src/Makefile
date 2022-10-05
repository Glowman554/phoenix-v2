CC = clang
CFLAGS = -Wall -Wextra -Wunreachable-code -g

OBJS = $(patsubst %.c,%.o,$(wildcard *.c))

main: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm $(OBJS) main

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

