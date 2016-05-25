#ifndef SYMBOL_TABLE_DATA_STRUCTURES_H
#define SYMBOL_TABLE_DATA_STRUCTURES_H

#include <stdint.h>

#define MAX_SYMBOL_LENGTH     32

typedef enum Boolean{
  FALSE = 0,
  TRUE = 1,
  NUL = 0,
} Boolean;

typedef enum SymbolType{
  REGISTER = 0,
  KNOWN = 1,
  LABELTYPE = 1,
  UNKNOWN = 2,
} SymbolType;

typedef struct Symbol{
  char* name;
  uint16_t value;
  SymbolType type;
  struct Symbol* next;
}Symbol;

#endif
