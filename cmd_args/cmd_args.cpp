// cmd_args.cpp : This sample prints the command line arguments to the standard output.
//

#include "stdio.h"

int main(int argc, char *argv[])
{
	int i;
	for (i = 0; i < argc; i++)
		printf("arg[%d]: %s\n", i, argv[i]);
}

