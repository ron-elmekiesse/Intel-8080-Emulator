#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "make_instruction.h"
#include "utils.h"

/*
	TODO: After every instruction update the FLAGS register!
*/


// Example: "NOP"
void handle_NOP(char* instruction, unsigned short extra_data)
{
	// Nothing to do with NOP.
}

// Example: "MOV B,H"	B <- H
void handle_MOV(char* instruction, unsigned short extra_data)
{
	char* token = NULL;
	UINT8* reg_left = NULL;
	UINT8* reg_right = NULL;
	int res = 0;

	token = get_regs_from_instruction(instruction);

	res = get_register_ptr(token[0], &reg_left);
	if (res) // if invalid reg.
		return;
	res = get_register_ptr(token[2], &reg_right);
	if (res)
		return;

	*reg_left = *reg_right; // Make the MOV.

	free(token);
}

// Example: "ADD H"		A <- A + H
void handle_ADD(char* instruction, unsigned short extra_data)
{
	int res = 0;
	UINT8* reg = NULL;

	res = get_register_ptr(instruction[strlen(instruction) - 1], &reg);
	if (res)
		return;

	handle_FLAGS(cpu.A + *reg, cpu.A, *reg, '+', sizeof(UINT8), SIGN, ZERO, AUXILIARY_CARRY, PARITY, CARRY);

	cpu.A += *reg; // Make the ADD
}

// Example: "SUB C"		A <- A - C
void handle_SUB(char* instruction, unsigned short extra_data)
{
	int res = 0;
	UINT8* reg = NULL;

	res = get_register_ptr(instruction[strlen(instruction) - 1], &reg);
	if (res)
		return;

	handle_FLAGS(cpu.A - *reg, cpu.A, *reg, '-', sizeof(UINT8), SIGN, ZERO, AUXILIARY_CARRY, PARITY, CARRY);

	cpu.A -= *reg;
}

// Example: "ADC B"		A <- A + B + CY,	 CY is the Carry flag.
void handle_ADC(char* instruction, unsigned short extra_data)
{
	int res = 0;
	UINT8* reg = NULL;

	res = get_register_ptr(instruction[strlen(instruction) - 1], &reg);
	if (res)
		return;

	res = cpu.A + *reg + get_bit_state(CARRY, &cpu.Flags);

	handle_FLAGS(res, cpu.A + *reg, get_bit_state(CARRY, &cpu.Flags), '+', sizeof(UINT8), SIGN, ZERO, AUXILIARY_CARRY, PARITY, CARRY);

	cpu.A = res;
}

// Example: "SBB D"		A <- A - D - CY,	 CY is the Carry flag.
void handle_SBB(char* instruction, unsigned short extra_data)
{
	int res = 0, prev_CpuA = 0;
	UINT8* reg = NULL;

	res = get_register_ptr(instruction[strlen(instruction) - 1], &reg);
	if (res)
		return;

	prev_CpuA = cpu.A;

	res = cpu.A - *reg - get_bit_state(CARRY, &cpu.Flags);

	handle_FLAGS(res, cpu.A - *reg, get_bit_state(CARRY, &cpu.Flags), '-', sizeof(UINT8), SIGN, ZERO, AUXILIARY_CARRY, PARITY, CARRY);

	cpu.A = res;
}

// Example: "ANA E"		A <- A & E
void handle_ANA(char* instruction, unsigned short extra_data)
{
	int res = 0, prev_CpuA = 0;
	UINT8* reg = NULL;

	res = get_register_ptr(instruction[strlen(instruction) - 1], &reg);
	if (res)
		return;

	res = cpu.A & *reg;

	handle_FLAGS(res, cpu.A, *reg, '&', sizeof(UINT8), SIGN, ZERO, -1, PARITY, -1);
	set_bit(CARRY, 0, &cpu.Flags); // Carry bit set to zero.

	cpu.A = res;
}

