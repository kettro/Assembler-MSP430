#ifndef EMIT_H
#define EMIT_H

#include "enum_types.h"

typedef struct Emit{
  uint8_t message[32];
  uint8_t index;
  uint16_t initial_lc;
  uint16_t checksum;
}Emit;

#endif
