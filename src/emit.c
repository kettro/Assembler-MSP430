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
Emit current_emit = { .index = 0, .initial_lc = 0, .checksum = 0 };
static char loc_cntr_inc_via_addrmode[] = {0,2,2,2,0,0,2};
static int as_val[] = {0,1,1,1,2,3,3};
static int ad_val[] = {0,1,1,1};
// Local Function Prototypes
void emit(uint8_t value, uint16_t lc);
void triggerEmit(uint16_t lc);
void emit_I(Inst* inst_ptr, OperandVal* src, OperandVal* dst, uint16_t lc);
// Extern Variables
extern uint16_t location_counter;
extern FILE* s19_file;
// Extern Function Prototypes

/* Emit
 * Desc: Adds a byte to the current S19 record
 * Param: 8-bit value, 16-bit location counter
 * Return: none
 * Results: adds to the current record, sends record if full
 */
void emit(uint8_t value, uint16_t lc)
{
  // gets data, and tacks it onto the end of the current_emit, and incs the index
  current_emit.message[current_emit.index] = value;
  current_emit.index += 1;
  current_emit.checksum += value;
  location_counter++;
  if(current_emit.index == MAX_EMIT_LENGTH-1){ // 32 - 1 => 32 postions
    triggerEmit(lc);
  }
}

/* Trigger Emit
 * Desc: send out the current record to the s19 file.
 * Param: location counter
 * Return: none
 * Results: printf to the .s19 file; calculates the checksum; resets the current_emit, at the passed lc
 */
void triggerEmit(uint16_t lc)
{
  int i;
  if(current_emit.index == 0){ 
    current_emit.initial_lc = lc;
    return;
  }
  uint8_t checksum = ((current_emit.initial_lc) & 0xFF) + ((current_emit.initial_lc >> 8) & 0xFF);
  uint8_t length = current_emit.index + 3;
  checksum += length;
  checksum += ~(current_emit.checksum) & 0xFF;
  fprintf(s19_file, "S1%02x%04x", length, current_emit.initial_lc);
  for(i = 0; i < current_emit.index; i++){
    fprintf(s19_file, "%02x", current_emit.message[i]);
  }
  fprintf(s19_file, "%02x\n", checksum);

  // calculate the checksum of current_emit
  // fprintf to the .s19 file
  // clean the current emit buffer, set index to 0
  for(i = 0; i < MAX_EMIT_LENGTH; i++){
    current_emit.message[i] = '\0';
  }
  current_emit.index = 0;
  current_emit.initial_lc = lc;
  current_emit.checksum = 0;
  return;
  // set new location counter to lc
  // return
}

/* Emit I
 * Desc: emit for instructions
 * Param: Instruction pointer, source, destination operands, location counter
 * Return: none
 * Results: given an instruction type, processes and emits the proper value(s)
 */
void emit_I(Inst* inst_ptr, OperandVal* src, OperandVal* dst, uint16_t lc)
{
  // check if the opvals are null: if so, then ignore it. 
  Type1Inst inst1;
  Type2Inst inst2;
  Type3Inst inst3;
  uint16_t as = 0, sval0= 0, sval1 = 0, ad = 0, dval0 = 0, dval1 = 0;
  uint8_t emit_val;
  if(inst_ptr->type == NONE){ // get it out of the way
    emit(0x00, lc++);
    emit(0x13, lc++);
    return;
  }
  as = as_val[src->mode];
  if(loc_cntr_inc_via_addrmode[src->mode]){
    sval1 = src->val1;
  }
  sval0 = src->val0;

  if(dst != NULL){
    ad = ad_val[dst->mode];
    if(loc_cntr_inc_via_addrmode[dst->mode]){
      dval1 = dst->val1;
    }
    dval0 = dst->val0;
  }
  switch(inst_ptr->type){
    case NONE:
      break;
    case ONE:
      inst1.value = 0;
      inst1.value |= inst_ptr->opcode;
      inst1.bw = inst_ptr->b_w;
      inst1.as = as;
      inst1.reg = sval0;
      emit_val = inst1.value & 0xFF;
      emit(emit_val, lc++);
      emit_val = (inst1.value >> 8) & 0xFF;
      emit(emit_val, lc++);
      break;
    case TWO:
      inst2.value = 0;
      inst2.value |= inst_ptr->opcode;
      inst2.src = sval0;
      inst2.ad = ad;
      inst2.bw = inst_ptr->b_w;
      inst2.as = as;
      inst2.dst = dval0;
      emit_val = inst2.value & 0xFF;
      emit(emit_val, lc++);
      emit_val = (inst2.value >> 8) & 0xFF;
      emit(emit_val, lc++);
      break;
    case JUMP:
      inst3.value = 0;
      inst3.value |= inst_ptr->opcode;
      inst3.addr = (sval0 - (location_counter)) / 2; // using the LC, div by 2
      emit_val = inst3.value & 0xFF;
      emit(emit_val, lc++);
      emit_val = (inst3.value >> 8) & 0xFF;
      emit(emit_val, lc++);
      return;
  }
  if(loc_cntr_inc_via_addrmode[src->mode]){
    emit_val = sval1 & 0xFF;
    emit(emit_val, lc++);
    emit_val = (sval1 >> 8) & 0xFF;
    emit(emit_val, lc++);
  }
  if(dst != NULL && loc_cntr_inc_via_addrmode[dst->mode]){
    emit_val = dval1 & 0xFF;
    emit(emit_val, lc++);
    emit_val = (dval1 >> 8) & 0xFF;
    emit(emit_val, lc++);
  }

  return;
}
