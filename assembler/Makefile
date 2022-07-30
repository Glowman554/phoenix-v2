SRCS = $(shell find -name '*.[c]')
CPPSRC = $(shell find -name '*.cpp')

OBJS = $(addsuffix .o,$(basename $(SRCS) $(CPPSRC)))

CC = gcc
LD = g++

CFLAGS = -g -Iinclude
LDFLAGS =

as.elf: $(OBJS)
	@echo LD $^
	@$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	@echo CC $^
	@$(CC) $(CFLAGS) -c -o $@ $^

%.o: %.cpp
	@echo CPP $^
	@$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm $(OBJS)