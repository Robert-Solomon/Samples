#include <stdio.h>

#define _BUF_SIZE 128
#define FALSE 0
#define TRUE 1

typedef int bool;

int main()
{
	int v, _v;
	char c, s[_BUF_SIZE];
	int i;
	bool digit_seen;
	bool non_numeric_seen;
	bool overflow;

	// do this as long as there is input, to exit just hit <ENTER>
	do
	{
		printf("Enter a string, to finish hit <ENTER>: ");

		// read in the input string first
		for (i = 0; i < _BUF_SIZE; i++)
		{
			if ((s[i] = getchar()) == '\n')
			{
				s[i] = '\0';
				break;
			}
		}

		// empty string - we are done
		if (i == 0)
			break;

		// string too long...
		if (i >= _BUF_SIZE)
		{
			printf("line too long. Maximum allowed: %d characters\n", _BUF_SIZE);
			continue;
		}

		// let's skip whitespaces first
		i = 0;
		while ((s[i] == ' ') || (s[i] == '\t'))
			i++;

		// only process numbers, discard trailint chars
		v = 0;
		digit_seen = FALSE;
		non_numeric_seen = FALSE;
		overflow = FALSE;
		while ((c = s[i++]) != '\0')
		{
			if ((c > '9') || (c < '0'))
			{
				non_numeric_seen = TRUE;
				continue;
			}

			digit_seen = TRUE;

			_v = 10 * v + (c - '0');
			if (v > _v)
			{
				overflow = TRUE;
				break;
			}
			v = _v;
		}

		if (overflow)
		{
			printf("number too big - overflow.\n");
		}
		else if (digit_seen)
		{
			if (non_numeric_seen)
				printf("non numeric characters discarded. ");
			printf("the value you entered is: %d = 0x%x\n", v, v);
		}
		else
		{
			printf("invalid (non number) input.\n");
		}
	} while (TRUE);
}
