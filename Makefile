CC ?= gcc
LINKS := -lSDL2 -lm -lSDL2_image -lcrelude -lSDL2_ttf -I. -I./include
WARN := -Wall -Wextra -Wpedantic
OPTIONS := -funsigned-char
CFLAGS := $(LINKS) $(WARN) $(OPTIONS)
SRCS := init.c, main.c, controls.c, player.c, header.h
OBJS := $(SRCS:.c=.o)
EXEC := main

all: $(EXEC)

clean:
	rm -f $(EXEC)

fresh: clean all

.PHONY: all clean fresh