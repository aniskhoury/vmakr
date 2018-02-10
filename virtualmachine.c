#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "virtualmachine.h"
/**
 * Virtual Machine - AKRasm language
 * AKRasm is an assembly language developed
 * for simulate Genetic Algorithms
 * 
 * Author: Anïs Khoury Ribas
 *  
 */

/* Copy the 32-bits instruction to the proper structure of instruction defined
   in typedef instruction. Remember: the order of structure is important
   Intel use little-endian: the important is not where you start, is where you end
*/


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
instruction createInstruction(unsigned int i){
	instruction b;
	memcpy(&b, &i, sizeof(struct instruction));
	return b;
}
/*Return 0 if something go wrong, like addressing wrong memory
  Return 1 if all okey
*/
int addInstruction(instruction i,VirtualMachine *vm){
	AritmeticInstruction add;
	memcpy(&add, &i, sizeof(AritmeticInstruction));
	vm->datamemory[0] = 5;
	//Check if instruction have access  the position of memory requested
	if (haveAccessMemory(add.dest,vm) == 0){
		return 0;
	}
	if (add.addr == 1){
		if (haveAccessMemory(add.inputA,vm) == 0 || haveAccessMemory(add.inputB,vm) == 0){
			return 0;
		}
		vm->datamemory[add.dest] = vm->datamemory[add.inputA] + vm->datamemory[add.inputB];
	}else{
		vm->datamemory[add.dest] = add.inputA + add.inputB;
	}
	return 1;
}
/*Return 0 if something go wrong, like addressing wrong memory
  Return 1 if all okey
*/
int subInstruction(instruction i,VirtualMachine *vm){	
	AritmeticInstruction sub;
	memcpy(&sub, &i, sizeof(AritmeticInstruction));
	vm->datamemory[0] = 5;
	//Check if instruction have access  the position of memory requested
	if (haveAccessMemory(sub.dest,vm) == 0){
		return 0;
	}
	if (sub.addr == 1){
		if (haveAccessMemory(sub.inputA,vm) == 0 || haveAccessMemory(sub.inputB,vm) == 0){
			return 0;
		}
		vm->datamemory[sub.dest] = vm->datamemory[sub.inputA] - vm->datamemory[sub.inputB];
	}else{
		vm->datamemory[sub.dest] = sub.inputA - sub.inputB;
	}
	return 1;
}
/*Return 0 if something go wrong, like addressing wrong memory
  Return 1 if all okey
*/
int mulInstruction(instruction i,VirtualMachine *vm){	
	AritmeticInstruction mul;
	memcpy(&mul, &i, sizeof(AritmeticInstruction));
	//Check if instruction have access  the position of memory requested
	if (haveAccessMemory(mul.dest,vm)   == 0){
		return 0;
	}
	if (mul.addr == 1){
		if (haveAccessMemory(mul.inputA,vm) == 0 || haveAccessMemory(mul.inputB,vm) == 0){
			return 0;
		}
		vm->datamemory[mul.dest] = vm->datamemory[mul.inputA] * vm->datamemory[mul.inputB];
	}else{
		vm->datamemory[mul.dest] = mul.inputA * mul.inputB;
	}
	return 1;
}
int divInstruction(instruction i,VirtualMachine *vm){	
	AritmeticInstruction div;
	memcpy(&div, &i, sizeof(AritmeticInstruction));
	//Check if instruction have access  the position of memory requested
	if (haveAccessMemory(div.dest,vm) == 0){
		return 0;
	}

	if (div.addr == 1){
		if (haveAccessMemory(div.inputA,vm) == 0 || haveAccessMemory(div.inputB,vm) == 0){
			return 0;
		}
		//Div. by 0 not allowed
		if (vm->datamemory[div.inputB] == 0){
			return 0;
		}
		vm->datamemory[div.dest] = vm->datamemory[div.inputA] / vm->datamemory[div.inputB];
	}else{
		vm->datamemory[div.dest] = div.inputA / div.inputB;

	}
	return 1;
}
/*Return 0 if something go wrong, like addressing wrong memory
  Return 1 if all okey
*/
int andInstruction(instruction i, VirtualMachine *vm){
	LogicInstructionsTwoInput and;
	memcpy(&and, &i, sizeof(LogicInstructionsTwoInput));
	
	//Check if instruction have access  the position of memory requested
	if (haveAccessMemory(and.dest,vm) == 0){
		return 0;
	}
	if (and.addr == 1){
		if (haveAccessMemory(and.inputA,vm) == 0 || haveAccessMemory(and.inputB,vm) == 0){
			return 0;
		}
		vm->datamemory[and.dest] = vm->datamemory[and.inputA] & vm->datamemory[and.inputB];
	}else{
		vm->datamemory[and.dest] = and.inputA & and.inputB;
	}
	return 1;
}
/*Return 0 if something go wrong, like addressing wrong memory
  Return 1 if all okey
*/
int orInstruction(instruction i, VirtualMachine *vm){
	LogicInstructionsTwoInput or;
	memcpy(&or, &i, sizeof(LogicInstructionsTwoInput));
	
	//Check if instruction have access  the position of memory requested
	if (haveAccessMemory(or.dest,vm) == 0){
		return 0;
	}
	if (or.addr == 1){
		if (haveAccessMemory(or.inputA,vm) == 0 || haveAccessMemory(or.inputB,vm) == 0){
			return 0;
		}
		vm->datamemory[or.dest] = vm->datamemory[or.inputA] & vm->datamemory[or.inputB];
	}else{
		vm->datamemory[or.dest] = or.inputA & or.inputB;
	}
	return 1;

}
/*Return 0 if something go wrong, like addressing wrong memory
  Return 1 if all okey
*/
int xorInstruction(instruction i, VirtualMachine *vm){
	LogicInstructionsTwoInput xor;
	memcpy(&xor, &i, sizeof(LogicInstructionsTwoInput));
	
	//Check if instruction have access  the position of memory requested
	if (haveAccessMemory(xor.dest,vm) == 0){
		return 0;
	}
	if (xor.addr == 1){
		if (haveAccessMemory(xor.inputA,vm) == 0 || haveAccessMemory(xor.inputB,vm) == 0){
			return 0;
		}
		vm->datamemory[xor.dest] = vm->datamemory[xor.inputA] ^ vm->datamemory[xor.inputB];
	}else{
		vm->datamemory[xor.dest] = xor.inputA ^ xor.inputB;
	}
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
	printf("Opcode %u\n",i.opcode);

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
			vm->ptrFunc = &mulInstruction;
			break;
		case OPCODE_DIV:
			printf("DIV\n");
			vm->ptrFunc = &divInstruction;
			break;
		case OPCODE_MOVE:
			printf("MOVE\n"); //TODO: Prepare correctly the bits of move
			break;
		case OPCODE_AND:
			printf("AND\n");
			vm->ptrFunc = &andInstruction;
			break;
		case OPCODE_OR:
			printf("OR\n");
			vm->ptrFunc = &orInstruction;
			break;
		case OPCODE_NOT:
			printf("NOT\n");  //TODO: Prepare struct of 1 logical input
			break;
		case OPCODE_XOR:
			printf("XOR\n");
			vm->ptrFunc = &xorInstruction;
			break;
		case OPCODE_S_LEFT:
			printf("SHIFT-LEFT\n");
			//vm->ptrFunc = &shiftLeftInstruction;
			break;
		case OPCODE_S_RIGHT:
			printf("SHIFT RIGHT\n");
			//vm->ptrFunc = &shiftRightInstruction;
			break;
		case OPCODE_JZ:
			printf("JZ\n");
			//vm->ptrFunc = &jzInstruction;
			break;
		case OPCODE_JE:
			printf("JE\n");
			//vm->ptrFunc = &jeInstruction;
			break;
		case OPCODE_GOTO:
			printf("SHIFT RIGHT\n");
			//vm->ptrFunc = &gotoRightInstruction;
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
	
	int result;
	result = computeInstruction(vm);
	printf("%d\n",result);
	result = computeInstruction(vm);
	printf("%d\n",result);	
	result = computeInstruction(vm);
	printf("%d\n",result);	
	unsigned int a = 0x05FFFF0F;
	instruction b = createInstruction(a);
	
	printf("num %u\n",b.opcode);
	
}
