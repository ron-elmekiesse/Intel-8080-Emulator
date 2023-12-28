#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "disassemble.h"
#include "make_instruction.h"
#include "utils.h"

OPCODE OPCODES_TABLE[] = {
{0x00,	"NOP"		,1	,NO_JUMP	,handle_NOP } ,
{0x01,	"LXI B,D16" ,3	,NO_JUMP	,handle_LXI	} ,
{0x02,	"STAX B"	,1	,NO_JUMP	,handle_STAX} ,
{0x03,	"INX B"		,1	,NO_JUMP	,handle_INX } ,
{0x04,	"INR B"		,1	,NO_JUMP	,handle_INR } ,
{0x05,	"DCR B"		,1	,NO_JUMP	,handle_DCR } ,
{0x06,	"MVI B,D8"	,2	,NO_JUMP	,handle_MVI } ,
{0x07,	"RLC"		,1	,NO_JUMP	,handle_RLC	} ,
{0x08,  "-"			,1	,NO_JUMP	,NULL		} ,
{0x09,	"DAD B"		,1	,NO_JUMP	,handle_DAD } ,
{0x0a,	"LDAX B"	,1	,NO_JUMP	,handle_LDAX} ,
{0x0b,	"DCX B"		,1	,NO_JUMP	,handle_DCX } ,
{0x0c,	"INR C"		,1	,NO_JUMP	,handle_INR	} ,
{0x0d,	"DCR C"		,1	,NO_JUMP	,handle_DCR	} ,
{0x0e,	"MVI C,D8"	,2	,NO_JUMP	,handle_MVI } ,
{0x0f,	"RRC"		,1	,NO_JUMP	,handle_RRC	} ,
{0x10,  "-"			,1	,NO_JUMP	,NULL		} ,
{0x11,	"LXI D,D16" ,3	,NO_JUMP	,handle_LXI	} ,
{0x12,	"STAX D"	,1	,NO_JUMP	,handle_STAX} ,
{0x13,	"INX D"		,1	,NO_JUMP	,handle_INX } ,
{0x14,	"INR D"		,1	,NO_JUMP	,handle_INR	} ,
{0x15,	"DCR D"		,1	,NO_JUMP	,handle_DCR	} ,
{0x16,	"MVI D,D8"	,2	,NO_JUMP	,handle_MVI } ,
{0x17,	"RAL"		,1	,NO_JUMP	,handle_RAL	} ,
{0x18,  "-"			,1	,NO_JUMP	,NULL		} ,
{0x19,	"DAD D"		,1	,NO_JUMP	,handle_DAD } ,
{0x1a,	"LDAX D"	,1	,NO_JUMP	,handle_LDAX} ,
{0x1b,	"DCX D"		,1	,NO_JUMP	,handle_DCX } ,
{0x1c,	"INR E"		,1	,NO_JUMP	,handle_INR	} ,
{0x1d,	"DCR E"		,1	,NO_JUMP	,handle_DCR	} ,
{0x1e,	"MVI E,D8"  ,2	,NO_JUMP	,handle_MVI } ,
{0x1f,	"RAR"		,1	,NO_JUMP	,handle_RAR	} ,
{0x20,	"-"			,1	,NO_JUMP	,NULL		} ,
{0x21,	"LXI H,D16"	,3	,NO_JUMP	,handle_LXI	} ,
{0x22,	"SHLD adr"	,3	,NO_JUMP	,handle_SHLD} ,
{0x23,	"INX H"		,1	,NO_JUMP	,handle_INX } ,
{0x24,	"INR H"		,1	,NO_JUMP	,handle_INR	} ,
{0x25,	"DCR H"		,1	,NO_JUMP	,handle_DCR	} ,
{0x26,	"MVI H,D8"  ,2	,NO_JUMP	,handle_MVI } ,
{0x27,	"DAA"		,1	,NO_JUMP	,NULL		} ,
{0x28,  "-"			,1	,NO_JUMP	,NULL		} ,
{0x29,	"DAD H"		,1	,NO_JUMP	,handle_DAD } ,
{0x2a,	"LHLD adr"	,3	,NO_JUMP	,handle_LHLD} ,
{0x2b,	"DCX H"		,1	,NO_JUMP	,handle_DCX } ,
{0x2c,	"INR L"		,1	,NO_JUMP	,handle_INR	} ,
{0x2d,	"DCR L"		,1	,NO_JUMP	,handle_DCR	} ,
{0x2e,	"MVI L,D8"	,2	,NO_JUMP	,handle_MVI } ,
{0x2f,	"CMA"		,1	,NO_JUMP	,handle_CMA } ,
{0x30,  "-"			,1	,NO_JUMP	,NULL		} ,
{0x31,	"LXI SP,D16",3	,NO_JUMP	,handle_LXI	} ,
{0x32,	"STA adr"	,3	,NO_JUMP	,handle_STA	} ,
{0x33,	"INX SP"	,1	,NO_JUMP	,handle_INX } ,
{0x34,	"INR M"		,1	,NO_JUMP	,handle_INR	} ,
{0x35,	"DCR M"		,1	,NO_JUMP	,handle_DCR	} ,
{0x36,	"MVI M,D8"	,2	,NO_JUMP	,handle_MVI } ,
{0x37,	"STC"		,1	,NO_JUMP	,handle_STC } ,
{0x38,  "-"			,1	,NO_JUMP	,NULL		} ,
{0x39,	"DAD SP"	,1	,NO_JUMP	,handle_DAD } ,
{0x3a,	"LDA adr"	,3	,NO_JUMP	,handle_LDA	} ,
{0x3b,	"DCX SP"	,1	,NO_JUMP	,handle_DCX } ,
{0x3c,	"INR A"		,1	,NO_JUMP	,handle_INR	} ,
{0x3d,	"DCR A,"	,1	,NO_JUMP	,handle_DCR	} ,
{0x3e,	"MVI A,D8"	,2	,NO_JUMP	,handle_MVI } ,
{0x3f,	"CMC"		,1	,NO_JUMP	,handle_CMC } ,
{0x40,	"MOV B,B"	,1	,NO_JUMP	,handle_MOV } ,
{0x41,	"MOV B,C"	,1	,NO_JUMP	,handle_MOV } ,
{0x42,	"MOV B,D"	,1	,NO_JUMP	,handle_MOV } ,
{0x43,	"MOV B,E"	,1	,NO_JUMP	,handle_MOV } ,
{0x44,	"MOV B,H"	,1	,NO_JUMP	,handle_MOV } ,
{0x45,	"MOV B,L"	,1	,NO_JUMP	,handle_MOV } ,
{0x46,	"MOV B,M"	,1	,NO_JUMP	,handle_MOV } ,
{0x47,	"MOV B,A"	,1	,NO_JUMP	,handle_MOV } ,
{0x48,	"MOV C,B"	,1	,NO_JUMP	,handle_MOV } ,
{0x49,	"MOV C,C"	,1	,NO_JUMP	,handle_MOV } ,
{0x4a,	"MOV C,D"	,1	,NO_JUMP	,handle_MOV } ,
{0x4b,	"MOV C,E"	,1	,NO_JUMP	,handle_MOV } ,
{0x4c,	"MOV C,H"	,1	,NO_JUMP	,handle_MOV } ,
{0x4d,	"MOV C,L"	,1	,NO_JUMP	,handle_MOV } ,
{0x4e,	"MOV C,M"	,1	,NO_JUMP	,handle_MOV } ,
{0x4f,	"MOV C,A"	,1	,NO_JUMP	,handle_MOV } ,
{0x50,	"MOV D,B"	,1	,NO_JUMP	,handle_MOV } ,
{0x51,	"MOV D,C"	,1	,NO_JUMP	,handle_MOV } ,
{0x52,	"MOV D,D"	,1	,NO_JUMP	,handle_MOV } ,
{0x53,	"MOV D,E"	,1	,NO_JUMP	,handle_MOV } ,
{0x54,	"MOV D,H"	,1	,NO_JUMP	,handle_MOV } ,
{0x55,	"MOV D,L"	,1	,NO_JUMP	,handle_MOV } ,
{0x56,	"MOV D,M"	,1	,NO_JUMP	,handle_MOV } ,
{0x57,	"MOV D,A"	,1	,NO_JUMP	,handle_MOV } ,
{0x58,	"MOV E,B"	,1	,NO_JUMP	,handle_MOV } ,
{0x59,	"MOV E,C"	,1	,NO_JUMP	,handle_MOV } ,
{0x5a,	"MOV E,D"	,1	,NO_JUMP	,handle_MOV } ,
{0x5b,	"MOV E,E"	,1	,NO_JUMP	,handle_MOV } ,
{0x5c,	"MOV E,H"	,1	,NO_JUMP	,handle_MOV } ,
{0x5d,	"MOV E,L"	,1	,NO_JUMP	,handle_MOV } ,
{0x5e,	"MOV E,M"	,1	,NO_JUMP	,handle_MOV } ,
{0x5f,	"MOV E,A"	,1	,NO_JUMP	,handle_MOV } ,
{0x60,	"MOV H,B"	,1	,NO_JUMP	,handle_MOV } ,
{0x61,	"MOV H,C"	,1	,NO_JUMP	,handle_MOV } ,
{0x62,	"MOV H,D"	,1	,NO_JUMP	,handle_MOV } ,
{0x63,	"MOV H,E"	,1	,NO_JUMP	,handle_MOV } ,
{0x64,	"MOV H,H"	,1	,NO_JUMP	,handle_MOV } ,
{0x65,	"MOV H,L"	,1	,NO_JUMP	,handle_MOV } ,
{0x66,	"MOV H,M"	,1	,NO_JUMP	,handle_MOV } ,
{0x67,	"MOV H,A"	,1	,NO_JUMP	,handle_MOV } ,
{0x68,	"MOV L,B"	,1	,NO_JUMP	,handle_MOV } ,
{0x69,	"MOV L,C"	,1	,NO_JUMP	,handle_MOV } ,
{0x6a,	"MOV L,D"	,1	,NO_JUMP	,handle_MOV } ,
{0x6b,	"MOV L,E"	,1	,NO_JUMP	,handle_MOV } ,
{0x6c,	"MOV L,H"	,1	,NO_JUMP	,handle_MOV } ,
{0x6d,	"MOV L,L"	,1	,NO_JUMP	,handle_MOV } ,
{0x6e,	"MOV L,M"	,1	,NO_JUMP	,handle_MOV } ,
{0x6f,	"MOV L,A"	,1	,NO_JUMP	,handle_MOV } ,
{0x70,	"MOV M,B"	,1	,NO_JUMP	,handle_MOV } ,
{0x71,	"MOV M,C"	,1	,NO_JUMP	,handle_MOV } ,
{0x72,	"MOV M,D"	,1	,NO_JUMP	,handle_MOV } ,
{0x73,	"MOV M,E"	,1	,NO_JUMP	,handle_MOV } ,
{0x74,	"MOV M,H"	,1	,NO_JUMP	,handle_MOV } ,
{0x75,	"MOV M,L"	,1	,NO_JUMP	,handle_MOV } ,
{0x76,	"HLT"		,1	,NO_JUMP	,NULL		} ,
{0x77,	"MOV M,A"	,1	,NO_JUMP	,handle_MOV } ,
{0x78,	"MOV A,B"	,1	,NO_JUMP	,handle_MOV } ,
{0x79,	"MOV A,C"	,1	,NO_JUMP	,handle_MOV } ,
{0x7a,	"MOV A,D"	,1	,NO_JUMP	,handle_MOV } ,
{0x7b,	"MOV A,E"	,1	,NO_JUMP	,handle_MOV } ,
{0x7c,	"MOV A,H"	,1	,NO_JUMP	,handle_MOV } ,
{0x7d,	"MOV A,L"	,1	,NO_JUMP	,handle_MOV } ,
{0x7e,	"MOV A,M"	,1	,NO_JUMP	,handle_MOV } ,
{0x7f,	"MOV A,A"	,1	,NO_JUMP	,handle_MOV } ,
{0x80,	"ADD B"		,1	,NO_JUMP	,handle_ADD } ,
{0x81,	"ADD C"		,1	,NO_JUMP	,handle_ADD } ,
{0x82,	"ADD D"		,1	,NO_JUMP	,handle_ADD } ,
{0x83,	"ADD E"		,1	,NO_JUMP	,handle_ADD } ,
{0x84,	"ADD H"		,1	,NO_JUMP	,handle_ADD } ,
{0x85,	"ADD L"		,1	,NO_JUMP	,handle_ADD } ,
{0x86,	"ADD M"		,1	,NO_JUMP	,handle_ADD } ,
{0x87,	"ADD A"		,1	,NO_JUMP	,handle_ADD } ,
{0x88,	"ADC B"		,1	,NO_JUMP	,handle_ADC } ,
{0x89,	"ADC C"		,1	,NO_JUMP	,handle_ADC } ,
{0x8a,	"ADC D"		,1	,NO_JUMP	,handle_ADC } ,
{0x8b,	"ADC E"		,1	,NO_JUMP	,handle_ADC } ,
{0x8c,	"ADC H"		,1	,NO_JUMP	,handle_ADC } ,
{0x8d,	"ADC L"		,1	,NO_JUMP	,handle_ADC } ,
{0x8e,	"ADC M"		,1	,NO_JUMP	,handle_ADC } ,
{0x8f,	"ADC A"		,1	,NO_JUMP	,handle_ADC } ,
{0x90,	"SUB B"		,1	,NO_JUMP	,handle_SUB } ,
{0x91,	"SUB C"		,1	,NO_JUMP	,handle_SUB } ,
{0x92,	"SUB D"		,1	,NO_JUMP	,handle_SUB } ,
{0x93,	"SUB E"		,1	,NO_JUMP	,handle_SUB } ,
{0x94,	"SUB H"		,1	,NO_JUMP	,handle_SUB } ,
{0x95,	"SUB L"		,1	,NO_JUMP	,handle_SUB } ,
{0x96,	"SUB M"		,1	,NO_JUMP	,handle_SUB } ,
{0x97,	"SUB A"		,1	,NO_JUMP	,handle_SUB } ,
{0x98,	"SBB B"		,1	,NO_JUMP	,handle_SBB } ,
{0x99,	"SBB C"		,1	,NO_JUMP	,handle_SBB } ,
{0x9a,	"SBB D"		,1	,NO_JUMP	,handle_SBB } ,
{0x9b,	"SBB E"		,1	,NO_JUMP	,handle_SBB } ,
{0x9c,	"SBB H"		,1	,NO_JUMP	,handle_SBB } ,
{0x9d,	"SBB L"		,1	,NO_JUMP	,handle_SBB } ,
{0x9e,	"SBB M"		,1	,NO_JUMP	,handle_SBB } ,
{0x9f,	"SBB A"		,1	,NO_JUMP	,handle_SBB } ,
{0xa0,	"ANA B"		,1	,NO_JUMP	,handle_ANA } ,
{0xa1,	"ANA C"		,1	,NO_JUMP	,handle_ANA } ,
{0xa2,	"ANA D"		,1	,NO_JUMP	,handle_ANA } ,
{0xa3,	"ANA E"		,1	,NO_JUMP	,handle_ANA } ,
{0xa4,	"ANA H"		,1	,NO_JUMP	,handle_ANA } ,
{0xa5,	"ANA L"		,1	,NO_JUMP	,handle_ANA } ,
{0xa6,	"ANA M"		,1	,NO_JUMP	,handle_ANA } ,
{0xa7,	"ANA A"		,1	,NO_JUMP	,handle_ANA } ,
{0xa8,	"XRA B"		,1	,NO_JUMP	,handle_XRA } ,
{0xa9,	"XRA C"		,1	,NO_JUMP	,handle_XRA } ,
{0xaa,	"XRA D"		,1	,NO_JUMP	,handle_XRA } ,
{0xab,	"XRA E"		,1	,NO_JUMP	,handle_XRA } ,
{0xac,	"XRA H"		,1	,NO_JUMP	,handle_XRA } ,
{0xad,	"XRA L"		,1	,NO_JUMP	,handle_XRA } ,
{0xae,	"XRA M"		,1	,NO_JUMP	,handle_XRA } ,
{0xaf,	"XRA A"		,1	,NO_JUMP	,handle_XRA } ,
{0xb0,	"ORA B"		,1	,NO_JUMP	,handle_ORA } ,
{0xb1,	"ORA C"		,1	,NO_JUMP	,handle_ORA } ,
{0xb2,	"ORA D"		,1	,NO_JUMP	,handle_ORA } ,
{0xb3,	"ORA E"		,1	,NO_JUMP	,handle_ORA } ,
{0xb4,	"ORA H"		,1	,NO_JUMP	,handle_ORA } ,
{0xb5,	"ORA L"		,1	,NO_JUMP	,handle_ORA } ,
{0xb6,	"ORA M"		,1	,NO_JUMP	,handle_ORA } ,
{0xb7,	"ORA A"		,1	,NO_JUMP	,handle_ORA } ,
{0xb8,	"CMP B"		,1	,NO_JUMP	,handle_CMP } ,
{0xb9,	"CMP C"		,1	,NO_JUMP	,handle_CMP } ,
{0xba,	"CMP D"		,1	,NO_JUMP	,handle_CMP } ,
{0xbb,	"CMP E"		,1	,NO_JUMP	,handle_CMP } ,
{0xbc,	"CMP H"		,1	,NO_JUMP	,handle_CMP } ,
{0xbd,	"CMP L"		,1	,NO_JUMP	,handle_CMP } ,
{0xbe,	"CMP M"		,1	,NO_JUMP	,handle_CMP } ,
{0xbf,	"CMP A"		,1	,NO_JUMP	,handle_CMP } ,
{0xc0,	"RNZ"		,1	,NO_JUMP	,handle_RNZ } ,
{0xc1,	"POP B"		,1	,NO_JUMP	,handle_POP } ,
{0xc2,	"JNZ adr"	,3	,JUMPS		,handle_JNZ },
{ 0xc3,	"JMP adr"	,3	,JUMPS		,handle_JMP },
{ 0xc4,	"CNZ adr"	,3	,JUMPS		,handle_CNZ },
{ 0xc5,	"PUSH B"	,1	,NO_JUMP	,handle_PUSH },
{ 0xc6,	"ADI D8"	,2	,NO_JUMP	,handle_ADI },
{ 0xc7,	"RST 0"		,1	,JUMPS		,handle_RST },
{ 0xc8,	"RZ"		,1	,NO_JUMP	,handle_RZ },
{ 0xc9,	"RET"		,1	,NO_JUMP	,handle_RET },
{ 0xca,	"JZ adr"	,3	,JUMPS		,handle_JZ },
{ 0xcb,  "-"		,1	,NO_JUMP	,NULL },
{ 0xcc,	"CZ adr"	,3	,JUMPS		,handle_CZ },
{ 0xcd,	"CALL adr"  ,3	,JUMPS		,handle_CALL },
{ 0xce,	"ACI D8	"	,2	,NO_JUMP	,handle_ACI },
{ 0xcf,	"RST 1"		,1	,JUMPS		,handle_RST },
{ 0xd0,	"RNC"		,1	,NO_JUMP	,handle_RNC },
{ 0xd1,	"POP D"		,1	,NO_JUMP	,handle_POP },
{ 0xd2,	"JNC adr"	,3	,JUMPS		,handle_JNC },
{ 0xd3,	"OUT D8"	,2	,NO_JUMP	,NULL },
{ 0xd4,	"CNC adr"	,3	,JUMPS		,handle_CNC },
{ 0xd5,	"PUSH D"	,1	,NO_JUMP	,handle_PUSH },
{ 0xd6,	"SUI D8"	,2	,NO_JUMP	,handle_SUI },
{ 0xd7,	"RST 2"		,1	,JUMPS		,handle_RST },
{ 0xd8,	"RC"		,1	,NO_JUMP	,handle_RC },
{ 0xd9,	"-"			,1	,NO_JUMP	,NULL },
{ 0xda,	"JC adr"	,3	,JUMPS		,handle_JC },
{ 0xdb,	"IN D8"		,2	,NO_JUMP	,NULL },
{ 0xdc,	"CC adr"	,3	,JUMPS		,handle_CC },
{ 0xdd,  "-"		,1	,NO_JUMP	,NULL },
{ 0xde,	"SBI D8"	,2	,NO_JUMP	,handle_SBI },
{ 0xdf,	"RST 3"		,1	,JUMPS		,handle_RST },
{ 0xe0,	"RPO"		,1	,NO_JUMP	,handle_RPO },
{ 0xe1,	"POP H"		,1	,NO_JUMP	,handle_POP },
{ 0xe2,	"JPO adr"	,3	,JUMPS		,handle_JPO },
{ 0xe3,	"XTHL"		,1	,NO_JUMP	,handle_XTHL },
{ 0xe4,	"CPO adr"	,3	,JUMPS		,handle_CPO },
{ 0xe5,	"PUSH H"	,1	,NO_JUMP	,handle_PUSH },
{ 0xe6,	"ANI D8"	,2	,NO_JUMP	,handle_ANI },
{ 0xe7,	"RST 4"		,1	,JUMPS		,handle_RST },
{ 0xe8,	"RPE"		,1	,NO_JUMP	,handle_RPE },
{ 0xe9,	"PCHL"		,1	,JUMPS		,handle_PCHL },
{ 0xea,	"JPE adr"	,3	,JUMPS		,handle_JPE },
{ 0xeb,	"XCHG"		,1	,NO_JUMP	,handle_XCHG },
{ 0xec,	"CPE adr"	,3	,JUMPS		,handle_CPE },
{ 0xed,  "-"		,1	,NO_JUMP	,NULL },
{ 0xee,	"XRI D8"	,2	,NO_JUMP	,handle_XRI },
{ 0xef,	"RST 5"		,1	,JUMPS		,handle_RST },
{ 0xf0,	"RP"		,1	,NO_JUMP	,handle_RP },
{ 0xf1,	"POP PSW"	,1	,NO_JUMP	,handle_POP },
{ 0xf2,	"JP adr"	,3	,JUMPS		,handle_JP },
{ 0xf3,	"DI"		,1	,NO_JUMP	,NULL },
{ 0xf4,	"CP adr"	,3	,JUMPS		,handle_CP },
{ 0xf5,	"PUSH PSW"	,1	,NO_JUMP	,handle_PUSH },
{ 0xf6,	"ORI D8"	,2	,NO_JUMP	,handle_ORI },
{ 0xf7,	"RST 6"		,1	,JUMPS		,handle_RST },
{ 0xf8,	"RM	"		,1	,NO_JUMP	,handle_RM },
{ 0xf9,	"SPHL"		,1	,NO_JUMP	,handle_SPHL },
{ 0xfa,	"JM adr"	,3	,JUMPS		,handle_JM },
{ 0xfb,	"EI	"		,1	,NO_JUMP	,NULL },
{ 0xfc,	"CM adr"	,3	,JUMPS		,handle_CM },
{ 0xfd,  "-"		,1	,NO_JUMP	,NULL },
{ 0xfe,	"CPI D8"	,2	,NO_JUMP	,handle_CPI },
{ 0xff,	"RST 7"		,1	,JUMPS		,handle_RST }
};


