#include "dir_table.h"
#include "enum_types.h"
#include "inst_table.h"
#include "symbol_table_data_structures.h"
#include "token_data_structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void firstPass_test(void);

extern void firstPass(char* filename);;
extern uint16_t location_counter;
extern void initSymbolTable(void);

void firstPass_test(void)
{
  char* filename = "test.asm";

  FILE* file = fopen(filename, "w");
  fprintf(file, "label1 BYTE 5 ; comment\n"); // label, dir, 1 op, comment
  fprintf(file, "ALIGN\n");
  fprintf(file, "label2 bss 34\n");
  fprintf(file, "swpb label1\n");
  fprintf(file, "mov label1, label2\n"); // NOTE: getting a seg fault due to no commas!!!!!!
  fprintf(file, "mov @r6, r2\n");
  fprintf(file, "words ascii \"thisistext\"\n");
  fprintf(file, "add words(r2), r4\n");
  fprintf(file, "end\n");
  fclose(file);
  
  initSymbolTable();
  firstPass(filename);

  printf("T1: LC- expect: 62, is: %d\n", location_counter);
}
