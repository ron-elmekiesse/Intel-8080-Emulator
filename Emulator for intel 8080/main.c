#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "emulate.h"

void print_binary(signed short val)
{
	printf("origin val: %d\n", val);
	for (int i = 0; i < 16; i++)
	{
		printf("%d:\t%d\n", 1<<i, val & 1);
		val >>= 1;
	}
	puts("");
}


int main()
{
	int temp = 0;
	/*temp = 126;
	printf("%d\n", (signed char)temp);
	print_binary(temp);
	puts("----------------");
	temp = 127;
	printf("%d\n", (signed char)temp);
	print_binary(temp);
	puts("----------------");
	temp = 128;
	printf("%d\n", (signed char)temp);
	print_binary(temp);
	puts("----------------");
	temp = 129;
	printf("As signed char: %d\n", (signed char)temp);
	printf("As signed int: %d\n", (signed int)temp);
	print_binary(temp);*/
	
	/*temp = 120;
	printf("%d\n", (signed char)(temp + 8));
	print_binary(temp + 8);
	puts("----------------");*/


	start_emulate();

	return 0;
}

