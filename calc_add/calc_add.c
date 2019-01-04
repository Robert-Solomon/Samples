// calc app
#include <stdio.h>

#include <stdlib.h>

int main(int argc, char *argv[])
{
	int v1, v2;
	if (argc != 3)
	{
		printf("useage: %s v1 v2\n", argv[0]);
		exit(1);
	}
	v1 = atoi(argv[1]);
	v2 = atoi(argv[2]);
	printf("%d + %d = %d\n",v1, v2, v1+v2);
	return 0;
}
