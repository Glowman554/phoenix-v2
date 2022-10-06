%.bin: %.asm
	@echo AS $<
	@phoenix-as $< $@

all: $(patsubst %.asm,%.bin,$(wildcard *.asm))

clean:
	rm *.bin -v