#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

#include "token_data_structures.h"
#include "symbol_table_data_structures.h"
#include "emit.h"
#include "inst_table.h"
#include "dir_table.h"

extern uint16_t location_counter;
extern Inst* inst_table;

extern void parseOperands(char* str, OperandVal* src);
extern void emit(uint8_t value, uint16_t lc);
extern void triggerEmit(uint16_t lc);
extern void emit_I(Inst* inst_ptr, OperandVal* src, OperandVal* dst, uint16_t lc);
extern void initSymbolTable(void);
void emit_test(void);
void emit_t(void);
void triggerEmit_test(void);
void emit_I_test(void);

static uint8_t val1 = 12;
static uint8_t val2 = 0x12;
static uint8_t val3 = 88;
static uint8_t val4 = 0xFF;
static uint8_t val5 = 0xAC;

static char ch1[] = "@R3";
static char ch2[] = "#13";
static char ch3[] = "#$12";
static char ch4[] = "ZERO";

// Inst in1 = inst_table[1]; // add
// Inst in2 = inst_table[30]; // jlo
// Inst in3 = inst_table[40]; // push
// Inst in4 = inst_table[43]; // reti

void emit_test(void)
{
  location_counter = 0;
  initSymbolTable();
  emit_t();
  triggerEmit_test();
  emit_I_test();
}

void emit_t(void)
{
  emit(val1, location_counter);
  emit(val2, location_counter);
  emit(val3, location_counter);
  emit(val4, location_counter);
  emit(val5, location_counter);

}

void triggerEmit_test(void)
{
  emit_t();
  triggerEmit(location_counter + 15);
  printf("lc = %d\n", location_counter);
}

void emit_I_test(void)
{
  
  Inst in1 = {"PUSH", 0x1200, 1, WORD};
  Inst in2 = {"RETI", 0x1300, 0};
  Inst in3 = {"MOV",  0x4000, 2, WORD};
  OperandVal src;
  OperandVal dst;
  parseOperands(ch1, &src); // indirect: @R3
  parseOperands(ch4, &dst); // immediate: #13
  //emit_I(&in1, &src, NULL, location_counter);
  //emit_I(&in2, NULL, NULL, location_counter);
  emit_I(&in3, &src, &dst, location_counter);
  printf("expect: 0x1223, 1300, ; is: \n");
  triggerEmit(location_counter + 10);
  triggerEmit(location_counter);
}

