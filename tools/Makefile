%.elf: %.c
	@echo CC $<
	@gcc $< -o $@

all: $(patsubst %.c,%.elf,$(wildcard *.c))

clean:
	rm *.elf *.bin -v