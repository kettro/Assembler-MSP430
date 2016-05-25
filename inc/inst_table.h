#ifndef INST_TABLE_H
#define INST_TABLE_H
#include <stdint.h>
#define TOTAL_INSTRUCTIONS    31
#define MAX_INST_NAME_LENGTH  4

typedef enum InstType{
  NONE = 0,
  ONE,
  TWO,
  JUMP
} InstType;

typedef enum BWType{
  WORD = 0,
  BYTE = 1
}BWType;

typedef struct Inst{
  char name[MAX_INST_NAME_LENGTH];
  uint16_t opcode;
  InstType type;
  BWType b_w;
} Inst;

#endif
