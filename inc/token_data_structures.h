#ifndef TOKEN_DATA_STRUCTURES_H 
#define TOKEN_DATA_STRUCTURES_H

#include <stdint.h>
#include "symbol_table_data_structures.h"
#include "enum_types.h"

typedef struct OperandVal{
  AddrMode mode;
  uint16_t val0;
  SymbolType type0;
  uint16_t val1;
  SymbolType type1;  
}OperandVal;

typedef union Type1Inst{
  struct{
    unsigned reg: 4;
    unsigned as: 2;
    unsigned bw: 1;
    unsigned opc: 9;
  };
  uint16_t value;
}Type1Inst;

typedef union Type2Inst{
  struct{
    unsigned dst: 4;
    unsigned as: 2;
    unsigned bw: 1;
    unsigned ad: 1;
    unsigned src: 4;
    unsigned opc: 4;
  };
  uint16_t value;
}Type2Inst;

typedef union Type3Inst{
  struct{
    signed addr: 10;
    unsigned opcode: 6;
  };
  uint16_t value;
}Type3Inst;

#endif
