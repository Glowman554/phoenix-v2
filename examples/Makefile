%.bin: %.asm
	@echo AS $<
	phoenixv2-as $< $@

all: $(patsubst %.asm,%.bin,$(wildcard *.asm))

clean:
	rm *.bin -v