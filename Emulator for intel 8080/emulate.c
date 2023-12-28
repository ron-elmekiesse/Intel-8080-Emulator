#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "emulate.h"
#include "disassemble.h"
#include "utils.h"


void start_emulate()
{
	unsigned int file_size = 0;
	FILE* fd = 0;

	if (open_file(&fd))
		return -1;

	file_size = get_file_size(fd);
	
	printf("Stack addr: 0x%p\n", stack);

	cpu.SP = stack; // initializing the Stack Pointer.

	disass_and_handle(fd, file_size);

	display_regs_after_emulate();

	display_stack_after_emulate();
}

void display_regs_after_emulate()
{
	printf("\nCPU:\n");
	printf("A [%d]\n", cpu.A);
	printf("B [%d]\n", cpu.B);
	printf("C [%d]\n", cpu.C);
	printf("D [%d]\n", cpu.D);
	printf("E [%d]\n", cpu.E);
	printf("H [%d]\n", cpu.H);
	printf("L [%d]\n", cpu.L);
	printf("Flags [%d]\n", cpu.Flags);
	printf("SP [0x%p]\n", cpu.SP);
	printf("PC [0x%p]\n", cpu.PC);
}

void display_stack_after_emulate()
{
	char* ptr = NULL;
	printf("\nStack:\n");
	for (ptr = stack + 1; ptr < cpu.SP; ptr++) // we start inserting values to stack from (stack + 1) addr.
	{
		printf("%02x ", *ptr);
	}
	puts("");
}