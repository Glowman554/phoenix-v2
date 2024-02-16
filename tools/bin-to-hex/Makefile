CC = clang
CFLAGS = -Iinclude -I../common -g

OBJS = $(patsubst %.c,%.o,$(wildcard *.c))

OUTPUT = bin-to-hex.elf

$(OUTPUT): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm $(OBJS) $(OUTPUT)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