// Example: "XRA C"		A <- A ^ C
void handle_XRA(char* instruction, unsigned short extra_data)
{
	int res = 0;
	UINT8* reg = NULL;

	res = get_register_ptr(instruction[strlen(instruction) - 1], &reg);
	if (res)
		return;

	res = cpu.A ^ *reg;

	handle_FLAGS(res, cpu.A, *reg, '^', sizeof(UINT8), SIGN, ZERO, AUXILIARY_CARRY, PARITY, -1);
	set_bit(CARRY, 0, &cpu.Flags); // Carry bit set to zero.

	cpu.A = res;
}

// Example: "ORA D"		A <- A | D
void handle_ORA(char* instruction, unsigned short extra_data)
{
	int res = 0;
	UINT8* reg = NULL;

	res = get_register_ptr(instruction[strlen(instruction) - 1], &reg);
	if (res)
		return;

	res = cpu.A | *reg;

	handle_FLAGS(res, cpu.A, *reg, '|', sizeof(UINT8), SIGN, ZERO, -1, PARITY, -1);
	set_bit(CARRY, 0, &cpu.Flags); // Carry bit set to zero.

	cpu.A = res;
}

// Example: "CMP L"		Compare A -> L
void handle_CMP(char* instruction, unsigned short extra_data)
{
	int res = 0;
	UINT8* reg = NULL;

	res = get_register_ptr(instruction[strlen(instruction) - 1], &reg);
	if (res)
		return;

	res = cpu.A - *reg;
	// The comparison is performed by internally subtracting
	// the contents of REG from the accumulator(leaving both unchanged).
	handle_FLAGS(res, cpu.A, *reg, '-', sizeof(UINT8), SIGN, ZERO, AUXILIARY_CARRY, PARITY, CARRY);
}

// Example: "JNZ adr"	if NZ, PC <- adr, It jumps to the specified location if the Zero Flag (ZF) is cleared (0).
void handle_JNZ(char* instruction, unsigned short extra_data)
{
	int flag_state = 0;

	flag_state = get_bit_state(ZERO, &cpu.Flags);
	if (flag_state == 1)
		return;

	cpu.PC = data + extra_data - 1;
	// Example: if extra_data = 1 (instruction 1), we need to change PC to start of the data buf (index 0).
	// Pay attention -> mbby this will be needed to change in the Future for real addresses.
}

// Example: "JMP adr"	PC <= adr
void handle_JMP(char* instruction, unsigned short extra_data)
{
	cpu.PC = data + extra_data - 1;
}

// Example: "JZ adr"	if Z, PC < -adr, It jumps to the specified location if the Zero Flag (ZF) is set (1).
void handle_JZ(char* instruction, unsigned short extra_data)
{
	int flag_state = 0;

	flag_state = get_bit_state(ZERO, &cpu.Flags);
	if (flag_state == 0)
		return;

	cpu.PC = data + extra_data - 1;
}

// Example: "JNC adr"		if NCY, PC<-adr (CF == 0)
void handle_JNC(char* instruction, unsigned short extra_data)
{
	if (get_bit_state(CARRY, &cpu.Flags) == 0)
		cpu.PC = data + extra_data - 1;
}

// Example: "JC adr"		if CY, PC<-adr (CF == 1)
void handle_JC(char* instruction, unsigned short extra_data)
{
	if (get_bit_state(CARRY, &cpu.Flags) == 1)	
		cpu.PC = data + extra_data - 1;
}

// Example: "JPE adr"		if PE, PC <- adr (jump if parity even, PF == 1)
void handle_JPE(char* instruction, unsigned short extra_data)
{
	if (get_bit_state(PARITY, &cpu.Flags) == 1)
		cpu.PC = data + extra_data - 1;
}

// Example: "JPO adr"		if PO, PC <- adr (jump if parity odd, PF == 0)
void handle_JPO(char* instruction, unsigned short extra_data)
{
	if (get_bit_state(PARITY, &cpu.Flags) == 0)
		cpu.PC = data + extra_data - 1;
}

// Example: "JM adr"		if M, PC <- adr	 (jump if Minus (SF == 1))
void handle_JM(char* instruction, unsigned short extra_data)
{
	if (get_bit_state(SIGN, &cpu.Flags) == 1)
		cpu.PC = data + extra_data - 1;
}

// Example: "JP adr"		if NM, PC <- adr  (jump if Positive (SF == 0))
void handle_JP(char* instruction, unsigned short extra_data)
{
	if (get_bit_state(SIGN, &cpu.Flags) == 0)
		cpu.PC = data + extra_data - 1;
}

