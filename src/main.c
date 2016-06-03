#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "symbol_table_data_structures.h"
#include "inst_table.h"

// Local Defines
// Local Variables
uint16_t location_counter;
FILE* s19_file;
FILE* asm_file;
// Local Prototypes
// Extern Variables
// Extern Prototypes
extern Symbol* getSymbol(char* name);
extern void addSymbol(char* name, uint16_t value, SymbolType type);
extern int findUnknowns(Symbol* unknown_array[]);
extern void initSymbolTable(void);

// tests:
extern void testSuite(char test);
extern void symbolTableTest(void);
// Definitions

int main(int argc, char* argv[])
{
  //s19_file = fopen("out.s19", "w");
  //asm_file = fopen("test.asm", "r");
  testSuite('e'); // Inst table test
  //fclose(s19_file);
  //fclose(asm_file);

  // initSymbolTable();
  // => get filename
  // firstPass(filename);
  // if(findUnknowns() == 0){
  //   error: unknowns!!!
  // }
  // => get .s19 filename
  // secondPass(s19_file);
  //
  return 0;
}
