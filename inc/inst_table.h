#ifndef INST_TABLE_H
#define INST_TABLE_H
#include <stdint.h>
#include "enum_types.h"
#define TOTAL_INSTRUCTIONS    61
#define MAX_INST_NAME_LENGTH  16


typedef struct Inst{
  char name[MAX_INST_NAME_LENGTH];
  uint16_t opcode;
  InstType type;
  BWType b_w;
} Inst;

#endif
