#ifndef ENUM_TYPES_H
#define ENUM_TYPES_H

// Instructions
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

// Symbols
typedef enum SymbolType{
  REGISTER = 0,
  KNOWN = 1,
  LABELTYPE = 1,
  UNKNOWN = 2,
} SymbolType;

// Tokens
typedef enum TokenType{
  LABEL = 0,
  REG,
  INSTRUCTION,
  DIRECTIVE,
  OPERAND, 
}TokenType; 

typedef enum AddrMode{
  REG_DIRECT = 0,
  INDEX,
  RELATIVE,
  ABSOLUTE,
  INDIRECT,
  INDIRECT_AA,
  IMMEDIATE,
  BAD_ADDR,
}AddrMode;

// Directives
typedef enum DirName{
  ALIGN_D = 0,
  ASCII_D,
  BSS_D,
  BYTE_D,
  END_D,
  EQU_D,
  ORG_D,
  WORD_D
}DirName;

#endif
