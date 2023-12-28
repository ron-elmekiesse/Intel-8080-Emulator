#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
	************* TODO: Remember to update Flags register after specified instructions!!!!! ***************
	
	** Know how to indicate when a register has an unsigned value or signed!!!!! **
*/

void handle_NOP(char* instruction, unsigned short extra_data);
void handle_MOV(char* instruction, unsigned short extra_data);
void handle_ADD(char* instruction, unsigned short extra_data);
void handle_SUB(char* instruction, unsigned short extra_data);
void handle_ADC(char* instruction, unsigned short extra_data);
void handle_SBB(char* instruction, unsigned short extra_data);
void handle_ANA(char* instruction, unsigned short extra_data);
void handle_XRA(char* instruction, unsigned short extra_data);
void handle_ORA(char* instruction, unsigned short extra_data);
void handle_CMP(char* instruction, unsigned short extra_data);
void handle_JNZ(char* instruction, unsigned short extra_data);
void handle_JMP(char* instruction, unsigned short extra_data);
void handle_JZ(char* instruction, unsigned short extra_data);
void handle_JNC(char* instruction, unsigned short extra_data);
void handle_JC(char* instruction, unsigned short extra_data);
void handle_JPE(char* instruction, unsigned short extra_data);
void handle_JPO(char* instruction, unsigned short extra_data);
void handle_JM(char* instruction, unsigned short extra_data);
void handle_JP(char* instruction, unsigned short extra_data);
void handle_INR(char* instruction, unsigned short extra_data);
void handle_DCR(char* instruction, unsigned short extra_data);
void handle_RLC(char* instruction, unsigned short extra_data);
void handle_RRC(char* instruction, unsigned short extra_data);
void handle_RAL(char* instruction, unsigned short extra_data);
void handle_RAR(char* instruction, unsigned short extra_data);
void handle_INX(char* instruction, unsigned short extra_data);
void handle_DCX(char* instruction, unsigned short extra_data);
void handle_STAX(char* instruction, unsigned short extra_data);
void handle_LDAX(char* instruction, unsigned short extra_data);
void handle_CMA(char* instruction, unsigned short extra_data);
void handle_STC(char* instruction, unsigned short extra_data);
void handle_CMC(char* instruction, unsigned short extra_data);
void handle_DAD(char* instruction, unsigned short extra_data);
void handle_PCHL(char* instruction, unsigned short extra_data);
void handle_XCHG(char* instruction, unsigned short extra_data);
void handle_XTHL(char* instruction, unsigned short extra_data);


// Immediate data (hard coded data) instruction handles:
void handle_MVI(char* instruction, unsigned short extra_data);
void handle_LXI(char* instruction, unsigned short extra_data);
void handle_ADI(char* instruction, unsigned short extra_data);
void handle_ACI(char* instruction, unsigned short extra_data);
void handle_SUI(char* instruction, unsigned short extra_data);
void handle_SBI(char* instruction, unsigned short extra_data);
void handle_ANI(char* instruction, unsigned short extra_data);
void handle_XRI(char* instruction, unsigned short extra_data);
void handle_CPI(char* instruction, unsigned short extra_data);
void handle_ORI(char* instruction, unsigned short extra_data);
void handle_RST(char* instruction, unsigned short extra_data);



// Stack handles:
/*
	Note: Because we are emulating the stack, we do **reverse mathematical operations** to the Stack Pointer
	when needed to increace or decreace.

	Note: Because we are on 32 bit (x86) our addresses are 4 bytes size so when making CALL and RET we
	make these for 4 bytes addresses, not for 2 bytes.
*/
void handle_PUSH(char* instruction, unsigned short extra_data);
void handle_POP(char* instruction, unsigned short extra_data);
void handle_CALL(char* instruction, unsigned short extra_data);
void handle_RET(char* instruction, unsigned short extra_data);
void handle_CZ(char* instruction, unsigned short extra_data);
void handle_CNZ(char* instruction, unsigned short extra_data);
void handle_CC(char* instruction, unsigned short extra_data);
void handle_CNC(char* instruction, unsigned short extra_data);
void handle_CPO(char* instruction, unsigned short extra_data);
void handle_CPE(char* instruction, unsigned short extra_data);
void handle_CP(char* instruction, unsigned short extra_data);
void handle_CM(char* instruction, unsigned short extra_data);
void handle_RNC(char* instruction, unsigned short extra_data);
void handle_RC(char* instruction, unsigned short extra_data);
void handle_RZ(char* instruction, unsigned short extra_data);
void handle_RNZ(char* instruction, unsigned short extra_data);
void handle_RM(char* instruction, unsigned short extra_data);
void handle_RP(char* instruction, unsigned short extra_data);
void handle_RPE(char* instruction, unsigned short extra_data);
void handle_RPO(char* instruction, unsigned short extra_data);
void handle_SPHL(char* instruction, unsigned short extra_data);
void handle_SHLD(char* instruction, unsigned short extra_data);
void handle_LHLD(char* instruction, unsigned short extra_data);
void handle_STA(char* instruction, unsigned short extra_data);
void handle_LDA(char* instruction, unsigned short extra_data);
