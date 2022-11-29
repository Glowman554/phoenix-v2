CC = clang
CXX = clang++
CFLAGS = -Iinclude -I../json/single_include -I../common -g

OBJS = $(patsubst %.c,%.o,$(wildcard *.c)) $(patsubst %.cpp,%.o,$(wildcard *.cpp))

OUTPUT = tester.elf

$(OUTPUT): $(OBJS)
	$(CXX) $(CFLAGS) $^ -o $@

clean:
	rm $(OBJS) $(OUTPUT)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@
