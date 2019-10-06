
#include <iostream>

//
// copy maxBytes-1 characters to the input buffer up to a newline character.
// guaranteed 'nil' termination
// returns the number of characters copied.
// returns -1 if end of input reached.
// return -2 to indicate input parameter error
//

int GetString(char* p, int maxBytes)
{
	static char buf[4096];
	static int Head = 0, Tail = 0;
	static bool EndOfInput = false;

	int bytesCopied = 0;

	if (maxBytes < 1 || p == NULL)
		return -2;

	if (((Tail - Head) == 0) && (EndOfInput == true))
		return -1;

	while (maxBytes > 1)
	{

	}

	*p = '\0';
	return bytesCopied;
}

int main()
{
    std::cout << "Hello World!\n";
}