void disass_and_handle(FILE* fd, unsigned int file_size)
{
	int k = 0, res = 0, p = 0;
	BYTE extra_data[MAX_EXTRA_DATA] = { 0 };
	unsigned short temp_data = 0;

	//
	
	// TODO:

	/*handle_MVI("MVI A,D8", 127);
	handle_MVI("MVI L,D8", 127);
	handle_MVI("MVI H,D8", 127);
	handle_ADD("ADD L", 0);
	handle_ADD("ADD H", 0);*/
	 /*
	handle_MVI("MVI A,D8", 15);
	handle_MVI("MVI L,D8", 30);
	handle_SUB("SUB L", 0);*/

	//

	data = (BYTE*)malloc(file_size); // allocate memory for the binary intel 8080 data. data is extern.
	if (!data) return;

	res = fread(data, sizeof(BYTE), file_size, fd);
	if (!data || !res)
		return;

	cpu.PC = data; // Set EIP / PC to the first instruction.

	while (cpu.PC != (data + file_size))
	{
		temp_data = 0;
		for (k = 0; k < sizeof(OPCODES_TABLE) / sizeof(OPCODE); k++)
		{
			if (*cpu.PC == OPCODES_TABLE[k].opcode)
			{
				printf("%d:    %#02x %s ", cpu.PC - data + 1, *cpu.PC, OPCODES_TABLE[k].instruction);

				// if there is extra data.
				if (OPCODES_TABLE[k].size > 1)
				{
					for (p = 0; p < OPCODES_TABLE[k].size - 1 && cpu.PC != (data + file_size); p++)
					{
						if (p > MAX_EXTRA_DATA)
						{
							printf("Too much extra data!!\n");
							exit(0);
						}
						cpu.PC++;
						extra_data[p] = *cpu.PC;
					}

					// save and print extra data.
					memcpy(&temp_data, extra_data, MAX_EXTRA_DATA);
					//temp_data = BIG_TO_LITTLE_ENDIAN(temp_data); // mbby the address is as LITTLE endian. ->> this convert is not needed.
					printf("%d", temp_data);
				}
				puts("");
				
				if (OPCODES_TABLE[k].func)
				{
					OPCODES_TABLE[k].func(OPCODES_TABLE[k].instruction, temp_data);
				}
				else
					printf("'%s' is not supported yet!\n", OPCODES_TABLE[k].instruction);
				
				if (cpu.PC == 0) // if RET is to exit the program.
					goto cleanup;

				if (OPCODES_TABLE[k].needs_to_jump == NO_JUMP) // if we didn't jump to a new address so INC the EIP.
					cpu.PC++; // INC EIP / PC

				printf("cpu.PC: %p\n", cpu.PC);

				break; // from the second loop.
			}
		}
	}
cleanup:
	free(data);
}