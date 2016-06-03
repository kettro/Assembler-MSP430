#include "dir_table.h"
#include "enum_types.h"
#include "inst_table.h"
#include "symbol_table_data_structures.h"
#include "token_data_structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

extern uint16_t location_counter;

extern int isDir(char* token);
extern Dir* getDir(char* token);
extern int handleDir_1(char* command, char* operand);
extern void addSymbol(char* name, uint16_t value, SymbolType type);
extern void initSymbolTable(void);

void dirTable_test(void);
void isDir_test(void);
void getDir_test(void);
void handleDir_1_test(void);

char* test_d_1 = "AliGn";
char* test_d_2 = "BYTE";
char* test_d_3 = "boote";
char* test_d_4 = "equ";
char* test_d_5 = "bss";

char* test_d_op1 = "7";
char* test_d_op2 = "7.";
char* test_d_op3 = "$FF";
char* test_d_op4 = "R8";

char* test_label_1 = "test_label_1";
char* test_label_2 = "test_label_2";


void dirTable_test(void)
{
  initSymbolTable();
  printf("isDir:\n");
  isDir_test();
  printf("getDir_test\n");
  getDir_test();
  printf("handleDir_1_test:\n");
  handleDir_1_test();
}

void isDir_test(void)
{
  int t1 = isDir(test_d_1);
  int t2 = isDir(test_d_2);
  int t3 = isDir(test_d_3);

  printf("T1: expect 1, is %d\n", t1);
  printf("T2: expect 1, is %d\n", t2);
  printf("T3: expect 0, is %d\n", t3);
}

void getDir_test(void)
{
  Dir* t1 = getDir(test_d_1);
  Dir* t2 = getDir(test_d_2);
  
  printf("T1: name- expect: %s, is: %s; enumname- expect %d, is %d\n", test_d_1, t1->name, 0, t1->enum_name);
  printf("T2: name- expect: %s, is: %s; enumname- expect %d, is %d\n", test_d_2, t2->name, 3, t2->enum_name);
}

void handleDir_1_test(void)
{
  location_counter = 0;
  addSymbol(test_label_1, 1, KNOWN);
  addSymbol(test_label_2, 2, UNKNOWN);

  // ALIGN
  location_counter = 1;
  handleDir_1(test_d_1, "");
  printf("T1: LC: exp: 2, is: %d\n", location_counter);
  // BYTE 7
  location_counter = 0;
  handleDir_1(test_d_2, test_d_op1);
  printf("T2: LC: exp: 1, is: %d\n", location_counter);
  // EQU 7
  location_counter = 0;
  handleDir_1(test_d_4, test_d_op1);
  printf("T3: LC: exp: 0, is: %d\n", location_counter);
  // BYTE 7.
  location_counter = 0;
  handleDir_1(test_d_2, test_d_op2);
  printf("T4: LC: exp: 1, is: %d\n", location_counter);
  // Byte $ff
  location_counter = 0;
  handleDir_1(test_d_2, test_d_op3);
  printf("T5: LC: exp: 1, is: %d\n", location_counter);
  // BYTE R8
  location_counter = 0;
  handleDir_1(test_d_2, test_d_op4);
  printf("T6: LC: exp: 1, is: %d\n", location_counter);
  // BYTE test_label_1
  location_counter = 0;
  handleDir_1(test_d_2, test_label_1);
  printf("T7: LC: exp: 1, is: %d\n", location_counter);
  // BYTE test_label_2
  location_counter = 0;
  handleDir_1(test_d_5, test_label_2);
  printf("T8: LC: exp: 0, is: %d\n", location_counter);

  
}

