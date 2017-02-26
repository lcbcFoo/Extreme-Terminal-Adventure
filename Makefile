# Author: Lucas de Camargo Barros de Castro (Foo)
# Date: 02/26/2017


# Define game laucher .c files
L_FILES= main.c init.c command.c combat.c autogen.c

# Default rule
all: launcher


# Create game launcher
launcher: $(L_FILES)
	gcc $(L_FILES) -o $@ -std=c99 -lm -lncurses

# Clean
clean:
	rm -f *.s *.o