// Example: "INR B"		B <- B + 1
void handle_INR(char* instruction, unsigned short extra_data)
{
	int res = 0;
	UINT8* reg = NULL;

	res = get_register_ptr(instruction[strlen(instruction) - 1], &reg);
	if (res)
		return;

	res = (*reg)++;

	handle_FLAGS(res, (*reg), 1, '+', sizeof(UINT8), SIGN, ZERO, AUXILIARY_CARRY, PARITY, -1);

	(*reg)++;
}

// Example: "DCR C"		C <- C - 1
void handle_DCR(char* instruction, unsigned short extra_data)
{
	int res = 0;
	UINT8* reg = NULL;

	res = get_register_ptr(instruction[strlen(instruction) - 1], &reg);
	if (res)
		return;

	res = (*reg)--;

	handle_FLAGS(res, (*reg), 1, '-', sizeof(UINT8), SIGN, ZERO, AUXILIARY_CARRY, PARITY, -1);

	(*reg)--;
}

// Example: "RLC"		A = A << 1;  bit 0 = prev bit 7;  CY = prev bit 7
void handle_RLC(char* instruction, unsigned short extra_data)
{
	int res = 0;
	unsigned int bit7_state = 0;


	// Making the RLC instruction.
	bit7_state = get_bit_state(7, &cpu.A);
	if (!&cpu.A) return; // check if not NULL.
	cpu.A = (cpu.A) << 1;
	set_bit(0, bit7_state, &cpu.A);
	set_bit(CARRY, bit7_state, &cpu.Flags);
}

// Example: "RRC"		A = A >> 1;  bit 7 = prev bit 0;  CY = prev bit 0
void handle_RRC(char* instruction, unsigned short extra_data)
{
	int res = 0;
	unsigned int bit0_state = 0;


	// Making the RLC instruction.
	bit0_state = get_bit_state(0, &cpu.A);
	if (!&cpu.A) return; // check if not NULL.
	cpu.A = (cpu.A) >> 1;
	set_bit(7, bit0_state, &cpu.A);
	set_bit(CARRY, bit0_state, &cpu.Flags);
}

// Example: "RAL"		A = A << 1;  bit 0 = prev CY;  CY = prev bit 7
void handle_RAL(char* instruction, unsigned short extra_data)
{
	int res = 0;
	unsigned int bit7_state = 0;


	// Making the RLC instruction.
	bit7_state = get_bit_state(7, &cpu.A);
	if (!&cpu.A) return; // check if not NULL.
	cpu.A = (cpu.A) << 1;
	set_bit(0, get_bit_state(CARRY, &cpu.Flags), &cpu.A); // (cpu.A) bit 0 = prev CY
	set_bit(CARRY, bit7_state, &cpu.Flags);
}

// Example: "RAR"		A = A >> 1;  bit 7 = prev bit 7;  CY = prev bit 0
void handle_RAR(char* instruction, unsigned short extra_data)
{
	int res = 0;
	unsigned int bit0_state = 0;
	unsigned int bit7_state = 0;


	// Making the RLC instruction.
	bit0_state = get_bit_state(0, &cpu.A);
	bit7_state = get_bit_state(7, &cpu.A);
	if (!&cpu.A) return; // check if not NULL.
	cpu.A = (cpu.A) >> 1;
	set_bit(7, bit7_state, &cpu.A);
	set_bit(CARRY, bit0_state, &cpu.Flags);
}

// Example: "INX B"		BC <- BC + 1
void handle_INX(char* instruction, unsigned short extra_data)
{
	int res = 0;
	UINT8* reg = NULL;

	if (instruction[strlen(instruction) - 1] == 'P') // if SP
	{
		reg = &cpu.SP;
	}
	else
	{
		res = get_register_ptr(instruction[strlen(instruction) - 1], &reg);
		if (res)
			return;
	}
	(*(short*)reg)++;
	// None of the Condition bits are affected.
}

