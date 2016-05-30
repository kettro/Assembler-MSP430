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

int main(void)
{
  testSuite('i'); // Inst table test
  // initSymbolTable(); // add the required elements to the ST

  // Symbol* symbol;
  // addSymbol("a_Symbol", 99, KNOWN);
  // addSymbol("not_a_name", 34, UNKNOWN);
  // symbol = getSymbol("a_Symbol");
  // if(symbol == NULL){ printf("SHIT\n"); }
  // printf("symbol name: %d\n", symbol->value);

  return 0;
}
