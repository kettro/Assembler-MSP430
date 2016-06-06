#include "dir_table.h"
#include "enum_types.h"
#include "inst_table.h"
#include "symbol_table_data_structures.h"
#include "token_data_structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void firstPass_test(void);

extern FILE* asm_file;
extern void firstPass(void);
extern uint16_t location_counter;
extern void initSymbolTable(void);

void firstPass_test(void)
{
  asm_file = fopen("test.asm", "w");
  fprintf(asm_file, "label1 BYTE 5 ; comment\n"); // label, dir, 1 op, comment
  fprintf(asm_file, "ALIGN\n");
  fprintf(asm_file, "label2 bss 34\n");
  fprintf(asm_file, "swpb label1\n");
  fprintf(asm_file, "mov label1, label2\n"); // NOTE: getting a seg fault due to no commas!!!!!!
  fprintf(asm_file, "mov @r6, r2\n");
  fprintf(asm_file, "words ascii \"thisistext\"\n");
  fprintf(asm_file, "add words(r2), r4\n");
  fprintf(asm_file, "end\n");
  fclose(asm_file);
  
  asm_file = fopen("test.asm", "r");
  initSymbolTable();
  firstPass();
  fclose(asm_file);
  printf("T1: LC- expect: 62, is: %d\n", location_counter);
}