// Example: "DCX B"		BC <- BC - 1
void handle_DCX(char* instruction, unsigned short extra_data)
{
	int res = 0;
	UINT8* reg = NULL;

	if (instruction[strlen(instruction) - 1] == 'P') // if SP
	{
		reg = &cpu.SP;
	}
	else
	{
		res = get_register_ptr(instruction[strlen(instruction) - 1], &reg);
		if (res)
			return;
	}
	(*(short*)reg)--;
	// None of the Condition bits are affected.
}

// Example: "STAX D"	(DE) <- A
void handle_STAX(char* instruction, unsigned short extra_data)
{
	int res = 0;
	UINT8* reg = NULL;

	res = get_register_ptr(instruction[strlen(instruction) - 1], &reg);
	if (res)
		return;

	(*(short*)reg) = cpu.A;
	// None of the Condition bits are affected.
}

// Example: "LDAX B"	A <- (BC)
void handle_LDAX(char* instruction, unsigned short extra_data)
{
	int res = 0;
	UINT8* reg = NULL;

	res = get_register_ptr(instruction[strlen(instruction) - 1], &reg);
	if (res)
		return;
	
	cpu.A = *(short*)reg;
	// None of the Condition bits are affected.
}

// Example: "CMA"	A <- !A
void handle_CMA(char* instruction, unsigned short extra_data)
{
	cpu.A = ~(cpu.A); // Make the bitwise NOT -> flips all of the bits.
	// None of the Condition bits are affected.
}

// Example: "STC"	CY = 1, (Carry Flag = 1)
void handle_STC(char* instruction, unsigned short extra_data)
{
	set_bit(CARRY, 1, &cpu.Flags);
}

// Example: "CMC"	CY=!CY, (Carry Flag = NOT Carry Flag), bitwise NOT.
void handle_CMC(char* instruction, unsigned short extra_data)
{
	int cur_state = 0;

	cur_state = get_bit_state(CARRY, cpu.Flags);
	if (cur_state) // if current state is 1 so change to 0.
		set_bit(CARRY, 0, &cpu.Flags);
	else
		set_bit(CARRY, 1, &cpu.Flags);
}

// Example: "DAD B"		HL = HL + BC
void handle_DAD(char* instruction, unsigned short extra_data)
{
	int res = 0;
	UINT8* reg = NULL;

	if (instruction[strlen(instruction) - 1] == 'P') // if SP
	{
		reg = &cpu.SP;
	}
	else
	{
		res = get_register_ptr(instruction[strlen(instruction) - 1], &reg);
		if (res)
			return;
	}

	res = *(short*)&cpu.H + *(short*)reg;

	handle_FLAGS(res, *(short*)&cpu.H, *(short*)reg, '+', sizeof(short), -1, -1, -1, -1, CARRY);

	*(short*)&cpu.H = res; // HL += {reg}
}

// Example:  "PCHL"		(PC) <- (HL)	 (Jump to location specified by register H and L)
void handle_PCHL(char* instruction, unsigned short extra_data)
{
	cpu.PC = *(short*)&cpu.H;
	// None of the Condition bits are affected.
}

// Exmaple:  "XCHG"		H <-> D; L <-> E	(Exchange Registers)
void handle_XCHG(char* instruction, unsigned short extra_data)
{
	char temp = 0;

	temp = cpu.H;
	cpu.H = cpu.D;
	cpu.D = temp;

	temp = cpu.L;
	cpu.L = cpu.E;
	cpu.E = temp;
	// None of the Condition bits are affected.
}

// Example:  "XTHL"		L <-> (SP); H <-> (SP + 1)  	(Exchange Stack)
void handle_XTHL(char* instruction, unsigned short extra_data)
{
	char temp = 0;

	temp = *cpu.SP;
	*cpu.SP = cpu.L;
	cpu.L = temp;

	temp = *(cpu.SP + 1);
	*(cpu.SP + 1) = cpu.H;
	cpu.H = temp;
	// None of the Condition bits are affected.
}


// Example:  "MVI L,F4"		The byte of immediate data is stored in the specified register or memory byte.
//							The example loads the L register with 0xF4.
// Special example:  "MVI M,FF"		0xff will be stored in the **memory** location obtained by concatenating the contents of 
//									the H and L registers into a 16 - bit address.
// Moves hard coded data into a specified register or memory address.

