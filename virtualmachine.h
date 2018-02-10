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

