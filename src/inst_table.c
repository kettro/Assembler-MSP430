// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "symbol_table_data_structures.h"
#include "inst_table.h"
#include "token_data_structures.h"
// Defines
// Local Variables
Inst inst_table[TOTAL_INSTRUCTIONS] = {
  {"ADD",   0x5000, 2, WORD}, // add
  {"ADD.B", 0x5000, 2, BYTE}, // add
  {"ADD.W", 0x5000, 2, WORD}, // add
  {"ADDC",  0x6000, 2, WORD}, // addc
  {"ADDC.B",0x6000, 2, BYTE}, // addc
  {"ADDC.W",0x6000, 2, WORD}, // addc
  {"AND",   0xF000, 2, WORD}, // and
  {"AND.B", 0xF000, 2, BYTE}, // and
  {"AND.W", 0xF000, 2, WORD}, // and
  {"BIC",   0xC000, 2, WORD}, // bic
  {"BIC.B", 0xC000, 2, BYTE}, // bic
  {"BIC.W", 0xC000, 2, WORD}, // bic
  {"BIS",   0xD000, 2, WORD}, // bis
  {"BIS.B", 0xD000, 2, BYTE}, // bis
  {"BIS.W", 0xD000, 2, WORD}, // bis
  {"BIT",   0xB000, 2, WORD}, // bit
  {"BIT.B", 0xB000, 2, BYTE}, // bit
  {"BIT.W", 0xB000, 2, WORD}, // bit
  {"CALL",  0x1280, 1, }, // call
  {"CMP",   0x9000, 2, WORD}, // cmp
  {"CMP.B", 0x9000, 2, BYTE}, // cmp
  {"CMP.W", 0x9000, 2, WORD}, // cmp
  {"DADD",  0xA000, 2, WORD}, // dadd
  {"DADD.B",0xA000, 2, BYTE}, // dadd
  {"DADD.W",0xA000, 2, WORD}, // dadd
  {"JC",    0x2C00, 3, }, // jc
  {"JEQ",   0x2400, 3, }, // jeq
  {"JGE",   0x3400, 3, }, // jge
  {"JHS",   0x2C00, 3, }, // jhs
  {"JL",    0x3800, 3, }, // jl
  {"JLO",   0x2800, 3, }, // jlo
  {"JMP",   0x3C00, 3, }, // jmp
  {"JN",    0x3000, 3, }, // jn
  {"JNC",   0x2800, 3, }, // jnc
  {"JNE",   0x2000, 3, }, // jne
  {"JNZ",   0x2000, 3, }, // jnz
  {"JZ",    0x2400, 3, }, // jz
  {"MOV",   0x4000, 2, WORD}, // mov
  {"MOV.B", 0x4000, 2, BYTE}, // mov
  {"MOV.W", 0x4000, 2, WORD}, // mov
  {"PUSH",  0x1200, 1, WORD}, // push
  {"PUSH.B",0x1200, 1, BYTE}, // push
  {"PUSH.W",0x1200, 1, WORD}, // push
  {"RETI",  0x1300, 0, }, // reti
  {"RRA",   0x1100, 1, WORD}, // rra
  {"RRA.B", 0x1100, 1, BYTE}, // rra
  {"RRA.W", 0x1100, 1, WORD}, // rra
  {"RRC",   0x1000, 1, WORD}, // rrc
  {"RRC.B", 0x1000, 1, BYTE}, // rrc
  {"RRC.W", 0x1000, 1, WORD}, // rrc
  {"SUB",   0x8000, 2, WORD}, // sub
  {"SUB.B", 0x8000, 2, BYTE}, // sub
  {"SUB.W", 0x8000, 2, WORD}, // sub
  {"SUBC",  0x7000, 2, WORD}, // subc
  {"SUBC.B",0x7000, 2, BYTE}, // subc
  {"SUBC.W",0x7000, 2, WORD}, // subc
  {"SWPB",  0x1080, 1, }, // swpb
  {"SXT",   0x1180, 1, }, // sxt
  {"XOR",   0xE000, 2, WORD}, // xor
  {"XOR.B", 0xE000, 2, BYTE}, // xor
  {"XOR.W", 0xE000, 2, WORD}, // xor
};  
static char loc_cntr_inc_via_addrmode[] = {0 , 2, 2, 2, 0, 0, 2};
// Local Function Prototypes
int isInst(char* token);
Inst* getInst(char* token);
void handleInst_1(char* command, char* operands);
void handleInst_2(char* command, char* operands);

// Extern Variables
extern uint16_t location_counter; // global
// Extern Function Prototypes
extern int parseType1(char* operand, OperandVal* src);
extern int parseType2(char* operand, OperandVal* src, OperandVal* dst);
extern int parseType3(char* operand, OperandVal* src);
// Definitions

int isInst(char* token)
{
  char dup[strlen(token)];
  strcpy(dup, token);
  int i;
  for(i = 0; i < strlen(dup); i++){
    dup[i] = toupper(dup[i]);
  }
  for (i = 0; i < TOTAL_INSTRUCTIONS; i++){
    if(strcmp(dup, inst_table[i].name) == 0){
      return 1;
    }
  }
  return 0;
}

Inst* getInst(char* token)
{
  int i;
  char dup[strlen(token)];
  strcpy(dup, token);

  for(i = 0; i < strlen(dup); i++){
    dup[i] = toupper(dup[i]);
  }

  for(i = 0; i < TOTAL_INSTRUCTIONS; i++){
    if(strcmp(dup, inst_table[i].name) == 0){
      return &(inst_table[i]);
    }
  }
  return NULL;
}

/* Handle Instruction for 1st Pass
 * params: command: string of instruction
 *         operands: string of the argument(s) to the instruction
 * returns: none
 * Resultants: Identify the LC increments, and increment
 */
void handleInst_1(char* command, char* operands)
{
  OperandVal src;
  OperandVal dst;
  Inst* inst_ptr = getInst(command);
  switch(inst_ptr->type){
    case NONE:
      // is RETI
      location_counter += 2; // for the inst itself
    case ONE:
      // one operand;
      parseType1(operands, &src);
      if(src.mode == BAD_ADDR){
        // error: badd addr
        return;
      }
      location_counter += 2; // for the inst itself
      location_counter += loc_cntr_inc_via_addrmode[src.mode];
      break;
    case TWO:
      if(parseType2(operands, &src, &dst) == 0){
        // error:
        return;
      }
      if(dst.mode == BAD_ADDR || src.mode == BAD_ADDR){
        // error: bad addr
        return;
      }
      if(dst.mode == INDIRECT || dst.mode == INDIRECT_AA || dst.mode == IMMEDIATE){
        // error: improper dst addr mode
        return;
      }
      location_counter += 2; // for the inst itself
      location_counter += loc_cntr_inc_via_addrmode[src.mode];
      location_counter += loc_cntr_inc_via_addrmode[dst.mode];
      break;
    case JUMP:
      location_counter += location_counter % 2; // must have the command lie on an even lc
      if(parseType3(operands, &src) == 0){
        // error in parsing
        return;
      }
      if(src.mode == BAD_ADDR){
        //error: bad addr
        return;
      }
      location_counter += 2; // for the inst itself
      location_counter += loc_cntr_inc_via_addrmode[src.mode];
      break;
  }
}

/* Handle Instructions for 2nd Pass
 * Params: command: the instruction string
 *         operands: the argument(s) to the instruction
 * returns: none
 * Resultants:  send one or more instructions to the emitter via emit()
 *              increase the LC by the required amount per call to emit()
 */
void handleInst_2(char* command, char* operands)
{
  return;
}