void handle_MVI(char* instruction, unsigned short extra_data)
{
	char* token = NULL;
	UINT8* reg = NULL;
	int res = 0;

	token = get_regs_from_instruction(instruction);

	if (token[0] == 'M') // if memory address given.
	{
		reg = (cpu.H << 8) + (cpu.L & 255); // concatenating the values in H and L to represent memory address.
	}
	else
	{
		res = get_register_ptr(token[0], &reg);
		if (res)
			return;
	}

	*reg = (char)extra_data;

	free(token);
	// None of the Condition bits are affected.
}

// Example:  "LXI H,D16"	LXI H,0x103 (0x103 == 259)
// The example above will load the H register with 0x01 and the L register with 0x03:
// the most significant 8 bits of the 16 - bit immediate data is loaded into the first register of the specified pair (reg H in example),
// while the least significant 8 bits of the 16 - bit immediate data is loaded into the second register of the specified pair (reg L in example).
void handle_LXI(char* instruction, unsigned short extra_data)
{
	char* token = NULL;
	UINT8* reg = NULL;
	int res = 0;

	token = get_regs_from_instruction(instruction);

	if (token[0] == 'S') // if SP
	{
		cpu.SP = extra_data;
	}
	else
	{
		res = get_register_ptr(token[0], &reg);
		if (res)
			return;

		*(reg + 1) = extra_data & 255;
		*reg = extra_data >> 8;

		//printf("HL: %d\n", ((*reg) << 8) + *(reg + 1)); // just for debugging
		//printf("H: %d\n", (*reg));
		//printf("L: %d\n", *(reg + 1));
	}

	free(token);
	// None of the Condition bits are affected.
}

// Example:  "ADI D8"	A <- A + byte	(The byte of immediate data is added to the contents of the primary 8-bit accumulator (reg A))
void handle_ADI(char* instruction, unsigned short extra_data)
{
	handle_FLAGS((int)(cpu.A + (char)extra_data), cpu.A, extra_data, '+', sizeof(UINT8), SIGN, ZERO, AUXILIARY_CARRY, PARITY, CARRY);
	cpu.A += (char)extra_data;
}

// Example:  "ACI D8"		A <- A + data + CY	(CY - Carry Flag)
void handle_ACI(char* instruction, unsigned short extra_data)
{
	int res = 0;
	res = cpu.A + (char)extra_data + get_bit_state(CARRY, &cpu.Flags);

	handle_FLAGS(res, cpu.A + (char)extra_data, get_bit_state(CARRY, &cpu.Flags), '+', sizeof(UINT8), SIGN, ZERO, AUXILIARY_CARRY, PARITY, CARRY);
	
	cpu.A = res;
}

// Example:  "SUI D8"		A <- A - data (data is hard coded - 8 bit size)
void handle_SUI(char* instruction, unsigned short extra_data)
{
	// *************       >>  TODO  <<       ***********************
	handle_FLAGS((int)(cpu.A - (char)extra_data), cpu.A, extra_data, '-', sizeof(UINT8), SIGN, ZERO, AUXILIARY_CARRY, PARITY, CARRY);
	cpu.A -= (char)extra_data;
}

// Example:  "SBI D8"		A <- A - data - CY	(CY - Carry Flag)
void handle_SBI(char* instruction, unsigned short extra_data)
{
	handle_FLAGS((int)(cpu.A - ((char)extra_data + get_bit_state(CARRY, &cpu.Flags))), cpu.A, ((char)extra_data + get_bit_state(CARRY, &cpu.Flags)), '-',
		sizeof(UINT8), SIGN, ZERO, AUXILIARY_CARRY, PARITY, CARRY);
	cpu.A -= ((char)extra_data + get_bit_state(CARRY, &cpu.Flags));
}

// Example:  "ANI D8"		A <- A & data
void handle_ANI(char* instruction, unsigned short extra_data)
{
	handle_FLAGS((int)(cpu.A - (char)extra_data), cpu.A, extra_data, '-', sizeof(UINT8), SIGN, ZERO, -1, PARITY, -1);
	set_bit(CARRY, 0, &cpu.Flags); // carry bit is reset to zero.

	cpu.A = cpu.A & (char)extra_data;
}

