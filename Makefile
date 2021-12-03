CC ?= gcc
LINKS := -lSDL2 -lm -lSDL2_image -lcrelude -lSDL2_ttf
WARN := -Wall -Wextra -Wpedantic
OPTIONS := -funsigned-char
CFLAGS := $(WARN) $(OPTIONS)
SRCS := init.c main.c controls.c player.c
OBJS := $(SRCS:.c=.o)
DBS := $(OBJS:.o:.o.json)
DB := false
EXEC := main

all: $(EXEC)

clean:
	rm -f $(EXEC) $(OBJS)

fresh: clean all

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(LINKS)
	@$(DB) \
		&& clang -MJ $@.json $(CFLAGS) -c $< -o $@ $(LINKS) \
		|| true

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC) $(LINKS)

db: DB=true
db: $(DBS) all
	sed -e '1s/^/[\n/' -e '$$s/,$$/\n]/' *.o.json > compile_commands.json



.PHONY: all clean fresh db