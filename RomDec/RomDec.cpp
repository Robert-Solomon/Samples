// RomDec.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <iostream>

#define SZ_INS  128

// skip initial whitespace characters. The string is guaranteed to terminate in a '\0'
char* SkipWS(char* p)
{
	while (isspace(*p))
		p++;
	return p;
}

const char* strings_1000[] = { "M", "MM", "MMM" };
const char* strings_100[] = { "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM" };
const char* strings_10[] = { "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC" };
const char* strings_1[] = { "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX" };
void DecToRom(char* op, int v)
{
	int t;

	t = v / 1000;
	if (t)
	{
		strcpy(op, strings_1000[t - 1]);
		op += strlen(op);
	}
	v = v % 1000;

	t = v / 100;
	if (t)
	{
		strcpy(op, strings_100[t - 1]);
		op += strlen(op);
	}
	v = v % 100;

	t = v / 10;
	if (t)
	{
		strcpy(op, strings_10[t - 1]);
		op += strlen(op);
	}
	v = v % 10;

	if (v)
	{
		strcpy(op, strings_1[v - 1]);
	}
}

int RomVal(char c)
{
	switch (c)
	{
	case 'I': return 1;
	case 'V': return 5;
	case 'X': return 10;
	case 'L': return 50;
	case 'C': return 100;
	case 'D': return 500;
	case 'M': return 1000;
	}
	return 0;
}

unsigned IsValidRoman(char* cp)
{
	while (RomVal(*cp))
		cp++;
	if (!isspace(*cp))
		return 0;
	*cp = '\0';
	return 1;
}

int RomToDec(char* cp)
{
	int v = 0;
	int c1, c2;
	while ((c1 = RomVal(*cp)) != 0)
	{
		c2 = RomVal(*(cp + 1));
		if (c2 > c1)
		{
			v += c2 - c1;
			cp += 2;
		}
		else
		{
			v += c1;
			cp++;
		}
	}
	return v;
}

int main()
{
	char ins[SZ_INS+1];
	char outs[32];
	char* cp;
	int dVal;
	while (fgets(ins, SZ_INS, stdin))
	{
		cp = SkipWS(ins);
		if ((*cp == '\0') || (*cp == 'Q'))
			break;
		if (isdigit(*cp))
		{
			// we are converting decimal to Roman, so validate the input
			dVal = 0;
			while (isdigit(*cp))
				dVal = dVal * 10 + (*cp++ - '0');
			if (dVal > 3999)
			{
				std::cout << "==> invalid decimal input " << dVal << '\n';
				continue;
			}
			DecToRom(outs, dVal);
			std::cout << "==> Dec: " << dVal << "  ->  Rom: " << outs << '\n';
		}
		else
		{
			// we are converting Roman to decimal, so validate the input
			if (!IsValidRoman(cp))
			{
				std::cout << "==> invalid Roman input " << cp << '\n';
				continue;
			}
			dVal = RomToDec(cp);
			std::cout << "==> Rom: " << cp << "  ->  Dec: " << dVal << '\n';
		}
	}
	return 0;
}