// Example:  "XRI D8"		A <- A ^ data
void handle_XRI(char* instruction, unsigned short extra_data)
{
	handle_FLAGS((int)(cpu.A ^ (char)extra_data), cpu.A, extra_data, '^', sizeof(UINT8), SIGN, ZERO, -1, PARITY, -1);
	set_bit(CARRY, 0, &cpu.Flags); // carry bit is reset to zero.

	cpu.A = cpu.A ^ (char)extra_data;
}

// Example:  "CPI D8"		A - data	
//  The byte of immediate data is compared to the contents of the accumulator.
//	The comparison is performed by internally subtracting the data from the accumulator, leaving the accumulator unchanged
//	but setting the condition bits (FLAGS) by the result.
//	In particular, the Zero Flag is set (1) if the quantities are equal, and reset (0) if they are unequal.
void handle_CPI(char* instruction, unsigned short extra_data)
{
	// we compare the immediate data to the content of the accumulator (A) by substraction, leaving the accumulator (register A) unchanged.
	handle_FLAGS((int)(cpu.A - (char)extra_data), cpu.A, extra_data, '-', sizeof(UINT8), SIGN, ZERO, AUXILIARY_CARRY, PARITY, CARRY);
}

// Example:  "ORI D8"		A <- A | data
void handle_ORI(char* instruction, unsigned short extra_data)
{
	handle_FLAGS((int)(cpu.A - (char)extra_data), cpu.A, extra_data, '-', sizeof(UINT8), SIGN, ZERO, -1, PARITY, -1);
	set_bit(CARRY, 0, &cpu.Flags); // carry bit is reset to zero.

	cpu.A = cpu.A | (char)extra_data;
}

// Example:  "RST exp"		0 <= exp <= 7		RST 7  ->  CALL $0x38	(exp = 7, call to the 0x38 (7<<3 == 0x38) instruction from the start).
//												PC pushed to the stack and then PC is updated to address ->	0b000exp000
//												exp in the new address is in binary representation.
void handle_RST(char* instruction, unsigned short extra_data)
{
	int new_addr = 0, exp = 0;

	exp = instruction[strlen(instruction) - 1] - '0'; // convert char to int.
	if (exp < 0 || exp > 7)
		return;
	
	new_addr = exp << 3;
	handle_CALL(instruction, new_addr); // make the call instruction for the new address.
	// None of the condition bits are affected.
}


// Stack handles:

/*
	Note: Because we are emulating the stack, we do **reverse mathematical operations** to the Stack Pointer
	when needed to increace or decreace.

	Note: Because we are on 32 bit (x86) our addresses are 4 bytes long so when making CALL and RET we
	make these for 4 bytes addresses, not for 2 bytes.
*/


// Example: "PUSH B"	(sp-2)<-C;  (sp-1)<-B;  sp <- sp - 2
// Special example: "PUSH PSW"	(sp-2)<-flags;  (sp-1)<-A;  sp <- sp - 2
void handle_PUSH(char* instruction, unsigned short extra_data)
{
	int res = 0;
	UINT8* reg = NULL;

	if (instruction[strlen(instruction) - 1] == 'W') // special Push
	{
		*(cpu.SP + 1) = cpu.A;
		*(cpu.SP + 2) = cpu.Flags;
		cpu.SP += 2;
	}
	else
	{
		res = get_register_ptr(instruction[strlen(instruction) - 1], &reg);
		if (res)
			return;

		*(cpu.SP + 1) = *reg; // B
		*(cpu.SP + 2) = *(reg + 1); // C
		cpu.SP += 2; // update stack pointer.
	}
	// None of the condition bits are affected.
}

