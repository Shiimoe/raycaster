CC ?= gcc
LINKS := -lSDL2 -lm -lSDL2_image -lcrelude -lSDL2_ttf -I. -I./include
WARN := -Wall -Wextra -Wpedantic
OPTIONS := -funsigned-char
CFLAGS := $(WARN) $(OPTIONS)
SRCS := init.c main.c controls.c player.c
OBJS := $(SRCS:.c=.o)
EXEC := main

all: $(EXEC)

clean:
	rm -f $(EXEC) $(OBJS)

fresh: clean all

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(LINKS)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC) $(LINKS)

.PHONY: all clean fresh