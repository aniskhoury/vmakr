#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#define BITS_INSTRUCTION 32
#define BITS_OPCODE       8    //2^8 instructions

/*
 * Structure of assembly instruction in 
   https://github.com/aniskhoury/vmakr/blob/master/asm.txt
 */ 
#define OPCODE_ADD     0b00000000
#define OPCODE_SUB     0b00000001
#define OPCODE_MUL     0b00000010
#define OPCODE_DIV     0b00000011
#define OPCODE_MOVE    0b00000100
#define OPCODE_AND     0b00000101
#define OPCODE_OR      0b00000110
#define OPCODE_NOT     0b00000111
#define OPCODE_XOR     0b00001000
#define OPCODE_S_LEFT  0b00001001
#define OPCODE_S_RIGHT 0b00001010
#define OPCODE_JZ      0b00001011   
#define OPCODE_JE      0b00001100
#define OPCODE_GOTO    0b00001101



typedef struct VirtualMachine VirtualMachine;
typedef struct LogicInstructionsTwoInput AritmeticInstruction; 
typedef struct instruction instruction;
typedef struct LogicInstructionsTwoInput LogicInstructionsTwoInput;

typedef struct instruction{
	unsigned int parameter:24;  
	unsigned int opcode:8;      
}instruction;

typedef struct LogicInstructionsTwoInput{
	unsigned int inputB:7; //inputB indicates where or what value will be used for compute instruction	
	unsigned int inputA:8; //inputA indicates where or what value will be used for compute instruction
	unsigned int dest:8;   //dest is the address of memory where will be saved the result of operation
	unsigned int addr:1;    //1-> the instruction will use Addr to get and store values Ex:
	unsigned int opcode:8; //8 bits OPCODE
}LogicInstructionsTwoInput;

typedef struct VirtualMachine {
	int PC;                          /*ProgramCounter*/
	unsigned int maxSizeCriature;    /*num. instruccions-1 of criature reserved by Criature*/
									 /*Optimitzation sizeCriature for function computeInstruction*/
	unsigned int maxSizeData;        /*Lengh of memory reserved by Criature -1*/
	                                 /*Optimitzation -1 for accessMemory function*/
	unsigned int sizeCriature;       /*Lengh of real size criature-1. The sub 1 is for computeinstruction
									   optimization.*/

	/*Isolated memory. This Virtual Machine doesnt follow Von Neumann Architecture*/
	unsigned int *criature;          				/*Every instruction have 32 bits*/
	unsigned int *datamemory;        				/*Every addr of data memory is a 32 bit-unsigned*/
	double *dataDmemory;      						/*Every addr of data memory is a 64 bit*/
	int (*ptrFunc)(instruction i,VirtualMachine *vm); /*Pointer to instruction*/
	
}VirtualMachine;



instruction createInstruction(unsigned int i);
unsigned int haveAccessMemory(unsigned int n,VirtualMachine *vm);
VirtualMachine *init_virtualmachine(unsigned int maxMemoryCriature, unsigned int maxDataMemory);
void free_VirtualMachine(VirtualMachine *vm);
void loadCriature(unsigned int *criature, int n,VirtualMachine *vm);
int addInstruction(instruction i,VirtualMachine *vm);
int subInstruction(instruction i,VirtualMachine *vm);
int mulInstruction(instruction i,VirtualMachine *vm);
int divInstruction(instruction i,VirtualMachine *vm);
int andInstruction(instruction i, VirtualMachine *vm);
int orInstruction(instruction i, VirtualMachine *vm);
int xorInstruction(instruction i, VirtualMachine *vm);

#endif
