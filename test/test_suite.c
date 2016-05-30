#include "dir_table.h"
#include "enum_types.h"
#include "inst_table.h"
#include "symbol_table_data_structures.h"
#include "token_data_structures.h"
void testSuite(char test_to_run);

extern void symbolTable_test(void);
extern void instTable_test(void);
extern void dirTable_test(void);
extern void firstPass_test(void);

void testSuite(char test_to_run)
{
  switch(test_to_run){
    case 's': // Symbol Table
    case 'S':
      symbolTable_test();
      break;
    case 'i': // Inst Table
    case 'I':
      instTable_test();
      break;
    case 'd': // Dir Table
    case 'D':
      dirTable_test();
      break;
    case '1': // First Pass
    case 1:   // ""
      firstPass_test();
      break;
    case '2': // Second Pass
    case 2:   // ""
      break;
  }
}
