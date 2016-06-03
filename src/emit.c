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

void emit(uint8_t value, uint16_t lc)
{
  // gets data, and tacks it onto the end of the current_emit, and incs the index
  current_emit.message[current_emit.index] = value;
  current_emit.index += 1;
  location_counter++;
  printf("lc = %d\n", location_counter);
  printf("index = %d\n", current_emit.index);
  printf("message = %02x = value = %02x\n", current_emit.message[current_emit.index - 1], value);
  if(current_emit.index == MAX_EMIT_LENGTH){
    triggerEmit(lc);
  }
}

void triggerEmit(uint16_t lc)
{
  uint8_t checksum = 145; // ???? need to calc this actually;
  int i;
  //fprintf(s19_file, "S1");
  printf("S1%04x", current_emit.initial_lc);
  //fprintf(s19_file, "%04x", current_emit.initial_lc);
  for(i = 0; i < current_emit.index; i++){
    printf("%02x", current_emit.message[i]);
    //fprintf(s19_file, "%02x", current_emit.message[i]);
  }
  //fprintf(s19_file, "%02x\n", checksum);
  printf("%02x\n", checksum);

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
  uint16_t as = 0, sval0= 0, sval1 = 0, ad = 0, dval0 = 0, dval1 = 0;
  uint8_t emit_val;
  if(inst_ptr->type == NONE){ // get it out of the way
    emit(0x00, lc++);
    emit(0x13, lc++);
    return;
  }
  as = as_val[src->mode];
  sval0 = src->val0;
  if(loc_cntr_inc_via_addrmode[src->mode]){
    sval1 = src->val1;
  }
  sval0 = src->val0;
  if(inst_ptr == NULL) printf("sdf\n");

  if(dst != NULL){
    ad = ad_val[dst->mode];
    dval0 = dst->val0;
    if(loc_cntr_inc_via_addrmode[dst->mode]){
      dval1 = dst->val1;
    }
  }
  printf("src mode: %d\n", src->mode);
  switch(inst_ptr->type){
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
      inst2.src = src->val0;
      inst2.ad = ad;
      inst2.bw = inst_ptr->b_w;
      inst2.as = as;
      inst2.dst = dst->val0;
      emit_val = inst2.value & 0xFF;
      emit(emit_val, lc++);
      emit_val = (inst2.value >> 8) & 0xFF;
      emit(emit_val, lc++);
      break;
    case JUMP:
      inst3.value = 0;
      inst3.value |= inst_ptr->opcode;
      inst3.addr = (src->val0 - (location_counter - 2)/2); // using the LC to get this to work
      emit_val = inst3.value & 0xFF;
      emit(emit_val, lc++);
      emit_val = (inst3.value >> 8) & 0xFF;
      emit(emit_val, lc++);
      // need to add more emits for more args, depending on mode
      break;
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
