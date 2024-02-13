%.bin: %.asm
	@echo AS $<
	@phoenixv2-as $< $@

%.hex: %.asm
	@echo AS $<
	@phoenixv2-as $< $@

all: $(patsubst %.asm,%.bin,$(wildcard *.asm)) $(patsubst %.asm,%.hex,$(wildcard *.asm))

clean:
	rm *.bin *.hex -v