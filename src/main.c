#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "symbol_table_data_structures.h"
#include "inst_table.h"

// Local Defines
// Local Variables
uint16_t location_counter;
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
  testSuite('1'); // Inst table test

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
