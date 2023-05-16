#pragma once

#include <main.h>

class FumMips {

public:

	enum OpCode {
		RTYPE = 0,
		J = 0b000010,
		BEQ = 0b000100,
		BNE = 0b000101,
		ADDI = 0b001000,
		SLTI = 0b001010,
		ANDI = 0b001100,
		ORI = 0b001101,
		LW = 0b100011,
		SW = 0b101011
	};

	enum Funct {
		ADD = 0b100000,
		SUB = 0b100010,
		AND = 0b100100,
		OR = 0b100101,
		SLT = 0b101010
	};

	struct RType {
		OpCode opcode;
		int rs;
		int rt;
		int rd;
		int shamt;
		Funct funct;
	};

	struct IType {
		OpCode opcode;
		int rs;
		int rt;
		int immediate;
	};

	struct JType {
		OpCode opcode;
		int immediate;
	};

	union Instruction {
		RType r;
		IType i;
		JType j;
	};

	enum class AluOp {
		AND,
		OR,
		ADD,
		SUB,
		SLT
	};

	enum class Op {
		ADD,
		SUB,
		OR,
		AND,
		SLT,
		J,
		SW,
		LW,
		ADDI,
		SLTI,
		ANDI,
		ORI,
		BEQ,
		BNE,
	};

	static const int MEMORY_SIZE = 100;
	static const int REGISTER_SIZE = 32;
	static const int INSTRUCTION_MEMORY_SIZR = 500;
	
	FumMips(string codePath);
	void init(string codePath);
	void next_clock();
	void show_register_file();
	void show_memory(int size);

private:

	int virtualMemory[MEMORY_SIZE];
	int virtualRegisterFile[REGISTER_SIZE];
	int virtualIstructionMemory[INSTRUCTION_MEMORY_SIZR];
	int pc;

	void memory(int address, int writeData, int read, int write, int *readData);
	void register_file(int readRegister1, int readRegister2, int writeRegister, int writeData, int regWrite, int *readData1, int *readData2);
	void alu(int a, int b, int aluOp, int *out, int *zero, int *negative);
	void mux(int a0, int a1, int c, int *out);
	void control(Op op, int *regDst, int *jump, int *branch, int *memRead, int *memToReg, AluOp *aluOp, int *memWrite, int *aluSrc, int *regWrite, int *slt);
	int binToDec(string bin);
	void writeInstructionMemory(string code);

};