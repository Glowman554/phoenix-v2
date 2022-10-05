CC = clang
CFLAGS = -Iinclude -Wall -Wextra -Wunreachable-code -g

OBJS = $(patsubst %.c,%.o,$(wildcard *.c))

OUTPUT = visual_emu.elf

$(OUTPUT): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm $(OBJS) $(OUTPUT)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

