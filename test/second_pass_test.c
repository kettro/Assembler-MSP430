#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <token_data_structures.h>
#include <symbol_table_data_structures.h>
#include <inst_table.h>
#include <dir_table.h>

extern uint16_t location_counter;
extern FILE* asm_file;
extern FILE* s19_file;

extern int secondPass(void);
extern void firstPass_test(void);

void secondPass_test(void);

void secondPass_test(void)
{
  firstPass_test();
  s19_file = fopen("out.s19", "w");
  asm_file = fopen("test.asm", "r");

  secondPass();
  fclose(s19_file);
  fclose(asm_file);
}
