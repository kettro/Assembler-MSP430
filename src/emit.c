// Includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include "token_data_structures.h"
#include "inst_table.h"
#include "symbol_table_data_structures.h"
#include "emit.h"
// Defines
#define MAX_EMIT_LENGTH     32
// Local Variables
Emit current_emit = { .index = 0, .initial_lc = 0 };
// Local Function Prototypes
void emit(uint8_t value, uint16_t lc);
void triggerEmit(uint16_t lc);
void emit_I(Inst* inst_ptr, OperandVal* src, OperandVal* dst, uint16_t lc);
// Extern Variables
extern uint16_t location_counter;
extern FILE* s19_file;
// Extern Function Prototypes

void emit(uint8_t value, uint16_t lc)
{
  // gets data, and tacks it onto the end of the current_emit, and incs the index
  current_emit.message[current_emit.index] = value;
  current_emit.index += 1;
  location_counter++;
  if(current_emit.index == MAX_EMIT_LENGTH){
    triggerEmit(lc);
  }
}
void triggerEmit(uint16_t lc)
{
  uint8_t checksum = 145; // ???? need to calc this actually;
  int i;
  fprintf(s19_file, "%04x", current_emit.initial_lc);
  for(i = 0; i < current_emit.index; i++){
    fprintf(s19_file, "%02x", current_emit.message[i]);
  }
  fprintf(s19_file, "%02x", checksum);
  // calculate the checksum of current_emit
  // fprintf to the .s19 file
  // clean the current emit buffer, set index to 0
  for(i = 0; i < MAX_EMIT_LENGTH; i++){
    current_emit.message[i] = '\0';
  }
  current_emit.index = 0;
  current_emit.initial_lc = lc;
  return;
  // set new location counter to lc
  // return
}

void emit_I(Inst* inst_ptr, OperandVal* src, OperandVal* dst, uint16_t lc)
{
  // check if the opvals are null: if so, then ignore it. 
  Type1Inst inst1;
  Type2Inst inst2;
  Type3Inst inst3;
  uint8_t emit_val;
  uint16_t jump = 0;
  switch(inst_ptr->type){
    case NONE:
      emit(0x11, lc);
      emit(0x80, lc);
    case ONE:
      inst1.opc = inst_ptr->opcode;
      inst1.bw = inst_ptr->b_w;
      inst1.as = 0; //??
      inst1.reg = 0; // ???
      emit_val = inst1.value & 0xFF;
      emit(emit_val, lc);
      emit_val = (inst1.value >> 8) & 0xFF;
      emit(emit_val, lc);
      // need to add more emits for more args, depending on mode
      break;
    case TWO:
      inst2.opc = inst_ptr->opcode;
      inst2.src = 0; // ??
      inst2.ad = 0; // ??
      inst2.bw = inst_ptr->b_w;
      inst2.as = 0; // ??
      inst2.dst = 0; // ??
      emit_val = inst2.value & 0xFF;
      emit(emit_val, lc);
      emit_val = (inst2.value >> 8) & 0xFF;
      emit(emit_val, lc);
      // need to add more emits for more args, depending on mode
      break;
    case JUMP:
      inst3.opc = inst_ptr->opcode;
      jump = src->val0; //?
      jump -= 2;
      jump = (jump - location_counter)/2; // using the LC to get this to work
      inst3.addr = jump;
      emit_val = inst3.value & 0xFF;
      emit(emit_val, lc);
      emit_val = (inst3.value >> 8) & 0xFF;
      emit(emit_val, lc);
      // need to add more emits for more args, depending on mode
      break;
  }
  return;
}
