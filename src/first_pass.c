// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "symbol_table_data_structures.h"
#include "token_data_structures.h"
#include "inst_table.h"
// Defines
#define MAX_LINE_LENGTH     256
// Local Variables
char loc_cntr_inc_via_addrmode[] = {0 , 2, 2, 2, 0, 0, 2};
// Local Function Prototypes
int first_pass(int in_file_desc);
int isBlank(char* token);
// Extern Variables
// Extern Function Prototypes
  // inst_table.c
extern int isInst(char* token);
extern Inst* getInst(char*token);
extern void handleInst(Inst* inst_ptr, char* token_ptr);
  // symbol_table.c
extern Symbol* getSymbol(char* name);
extern void addSymbol(char* name, uint16_t value, SymbolType type);
extern int findUnknowns(Symbol* unknown_array[]);
extern int isLabel(char* token);
  // both_passes.c
extern int parseOperands(char* operand, OperandVal* val);
  // dir_table.c
//extern Dir* getDir(char* token);
//extern void handleDir(Dir* dir_ptr, char* token_ptr);
extern int isDir(char* token);
// Definitions

// Output is a Symbol Table
// checks to make:
//    > unknown ids @ end?
//    > per inst & dir: proper operand #'s?
//  > If errors: output to stderr, end compilation

// On parse:
//  if addSymbol() => Unknown:
//    > set unknown = known
//    > update with LC/value
//  if addSymbol() => Unknown:
//    > value = linenumber, unless value != 0;
//  determine if symbol is unknown:
//    > if at start of line => Known
//    > if operand => Unknown
//    > if EQU => Known

int first_pass(int in_file_desc)
{
  FILE* infile_ptr = fdopen(in_file_desc,"r");
  char line[MAX_LINE_LENGTH];
  char* line_tok_ptr[2];
  Inst* inst_ptr;
  Symbol* symbol_ptr;
  OperandVal* operand_ptr;
  //Dir* dir_ptr;
  int i;
  while(fgets(line, sizeof(line), infile_ptr)){
    line_tok_ptr[0] = strtok(line, ";"); // tokenize the comments out
    if(line_tok_ptr[0] == NULL){ break; }
    line_tok_ptr[1] = strtok(line_tok_ptr[0], " \n\r"); // tokenize on space, and endline
    if(isBlank(line_tok_ptr[0])){
      continue;
    }
    for(i = 0; *(line_tok_ptr[0]) == '\0' && i < 4; i++){ line_tok_ptr[0]++; } // find the next non-null;
    if(isInst(line_tok_ptr[1])){
      if((inst_ptr = getInst(line_tok_ptr[1])) == NULL){
        break; // error
      }
      handleInst(inst_ptr, line_tok_ptr[0]); // point to the first non_null;
      continue;
    }else if(isDir(line_tok_ptr[1])){
      // handle dir
      // if((dir_ptr = getDir(line_tok_ptr[1])) == NULL){
      //   break;
      // }
      // handledir(dir_ptr, line_tok_ptr);
      // continue;
    }else if(isLabel(line_tok_ptr[1])){
      // handle Label
      // if((symbol_ptr = getSymbol(line_tok_ptr[1])) == NULL){
      //   break;
      // }
    }
  }
}

int isBlank(char* token)
{
  if(token == NULL || *token == '\n' || *token == '\r'){ return 1; }
  else { return 0; }
}
