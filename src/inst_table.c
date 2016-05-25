// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "symbol_table_data_structures.h"
#include "inst_table.h"
// Defines
// Local Variables
Inst inst_table[TOTAL_INSTRUCTIONS] = {
  {.opcode = 0x5000, .type = 2}, // add
  {.opcode = 0x6000, .type = 2}, // addc
  {.opcode = 0xF000, .type = 2}, // and
  {.opcode = 0xC000, .type = 2}, // bic
  {.opcode = 0xD000, .type = 2}, // bis
  {.opcode = 0xB000, .type = 2}, // bit
  {.opcode = 0x1280, .type = 1}, // call
  {.opcode = 0x9000, .type = 2}, // cmp
  {.opcode = 0xA000, .type = 2}, // dadd
  {.opcode = 0x2C00, .type = 3}, // jc
  {.opcode = 0x2400, .type = 3}, // jeq
  {.opcode = 0x3400, .type = 3}, // jge
  {.opcode = 0x2C00, .type = 3}, // jhs
  {.opcode = 0x3800, .type = 3}, // jl
  {.opcode = 0x2800, .type = 3}, // jlo
  {.opcode = 0x3C00, .type = 3}, // jmp
  {.opcode = 0x3000, .type = 3}, // jn
  {.opcode = 0x2800, .type = 3}, // jnc
  {.opcode = 0x2000, .type = 3}, // jne
  {.opcode = 0x2000, .type = 3}, // jnz
  {.opcode = 0x2400, .type = 3}, // jz
  {.opcode = 0x4000, .type = 2}, // mov
  {.opcode = 0x1200, .type = 1}, // push
  {.opcode = 0x1300, .type = 0}, // reti
  {.opcode = 0x1100, .type = 1}, // rra
  {.opcode = 0x1000, .type = 1}, // rrc
  {.opcode = 0x8000, .type = 2}, // sub
  {.opcode = 0x7000, .type = 2}, // subc
  {.opcode = 0x1080, .type = 1}, // swpb
  {.opcode = 0x1180, .type = 1}, // sxt
  {.opcode = 0xE000, .type = 2}, // xor
};
// Local Function Prototypes
void initInstTable(void);
int isInst(char* token, Inst* return_inst);
// Extern Variables
// Extern Function Prototypes
// Definitions

void initInstTable(void)
{
  // for each element in the inst table, passing a name
  // loop and set the character =s in token to uppercase

  strcpy(inst_table[0].name, "ADD");
  strcpy(inst_table[1].name, "ADDC");
  strcpy(inst_table[2].name, "AND");
  strcpy(inst_table[3].name, "BIC");
  strcpy(inst_table[4].name, "BIS");
  strcpy(inst_table[5].name, "BIT");
  strcpy(inst_table[6].name, "CALL");
  strcpy(inst_table[7].name, "CMP");
  strcpy(inst_table[8].name, "DADD");
  strcpy(inst_table[9].name, "JC");
  strcpy(inst_table[10].name, "JEQ");
  strcpy(inst_table[11].name, "JGE");
  strcpy(inst_table[12].name, "JHS");
  strcpy(inst_table[13].name, "JL");
  strcpy(inst_table[14].name, "JLO");
  strcpy(inst_table[15].name, "JMP");
  strcpy(inst_table[16].name, "JN");
  strcpy(inst_table[17].name, "JNC");
  strcpy(inst_table[18].name, "JNE");
  strcpy(inst_table[19].name, "JNZ");
  strcpy(inst_table[20].name, "JZ");
  strcpy(inst_table[21].name, "MOV");
  strcpy(inst_table[22].name, "PUSH");
  strcpy(inst_table[23].name, "RETI");
  strcpy(inst_table[24].name, "RRA");
  strcpy(inst_table[25].name, "RRC");
  strcpy(inst_table[26].name, "SUB");
  strcpy(inst_table[27].name, "SUBC");
  strcpy(inst_table[28].name, "SWPB");
  strcpy(inst_table[29].name, "SXT");
  strcpy(inst_table[30].name, "XOR");
}

int isInst(char* token, Inst* return_inst)
{
  // determine if the string token is an instruction.
  // tokenize the passed token 
  // check the first token: if it matches an instruction, thencont
  // if not, then unfound. 
  char dup[strlen(token)];
  strcpy(dup, token);
  int i;
  for(i = 0; i < strlen(dup); i++){
    toupper(dup[i]);
  }
  char* instruction;
  char* b_w;
  instruction = strtok(dup, ".");
  b_w = strtok(NULL, "\0");

  for (i = 0; i < TOTAL_INSTRUCTIONS; i++){
    if(strcmp(instruction, inst_table[i].name) == 0){
      *return_inst = inst_table[i];
      break;
    }
  }
  if(i == TOTAL_INSTRUCTIONS){ return 0; } // unfound

  if(return_inst->type != JUMP && return_inst->type != NONE){
    if(b_w != NULL){
      if(*b_w == 'W'){
        // is a word
        return_inst->b_w = WORD;
        return 1;
      } else if(*b_w == 'B'){
        // is a byte
        return_inst->b_w = BYTE;
        return 1;
      }
    } else{
      // is a word
      return_inst->b_w = WORD;
      return 1;
    }
  }
  return 0;
}

