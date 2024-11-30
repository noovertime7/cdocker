CC = gcc
CFLAGS = -g -Wall -Wextra -D_GNU_SOURCE
SRC = main.c include/container.c include/tools.c include/namespace.c
OBJDIR = build
OBJS = $(addprefix $(OBJDIR)/,$(notdir $(SRC:.c=.o)))
EXEC = $(OBJDIR)/cdocker

.PHONY: all clean debug run

all: $(EXEC)

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: include/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR)

debug:
	gdbserver :1234 $(EXEC) --debug-mode

run:
	sudo $(EXEC)