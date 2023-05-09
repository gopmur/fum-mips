#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class FumMips {

public:
	
	static enum OpCode {
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

	static enum Funct {
		ADD = 0b100000,
		SUB = 0b100010,
		AND = 0b100100,
		OR = 0b100101,
		SLT = 0b101010
	};

	static struct RType {
		OpCode opcode;
		int rs;
		int rt;
		int rd;
		int shamt;
		Funct funct;
	};

	static struct IType {
		OpCode opcode;
		int rs;
		int rt;
		int immediate;
	};

	static struct JType {
		OpCode opcode;
		int immediate;
	};

	static union Instruction {
		RType r;
		IType i;
		JType j;
	};

	static enum AluOp {
		AND,
		OR,
		ADD,
		SUB,
		SLT
	};

	static enum Op {
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

private:
	int virtualMemory[MEMORY_SIZE];
	int virtualRegisterFile[REGISTER_SIZE];
	int virtualIstructionMemory[INSTRUCTION_MEMORY_SIZR];
	int pc;

public:

	void memory(int address, int writeData, int read, int write, int *readData) {
		if (read && !write) {
			*readData = virtualMemory[address >> 2];
		}
		else if (!read && write) {
			virtualMemory[address >> 2] = writeData;
		}
	}

	void register_file(int readRegister1, int readRegister2, int writeRegister, int writeData, int regWrite, int *readData1, int *readData2) {
		*readData1 = virtualRegisterFile[readRegister1];
		*readData2 = virtualRegisterFile[readRegister2];
		if (regWrite) {
			virtualRegisterFile[writeRegister] = writeData;
		}
	}

	void alu(int a, int b, int aluOp, int *out, int *zero, int *negative) {

		*zero = 0;
		*negative = 0;

		switch ((AluOp)aluOp) {
		case AluOp::AND:
			*out = a & b;
			break;

		case AluOp::OR:
			*out = a | b;
			break;

		case AluOp::ADD:
			*out = a + b;
			break;

		case AluOp::SUB:
			*out = a - b;
			break;

		case AluOp::SLT:
			*out = a - b;
			break;

		}

		if (*out == 0) {
			*zero = 1;
		}

		else if (*out < 0) {
			*negative = 1;
		}

	}

	void mux(int a0, int a1, int c, int *out) {
		*out = a1 ? a0 : c;
	}

	void control(Op op, int *regDst, int *jump, int *branch, int *memRead, int *memToReg, AluOp *aluOp, int *memWrite, int *aluSrc, int *regWrite, int *slt) {
		switch (op) {
		case Op::ADD:
			*regDst = 1;
			*jump = 0;
			*branch = 0;
			*memRead = 0;
			*memToReg = 0;
			*aluOp = AluOp::ADD;
			*memWrite = 0;
			*aluSrc = 0;
			*regWrite = 1;
			*slt = 0;
			break;

		case Op::AND:
			*regDst = 1;
			*jump = 0;
			*branch = 0;
			*memRead = 0;
			*memToReg = 0;
			*aluOp = AluOp::AND;
			*memWrite = 0;
			*aluSrc = 0;
			*regWrite = 1;
			*slt = 0;
			break;

		case Op::OR:
			*regDst = 1;
			*jump = 0;
			*branch = 0;
			*memRead = 0;
			*memToReg = 0;
			*aluOp = AluOp::OR;
			*memWrite = 0;
			*aluSrc = 0;
			*regWrite = 1;
			*slt = 0;
			break;

		case Op::SLT:
			*regDst = 1;
			*jump = 0;
			*branch = 0;
			*memRead = 0;
			*memToReg = 0;
			*aluOp = AluOp::SLT;
			*memWrite = 0;
			*aluSrc = 0;
			*regWrite = 1;
			*slt = 1;
			break;

		case Op::SUB:
			*regDst = 1;
			*jump = 0;
			*branch = 0;
			*memRead = 0;
			*memToReg = 0;
			*aluOp = AluOp::SUB;
			*memWrite = 0;
			*aluSrc = 0;
			*regWrite = 1;
			*slt = 0;
			break;

		case Op::ADDI:
			*regDst = 0;
			*jump = 0;
			*branch = 0;
			*memRead = 0;
			*memToReg = 0;
			*aluOp = AluOp::ADD;
			*memWrite = 0;
			*aluSrc = 1;
			*regWrite = 1;
			*slt = 0;
			break;

		case Op::ANDI:
			*regDst = 0;
			*jump = 0;
			*branch = 0;
			*memRead = 0;
			*memToReg = 0;
			*aluOp = AluOp::AND;
			*memWrite = 0;
			*aluSrc = 1;
			*regWrite = 1;
			*slt = 0;
			break;

		case Op::ORI:
			*regDst = 0;
			*jump = 0;
			*branch = 0;
			*memRead = 0;
			*memToReg = 0;
			*aluOp = AluOp::OR;
			*memWrite = 0;
			*aluSrc = 1;
			*regWrite = 1;
			*slt = 0;
			break;

		case Op::SLTI:
			*regDst = 0;
			*jump = 0;
			*branch = 0;
			*memRead = 0;
			*memToReg = 0;
			*aluOp = AluOp::SUB;
			*memWrite = 0;
			*aluSrc = 1;
			*regWrite = 1;
			*slt = 1;
			break;

		case Op::BEQ:
			*regDst = 0;
			*jump = 0;
			*branch = 1;
			*memRead = 0;
			*memToReg = 0;
			*aluOp = AluOp::SUB;
			*memWrite = 0;
			*aluSrc = 0;
			*regWrite = 0;
			*slt = 0;
			break;

		case Op::BNE:
			*regDst = 0;
			*jump = 0;
			*branch = 1;
			*memRead = 0;
			*memToReg = 0;
			*aluOp = AluOp::SUB;
			*memWrite = 0;
			*aluSrc = 0;
			*regWrite = 0;
			*slt = 0;
			break;

		case Op::LW:
			*regDst = 0;
			*jump = 0;
			*branch = 0;
			*memRead = 1;
			*memToReg = 1;
			*aluOp = AluOp::ADD;
			*memWrite = 0;
			*aluSrc = 1;
			*regWrite = 1;
			*slt = 0;
			break;

		case Op::SW:
			*regDst = 0;
			*jump = 0;
			*branch = 0;
			*memRead = 0;
			*memToReg = 0;
			*aluOp = AluOp::ADD;
			*memWrite = 1;
			*aluSrc = 1;
			*regWrite = 0;
			*slt = 0;
			break;

		case Op::J:
			*regDst = 0;
			*jump = 1;
			*branch = 0;
			*memRead = 0;
			*memToReg = 0;
			*aluOp = AluOp::ADD;
			*memWrite = 0;
			*aluSrc = 0;
			*regWrite = 0;
			*slt = 0;
			break;
		}
	}

	void instructionMemory(int address, int *instruction) {
		*instruction = virtualIstructionMemory[address];
	}

	void init() {
		pc = 0;
	}

	void next_clock() {
		OpCode opcode = (OpCode) (virtualIstructionMemory[pc] >> 26);
		Instruction instruction;
		Op op;
		// j type
		if (opcode == OpCode::J) {
			instruction.j.opcode = opcode;
			instruction.j.immediate = virtualIstructionMemory[pc] & ((1 << 26) - 1);
			op = Op::J;
		}
		// r type
		else if (opcode == OpCode::RTYPE) {
			instruction.r.opcode = opcode;
			instruction.r.rs = (virtualIstructionMemory[pc] >> 21) & ((1 << 5) - 1);
			instruction.r.rt = (virtualIstructionMemory[pc] >> 16) & ((1 << 5) - 1);
			instruction.r.rd = (virtualIstructionMemory[pc] >> 11) & ((1 << 5) - 1);
			instruction.r.shamt = (virtualIstructionMemory[pc] >> 6) & ((1 << 5) - 1);
			instruction.r.funct = (Funct) (virtualIstructionMemory[pc] & ((1 << 6) - 1));
			switch (instruction.r.funct) {

			case Funct::ADD:
				op = Op::ADD;
				break;

			case Funct::SUB:
				op = Op::SUB;
				break;

			case Funct::AND:
				op = Op::AND;
				break;

			case Funct::OR:
				op = Op::OR;
				break;

			case Funct::SLT:
				op = Op::SLT;
				break;

			}
		}
		// i type
		else {
			instruction.i.opcode = opcode;
			instruction.i.rs = (virtualIstructionMemory[pc] >> 21) & ((1 << 5) - 1);
			instruction.i.rt = (virtualIstructionMemory[pc] >> 16) & ((1 << 5) - 1);
			instruction.i.immediate = virtualIstructionMemory[pc] & ((1 << 16) - 1);
	
			switch (instruction.i.opcode) {

			case OpCode::SW:
				op = Op::SW;
				break;

			case OpCode::LW:
				op = Op::LW;
				break;

			case OpCode::ADDI:
				op = Op::ADDI;
				break;

			case OpCode::SLTI:
				op = Op::SLTI;
				break;

			case OpCode::ANDI:
				op = Op::ANDI;
				break;

			case OpCode::ORI:
				op = Op::ORI;
				break;

			case OpCode::BEQ:
				op = Op::BEQ;
				break;

			case OpCode::BNE:
				op = Op::BNE;
				break;

			}
		}

		int regDst;
		int jump;
		int branch;
		int memRead;
		int memToReg;
		AluOp aluOp;
		int memWrite;
		int aluSrc;
		int regWrite;
		int slt;

		int writeRegister;
		int writeRegisterData;

		int readData1;
		int readData2;

		int aluIn2;
		int aluOut;
		int aluZero;
		int aluNegative;

		int memoryReadData;

		int pc4;

		int jumpAddress;
		int branchAddress;

		int pcAddr1;
		int pcAddr2;

		control(op, &regDst, &jump, &branch, &memRead, &memToReg, &aluOp, &memWrite, &aluSrc, &regWrite, &slt);
		
		mux(instruction.r.rt, instruction.r.rd, regDst, &writeRegister);

		register_file(instruction.r.rs, instruction.r.rt, writeRegister, writeRegisterData, regWrite, &readData1, &readData2);

		mux(readData2, instruction.i.immediate, aluSrc, &aluIn2);

		alu(readData1, aluIn2, aluOp, &aluOut, &aluZero, &aluNegative);

		memory(aluOut, readData2, memRead, memWrite, &memoryReadData);

		mux(aluOut, memoryReadData, memToReg, &writeRegisterData);

		register_file(instruction.r.rs, instruction.r.rt, writeRegister, writeRegisterData, regWrite, &readData1, &readData2);

		alu(pc, 4, AluOp::ADD, &pc4, NULL, NULL);

		jumpAddress = (instruction.j.immediate << 2) | ((((1 << 4) - 1) << 28) & pc);

		alu(pc4, instruction.i.immediate << 2, AluOp::ADD, &branchAddress, NULL, NULL);

		mux(pc4, branchAddress, (int) (branch &&aluZero), &pcAddr1);

		mux(pcAddr1, jumpAddress, jump, &pcAddr2);

		pc = pcAddr2;

	}

};
