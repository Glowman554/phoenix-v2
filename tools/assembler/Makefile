CC = clang
CXX = clang++
CFLAGS = -Iinclude -I../common -g

OBJS = $(patsubst %.c,%.o,$(wildcard *.c)) $(patsubst %.cpp,%.o,$(wildcard *.cpp))

OUTPUT = assembler.elf

$(OUTPUT): $(OBJS)
	$(CXX) $(CFLAGS) $^ -o $@

clean:
	rm $(OBJS) $(OUTPUT)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@
