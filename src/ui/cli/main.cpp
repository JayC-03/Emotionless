#include "core/main.h"

int main(int ac, char** av)
{
	if(ac > 2) main_loop(av[1],av[2]);
    else main_loop(av[1], "");
}
