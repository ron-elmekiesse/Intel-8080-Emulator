#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define JUMPS 1
#define NO_JUMP 0

typedef struct _OPCODE
{
	unsigned int opcode;
	char* instruction;
	unsigned int size;
	unsigned int needs_to_jump;
	void (*func) (char* instruction, unsigned short extra_data);
} OPCODE;

void disass_and_handle(FILE* fd, unsigned int file_size);