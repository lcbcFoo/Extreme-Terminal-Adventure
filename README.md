# Extreme-Terminal-Adventure

Source code of Extreme Terminal Adventure.

# ETA Team:

Lucas "Foo" de Castro

Eduardo "Chamyto" Souza

Heitor "Led" Comel

-------------------------------------------------------------------------------------------------

"main.c" is the code which should be compiled in order to get the game executable.

"combat.c", "command.c" and "init.c" contain the functions described at "combat.h", "command.h" and "init.h"

Files ended with '.bin' are databases used by the game.

Other files ended with '.c' are auxiliar programs, you can use them to modify the game if you want.


To play the game, execute the following lines on the terminal while in the directory you want to
save the game:

================================================================================================

$ sudo git init

$ git clone https://github.com/lcbcFoo/Extreme-Terminal-Adventure.git

$ cd Extreme-Terminal-Adventure/

$ gcc main.c -std=c99 -lm -o launcher combat.c command.c init.c autogen.c

$ ./launcher


