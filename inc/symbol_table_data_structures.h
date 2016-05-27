#ifndef SYMBOL_TABLE_DATA_STRUCTURES_H
#define SYMBOL_TABLE_DATA_STRUCTURES_H

#include <stdint.h>
#include "enum_types.h"

#define MAX_SYMBOL_LENGTH     32


typedef struct Symbol{
  char* name;
  uint16_t value;
  SymbolType type;
  struct Symbol* next;
}Symbol;

#endif
