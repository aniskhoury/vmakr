#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
 * Virtual Machine - AKRasm language
 * AKRasm is an assembly language developed
 * for simulate Genetic Algorithms
 * 
 * Author: Anïs Khoury Ribas
 *  
 */
#define BITS_INSTRUCTION 32
#define BITS_OPCODE       8    //2^8 instructions

#define OPCODE_ADD 0x00
#define OPCODE_SUB 0x01
#define OPCODE_MUL 0x02
#define OPCODE_DIV 0x03
#define OPCODE_PUT 0x04
#define OPCODE_AND 0x05
#define OPCODE_OR  0x06
#define OPCODE_NOT 0x07
#define OPCODE_XOR 0x08
#define OPCODE_JZ  0x09   
#define OPCODE_JQ  0x10

typedef struct VirtualMachine VirtualMachine;
typedef struct LogicInstructionsTwoInput AritmeticInstruction; //same structure but different operations!
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


/* Copy the 32-bits instruction to the proper structure of instruction defined
   in typedef instruction. Remember: the order of structure is important
   Intel use little-endian: the important is not where you start, is where you end
*/
instruction createInstruction(unsigned int i){
	instruction b;
	memcpy(&b, &i, sizeof(struct instruction));
	return b;
}

unsigned int haveAccessMemory(unsigned int n,VirtualMachine *vm){
	if (vm->maxSizeData < n)
		return 0;
	return 1;
}
VirtualMachine *init_virtualmachine(unsigned int maxMemoryCriature, unsigned int maxDataMemory){
	VirtualMachine *vm;
	vm = malloc(sizeof(VirtualMachine));

	if (vm == NULL){
		return NULL;
	}
	vm->criature = malloc(sizeof(unsigned int)*maxMemoryCriature);
	vm->datamemory = malloc(sizeof(unsigned int)*maxDataMemory);

	if (vm->criature == NULL || vm->datamemory == NULL){
		return NULL;
	}
	vm->maxSizeCriature = maxMemoryCriature;
	vm->maxSizeData = maxDataMemory;
	vm->PC = -1;
	vm->sizeCriature = 0;
	return vm;
}

void free_VirtualMachine(VirtualMachine *vm){
	free(vm->criature);
	free(vm->datamemory);
	free(vm);
}
void loadCriature(unsigned int *criature, int n,VirtualMachine *vm){
	int i;
	for (i=0; i<n; i++){
		vm->criature[i] = criature[i];
	}
	vm->sizeCriature = n-1;
}

/*Return 0 if something go wrong, like addressing wrong memory
  Return 1 if all okey
*/
int addInstruction(instruction i,VirtualMachine *vm){
	LogicInstructionsTwoInput add;
	memcpy(&add, &i, sizeof(struct LogicInstructionsTwoInput));
	vm->datamemory[0] = 5;
	//Check if instruction have access  the position of memory requested
	if (haveAccessMemory(add.inputA,vm) == 0 || 
	    haveAccessMemory(add.inputB,vm) == 0 || 
	    haveAccessMemory(add.dest,vm)   == 0){
		return 0;
	}
		
	vm->datamemory[add.dest] = vm->datamemory[add.inputA] + vm->datamemory[add.inputB];
	return 1;
}
int subInstruction(instruction i,VirtualMachine *vm){	
	LogicInstructionsTwoInput sub;
	memcpy(&sub, &i, sizeof(struct LogicInstructionsTwoInput));
	vm->datamemory[0] = 5;
	//Check if instruction have access  the position of memory requested
	if (haveAccessMemory(sub.inputA,vm) == 0 || 
	    haveAccessMemory(sub.inputB,vm) == 0 || 
	    haveAccessMemory(sub.dest,vm)   == 0){
		return 0;
	}
		
	vm->datamemory[sub.dest] = vm->datamemory[sub.inputA] - vm->datamemory[sub.inputB];
	return 1;
}
int mulInstruction(instruction i,VirtualMachine *vm){	
	LogicInstructionsTwoInput mul;
	memcpy(&mul, &i, sizeof(struct LogicInstructionsTwoInput));
	//Check if instruction have access  the position of memory requested
	if (haveAccessMemory(mul.inputA,vm) == 0 || 
	    haveAccessMemory(mul.inputB,vm) == 0 || 
	    haveAccessMemory(mul.dest,vm)   == 0){
		return 0;
	}
		
	vm->datamemory[mul.dest] = vm->datamemory[mul.inputA] * vm->datamemory[mul.inputB];
	return 1;
}
int divInstruction(instruction i,VirtualMachine *vm){	
	LogicInstructionsTwoInput div;
	memcpy(&div, &i, sizeof(struct LogicInstructionsTwoInput));
	//Check if instruction have access  the position of memory requested
	if (haveAccessMemory(div.inputA,vm) == 0 || 
	    haveAccessMemory(div.inputB,vm) == 0 || 
	    haveAccessMemory(div.dest,vm)   == 0){
		return 0;
	}
	//Div. by 0 not allowed
	if (vm->datamemory[div.inputB] == 0){
		return 0;
	}
	vm->datamemory[div.dest] = vm->datamemory[div.inputA] / vm->datamemory[div.inputB];
	return 1;
}
int computeInstruction(VirtualMachine *vm){
	int result;
	instruction i;
	vm->PC = vm->PC+1;
	
	
	/*Optimized vm->sizeCriature, the real
	  size of criature is vm->sizeCriature+1, but PC start with 0
	  For avoiding sub here, we sub at loadCriature.*/
	if (vm->PC > vm->sizeCriature){
		return -1;
	}
	unsigned int instruc = vm->criature[vm->PC];
	i= createInstruction(instruc);
	printf("Opcode %u\n",i.parameter);

	//Compute properly the instruction
	switch(i.opcode){
		case OPCODE_ADD:
			vm->ptrFunc = &addInstruction;
			printf("ADD\n");
			break;
		case OPCODE_SUB:
			printf("SUB\n");
			vm->ptrFunc = &subInstruction;
			break;
		case OPCODE_MUL:
			printf("MUL\n");
			break;
		case OPCODE_DIV:
			printf("DIV\n");
			break;
		case OPCODE_PUT:
			printf("PUT\n");
			break;
		case OPCODE_AND:
			printf("AND\n");
			break;
		case OPCODE_OR:
			printf("OR\n");
			break;
		case OPCODE_NOT:
			printf("NOT\n");
			break;
		case OPCODE_XOR:
			printf("XOR\n");
			break;
		//unknown instruction
		default:
			return -1;
	}
	//Parameter: properly prepared for instruction to execute
	result = vm->ptrFunc(i,vm);

	return 1;
}

int main(void){
	VirtualMachine *vm;
	unsigned int *criature = malloc(sizeof(int)*2);
	criature[0] = 0x00FFFFFF;
	criature[1] = 1<<24;
	vm = init_virtualmachine(5,5);
	loadCriature(criature,2,vm);
	
	int result = computeInstruction(vm);
	printf("%d\n",result);
	result = computeInstruction(vm);
	printf("%d\n",result);	
	result = computeInstruction(vm);
	printf("%d\n",result);	
	unsigned int a = 0x05FFFF0F;
	instruction b = createInstruction(a);
	
	printf("num %u\n",b.opcode);
	
}