// Example: "POP H"	L <- (sp);  H <- (sp+1);  sp <- sp+2
// Special example: "POP PSW"	flags <- (sp);  A <- (sp+1);  sp <- sp+2
void handle_POP(char* instruction, unsigned short extra_data)
{
	int res = 0;
	UINT8* reg = NULL;

	if (instruction[strlen(instruction) - 1] == 'W') // special Push
	{
		cpu.A = *(cpu.SP - 1);
		cpu.Flags = *cpu.SP;
		cpu.SP -= 2;
	}
	else
	{
		res = get_register_ptr(instruction[strlen(instruction) - 1], &reg);
		if (res)
			return;

		*reg = *(cpu.SP - 1);
		*(reg + 1) = *cpu.SP;
		cpu.SP -= 2;
	}
	// None of the condition bits are affected, beside if PSW register is specified then they may be changed as seen in the code above, but the Flags do not change for sure.
}

// Example: "CALL adr"	(SP-1) <- PC.hi;  (SP-2) <- PC.lo;  SP <- SP-2;  PC=adr // saving adr as 4 bytes
void handle_CALL(char* instruction, unsigned short extra_data)
{
	printf("saved: %p\n", cpu.PC);
	Push_PC(); // handles the PUSH

	cpu.PC = data + extra_data - 1; 
	// Example: if extra_data = 1 (instruction 1), we need to change PC to start of the data buf (index 0).
	// Pay attention -> mbby this will be needed to change in the Future for real addresses.
	// None of the condition bits are affected.
}

// Example: "RET"	PC.lo <- (sp);  PC.hi <- (sp+1);  SP <- SP+2
void handle_RET(char* instruction, unsigned short extra_data) // getting adr as 4 bytes
{
	cpu.PC = 0; // Just for initialize.
		
	Pop_PC(); // handles the POP.
	// None of the condition bits are affected
}

// Example:  "CZ adr"	if Z, CALL adr		(if Zero Flag (ZF) is not set (0)) // Pay attention!! -> the condition is different from jz and others!
void handle_CZ(char* instruction, unsigned short extra_data)
{
	if (get_bit_state(ZERO, &cpu.Flags) == 0)
		handle_CALL(instruction, extra_data);
	// None of the condition bits are affected.
}

// Example:  "CNZ adr"  if NZ, CALL adr		(If the Zero bit is set (1), a call operation isperformed).
void handle_CNZ(char* instruction, unsigned short extra_data)
{
	if (get_bit_state(ZERO, &cpu.Flags) == 1)
		handle_CALL(instruction, extra_data);
	// None of the condition bits are affected.
}

// Example:  "CC adr"	if CY, CALL adr		(if Carry Flag (CY) is set (1))
void handle_CC(char* instruction, unsigned short extra_data)
{
	if (get_bit_state(CARRY, &cpu.Flags) == 1)
		handle_CALL(instruction, extra_data);
	// None of the condition bits are affected.
}

// Example:  "CNC adr"	if CNY, CALL adr	(if Carry Flag (CY) is zero (0))
void handle_CNC(char* instruction, unsigned short extra_data)
{
	if (get_bit_state(CARRY, &cpu.Flags) == 0)
		handle_CALL(instruction, extra_data);
	// None of the condition bits are affected.
}

// Example:  "CPO adr"		if PO, CALL adr		(if Parity Flag is odd (PF=0))
void handle_CPO(char* instruction, unsigned short extra_data)
{
	if (get_bit_state(PARITY, &cpu.Flags) == 0)
		handle_CALL(instruction, extra_data);
	// None of the condition bits are affected.
}

// Example:  "CPE adr"		if PE, CALL adr		(if parity Flag is even (PF = 1))
void handle_CPE(char* instruction, unsigned short extra_data)
{
	if (get_bit_state(PARITY, &cpu.Flags) == 1)
		handle_CALL(instruction, extra_data);
	// None of the condition bits are affected.
}

// Example:  "CP adr"		if P, CALL adr		(call if plus)		
void handle_CP(char* instruction, unsigned short extra_data)
{
	if (get_bit_state(SIGN, &cpu.Flags) == 0)
		handle_CALL(instruction, extra_data);
	// None of the condition bits are affected.
}

// Example:  "CM adr"		if M, CALL adr		(call if minus)
void handle_CM(char* instruction, unsigned short extra_data)
{
	if (get_bit_state(SIGN, &cpu.Flags) == 1)
		handle_CALL(instruction, extra_data);
	// None of the condition bits are affected.
}

