#include "dir_table.h"
#include "enum_types.h"
#include "inst_table.h"
#include "symbol_table_data_structures.h"
#include "token_data_structures.h"

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void symbolTable_test(void);
void isLabel_test(void);
void findUnknowns_test(void);
void addSymbol_test(void);
void updateSymbol_test(void);

extern void initSymbolTable(void);
extern void addSymbol(char* name, uint16_t value, SymbolType type);
extern Symbol* getSymbol(char* name);
extern int findUnknowns(void);
extern void updateSymbol(Symbol* symbol_ptr, uint16_t value);
extern int isLabel(char* token);


char test_symbol_1[] = "test_symbol_1";
char test_symbol_2[] = "$test_symbol_2";
char test_symbol_3[] = "";
uint16_t test_val_1 = 15;
uint16_t test_val_2 = 0xFFFF;
uint16_t test_val_3 = 0;


void symbolTable_test(void)
{
  // purpose is to test aspects of the symbol table
  // getting, setting
  // updating
  // find unknowns
  // isLabel?
  initSymbolTable();

  printf("Test symbols: \n1: %s\n2: %s\n3: %s\n", test_symbol_1, test_symbol_2, test_symbol_3);
  printf("\nisLabel test: \n");
  isLabel_test();
  printf("\naddSymbol test:\n");
  addSymbol_test();
  printf("\nfindUnknowns test:\n");
  findUnknowns_test();
  printf("\nupdateSymbol test:\n");
  updateSymbol_test();
}

// Test the detection for the isLabel metric
void isLabel_test(void)
{
  int t1 = isLabel(test_symbol_1);
  int t2 = isLabel(test_symbol_2);
  int t3 = isLabel(test_symbol_3);

  printf("T1: expect 1, is: %d\n", t1);
  printf("T2: expect 0, is: %d\n", t2);
  printf("T3: expect 0, is: %d\n", t3);
}

void findUnknowns_test(void)
{
  // to be run after addSymbol_test
  int unknown_number = findUnknowns();
  printf("T: expect 1, is %d\n", unknown_number);
}

void addSymbol_test(void)
{
  addSymbol(test_symbol_1, test_val_1, KNOWN);
  addSymbol(test_symbol_2, test_val_2, UNKNOWN);
  addSymbol(test_symbol_3, test_val_3, REGISTER);

  Symbol* t1 = getSymbol(test_symbol_1);
  Symbol* t2 = getSymbol(test_symbol_2);
  Symbol* t3 = getSymbol(test_symbol_3);
  
  printf("T1: expect n = %s, v = %d; is: n = %s, v = %d\n", test_symbol_1, test_val_1, t1->name, t1->value);
  printf("T2: expect n = %s, v = %d; is: n = %s, v = %d\n", test_symbol_2, test_val_2, t2->name, t2->value);
  printf("T3: expect n = %s, v = %d; is: n = %s, v = %d\n", test_symbol_3, test_val_3, t3->name, t3->value);
}

void updateSymbol_test(void)
{
  Symbol* t1 = getSymbol(test_symbol_1);
  Symbol* t2 = getSymbol(test_symbol_2);
  Symbol* t3 = getSymbol(test_symbol_3);

  int new_val_1 = ~(test_val_1);
  int new_val_2 = ~(test_val_2);
  int new_val_3 = ~(test_val_3);
  updateSymbol(t1, new_val_1);
  updateSymbol(t2, new_val_2);
  updateSymbol(t3, new_val_3);

  Symbol* t1_new = getSymbol(test_symbol_1);
  Symbol* t2_new = getSymbol(test_symbol_2);
  Symbol* t3_new = getSymbol(test_symbol_3);
  
  printf("T1: name- expect %s, is: %s; value- expect: %d, is: %d; state: expect: %d, is: %d\n", t1->name, t1_new->name, t1->value, t1->value, t1->type, t1_new->type);
  printf("T2: name- expect %s, is: %s; value- expect: %d, is: %d; state: expect: %d, is: %d\n", t2->name, t2_new->name, t2->value, t2->value, t2->type, t2_new->type);
  printf("T3: name- expect %s, is: %s; value- expect: %d, is: %d; state: expect: %d, is: %d\n", t3->name, t3_new->name, t3->value, t3->value, t3->type, t3_new->type);
}
