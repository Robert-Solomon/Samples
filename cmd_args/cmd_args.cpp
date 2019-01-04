// cmd_args.cpp : This sample prints the command line arguments to the standard output.
//

#include "stdio.h"

int main(int argc, char *argv[], char *envp[])
{
	int i;
	printf("argc = 0x%x\n", argc);
	for (i = 0; i < argc; i++)
		printf("arg[%d]: %s\n", i, argv[i]);
	for(i = 0; envp[i] != NULL; i++)
		printf("env[%d]: %s\n", i, envp[i]);
}