// Example:  "RNC"			if CY == 0, RET		(if carry bit is zero, return)
void handle_RNC(char* instruction, unsigned short extra_data)
{
	if (get_bit_state(CARRY, &cpu.Flags) == 0)
		handle_RET(instruction, extra_data);
	// None of the condition bits are affected.
}

// Example:  "RC"			if CY == 1, RET		(if carry bit is set (1), return)
void handle_RC(char* instruction, unsigned short extra_data)
{
	if (get_bit_state(CARRY, &cpu.Flags) == 1)
		handle_RET(instruction, extra_data);
	// None of the condition bits are affected.
}

// Example:  "RZ"			if ZF == 1, RET		(if Zero Flag is set (1), return)
void handle_RZ(char* instruction, unsigned short extra_data)
{
	if (get_bit_state(ZERO, &cpu.Flags) == 1)
		handle_RET(instruction, extra_data);
	// None of the condition bits are affected.
}

// Example:  "RNZ"			if ZF == 0, RET		(if Zero Flag is zero, return)
void handle_RNZ(char* instruction, unsigned short extra_data)
{
	if (get_bit_state(ZERO, &cpu.Flags) == 0)
		handle_RET(instruction, extra_data);
	// None of the condition bits are affected.
}

// Example:  "RM"			if M, RET		(if minus (Sign Flag is set (1)), return)
void handle_RM(char* instruction, unsigned short extra_data)
{
	if (get_bit_state(SIGN, &cpu.Flags) == 1)
		handle_RET(instruction, extra_data);
	// None of the condition bits are affected.
}

// Example:  "RP"			if P, RET		(if positive (Sign Flag is zero (0)), return)
void handle_RP(char* instruction, unsigned short extra_data)
{
	if (get_bit_state(SIGN, &cpu.Flags) == 0)
		handle_RET(instruction, extra_data);
	// None of the condition bits are affected.
}

// Example:  "RPE"			if PE, RET		(if Parity Flag is even (1), return)
void handle_RPE(char* instruction, unsigned short extra_data)
{
	if (get_bit_state(PARITY, &cpu.Flags) == 1)
		handle_RET(instruction, extra_data);
	// None of the condition bits are affected.
}

// Example:  "RPO"			if PO, RET		(if Parity Flag is odd (0), return)
void handle_RPO(char* instruction, unsigned short extra_data)
{
	if (get_bit_state(PARITY, &cpu.Flags) == 0)
		handle_RET(instruction, extra_data);
	// None of the condition bits are affected.
}

// Example:  "SPHL"			SP <- HL	(The 16 bits of data held in the H and L registers replace the contents of the stack pointer SP)
void handle_SPHL(char* instruction, unsigned short extra_data)
{
	cpu.SP = *(short*)&cpu.H;
	// None of the condition bits are affected.
}

// Example:  "SHLD"		(adr) <- L; (adr + 1) <- H		(Store H and L Direct)
// The contents of the L register are stored at the memory address formed by concatenating HI ADDR with LOW ADDR (the addr given).
// The contents of the H register are stored at the next higher memory address.
void handle_SHLD(char* instruction, unsigned short extra_data)
{
	char* addr = (char*)extra_data;

	*addr = cpu.L;
	*(addr + 1) = cpu.H;
	// None of the condition bits are affected.
}

// Example: "LHLD"		L <- (adr); H <- (adr + 1)		(Load H and L Direct)
void handle_LHLD(char* instruction, unsigned short extra_data)
{
	char* addr = (char*)extra_data;

	cpu.L = *addr;
	cpu.H = *(addr + 1);
	// None of the condition bits are affected.
}

// Example:  "STA"		(adr) <- A	(Store Accumulator Direct)
// The contents of the accumulator (reg A) replace the byte at the memory address formed by concatenating HI ADDR with LOW ADDR.
void handle_STA(char* instruction, unsigned short extra_data)
{
	char* addr = (char*)extra_data;
	*addr = cpu.A;
	// None of the condition bits are affected.
}

// Example:  "LDA"		A <- (adr)	(Load Accumulator Direct)
void handle_LDA(char* instruction, unsigned short extra_data)
{
	char* addr = (char*)extra_data;
	cpu.A = *addr;
	// None of the condition bits are affected.
}
