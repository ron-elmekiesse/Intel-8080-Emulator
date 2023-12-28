#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_EXTRA_DATA (2)

#define BIG_TO_LITTLE_ENDIAN(num) ((num>>8) | (num<<8));

#define STACK_SIZE (2048)

#define SPACE_DELIM " "

typedef struct _CPU {
	UINT8 A;
	UINT8 B;
	UINT8 C;
	UINT8 D;
	UINT8 E;
	UINT8 H;
	UINT8 L;
	BYTE Flags;				// Flags { S , Z , - , AC , - , P , - , C }
	char* SP;	// ESP -> stack pointer.
	BYTE* PC;	// EIP -> program counter / instruction pointer.
} CPU;

extern CPU cpu;
extern char stack[];
extern BYTE* data; // binary data (intel 8080)

enum FLAGS_INDEX {SIGN = 0, ZERO = 1, AUXILIARY_CARRY = 3, PARITY = 5, CARRY = 7}; // Flags indexes in the Flag register.

int open_file(FILE** fd);

unsigned int get_file_size(FILE* fd);

int get_register_ptr(char reg, UINT8** out);

int set_bit(unsigned int bit_index, unsigned int bit_val, char* num);

int get_bit_state(unsigned int bit_index, char* num);

char* get_regs_from_instruction(char* instruction);

void Push_PC();

void Pop_PC();

void handle_Sign_Flag(UINT8 result);

void handle_Zero_Flag(int result);

void handle_Auxiliary_Carry_Flag(int val1, int val2, char operation);

void handle_Parity_Flag(BYTE result);

void handle_Carry_Flag(int val1, int val2, char operation, int size_of_data_type);

void handle_FLAGS(int result, int val1, int val2, char operation, int size_of_data_type,
	int check_SIGN, int check_ZERO, int check_AUXILIARY_CARRY, int check_PARITY, int check_CARRY);