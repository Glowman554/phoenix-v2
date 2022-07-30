%.bin: %.asm
	@echo AS $<
	@ccpu-as $< $@

all: $(patsubst %.asm,%.bin,$(wildcard *.asm))

clean:
	rm *.bin -v