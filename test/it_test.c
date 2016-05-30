#include "dir_table.h"
#include "enum_types.h"
#include "inst_table.h"
#include "symbol_table_data_structures.h"
#include "token_data_structures.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


extern uint16_t location_counter;
void instTable_test();
void isInst_test(void);
void getInst_test(void);
void handleInst_1_test(void);
extern int isInst(char* token);
extern Inst* getInst(char* token);
extern void handleInst_1(char* command, char* operands);
extern void initSymbolTable(void);
extern void addSymbol(char* name, uint16_t value, SymbolType type);

char* inst_1 = "xor";
char* inst_2 = "XoR";
char* inst_3 = "xnor";
char* inst_4 = "XOR.B";
char* inst_5 = "XOR.v";
char* inst_6 = "xor.w";
char* inst_7 = "jlo";
char* inst_8 = "swpb";
char* inst_9 = "reti";

char* label_1 = "label_1";

char* op_1 = "label_1";     // relative
char* op_2 = "#3";          // immediate
char* op_3 = "&label_1";    // absolute
char* op_4 = "R12";         // Reg_direct
char* op_5 = "label_1(R1)"; // indexed
char* op_6 = "@label_1";    // indirect, but bad
char* op_7 = "@R5+";        // indirect_aa
char* op_8 = "@R5";         // indirect, good

char* op_11 = "label_1, label_1"; // => good
char* op_22 = "#3, #3"; // => error
char* op_23 = "#3, &label_1"; // => good
char* op_44 = "R12, R10"; // => good
char* op_61 = "@label_1, label_1"; // => good


void instTable_test(void)
{
  // printf("Tests:\n%s\n%s\n%s\n%s\n%s\n%s\n",inst_1,inst_2,inst_3,inst_4,inst_5,inst_6);
  // printf("%s\n%s\n%s\n", inst_7, inst_8, inst_9);

  // printf("isInst test\n");
  // isInst_test();
  // printf("getInst_test\n");
  // getInst_test();
  printf("handleInst_1\n");
  handleInst_1_test();
}

void isInst_test(void)
{
  int t1 = isInst(inst_1);
  int t2 = isInst(inst_2);
  int t3 = isInst(inst_3);
  int t4 = isInst(inst_4);
  int t5 = isInst(inst_5);

  printf("T1: expect 1, is: %d\n", t1);
  printf("T2: expect 1, is: %d\n", t2);
  printf("T3: expect 0, is: %d\n", t3);
  printf("T4: expect 1, is: %d\n", t4);
  printf("T5: expect 0, is: %d\n", t5);
}

void getInst_test(void)
{
  printf("Testing BW, type, test 1,4,6,7,8,9\n");
  Inst* t1 = getInst(inst_1);
  Inst* t4 = getInst(inst_4);
  Inst* t6 = getInst(inst_6);
  Inst* t7 = getInst(inst_7);
  Inst* t8 = getInst(inst_8);
  Inst* t9 = getInst(inst_9);
  
  printf("T1: BW- expect: 0, is: %d; type- expect: 2, is: %d\n", t1->b_w, t1->type);
  printf("T4: BW- expect: 1, is: %d; type- expect: 2, is: %d\n", t4->b_w, t4->type);
  printf("T6: BW- expect: 0, is: %d; type- expect: 2, is: %d\n", t6->b_w, t6->type);
  printf("T7: BW- expect: 0, is: %d; type- expect: 3, is: %d\n", t7->b_w, t7->type);
  printf("T8: BW- expect: 0, is: %d; type- expect: 1, is: %d\n", t8->b_w, t8->type);
  printf("T9: BW- expect: 0, is: %d; type- expect: 0, is: %d\n", t9->b_w, t9->type);
}

void handleInst_1_test(void)
{
  initSymbolTable();
  addSymbol(op_1, 1, KNOWN);
  // testing the handling of inst statements
  // xor
  location_counter = 0;
  handleInst_1(inst_1, op_11);
  printf("T1: %s: LC- expect 6, is %d\n", inst_1, location_counter);

  location_counter = 0;
  handleInst_1(inst_1, op_22);
  printf("T2: %s: LC- expect 0 + error, is %d\n", inst_1, location_counter);
  // jlo - type3
  location_counter = 0;
  handleInst_1(inst_7, op_1);
  printf("T3: %s: LC- expect 4, is %d\n", inst_7, location_counter);
  // swpb - type1
  location_counter = 0;
  handleInst_1(inst_8, op_6);
  printf("T4: %s: LC- expect 0, => error, is %d\n", inst_8, location_counter);

  location_counter = 0;
  handleInst_1(inst_8, op_8);
  printf("T5: %s: LC- expect 2, is %d\n", inst_8, location_counter);

  location_counter = 0;
  handleInst_1(inst_8, op_7);
  printf("T5: %s: LC- expect 2, is %d\n", inst_8, location_counter);
}

