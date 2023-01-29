CC = clang
CXX = clang
CFLAGS = -Iinclude -I../common -g
ifeq ($(SANITIZE), true)
	CFLAGS += -fsanitize=address,undefined
endif

OBJS = $(patsubst %.c,%.o,$(wildcard *.c)) $(patsubst %.cpp,%.o,$(wildcard *.cpp))
TESTS = $(patsubst %.asm,%.bin,$(wildcard test/*.asm))

OUTPUT = assembler.elf

$(OUTPUT): $(OBJS)
	$(CXX) $(CFLAGS) $^ -o $@

test: $(OUTPUT) $(TESTS)

clean:
	rm $(OBJS) $(TESTS) $(OUTPUT)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

%.bin: %.asm
	./$(OUTPUT) $< $@

format:
	find . -regex '.*\.\(cpp\|hpp\|cu\|c\|h\)' -exec clang-format --verbose -style=file -i {} \;
