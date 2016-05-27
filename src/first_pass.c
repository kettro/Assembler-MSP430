// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "symbol_table_data_structures.h"
#include "token_data_structures.h"
#include "inst_table.h"
#include "dir_table.h"
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
extern void handleInst_1(Inst* inst_ptr, char* token_ptr);
  // symbol_table.c
extern Symbol* getSymbol(char* name);
extern void addSymbol(char* name, uint16_t value, SymbolType type);
extern int findUnknowns(Symbol* unknown_array[]);
extern int isLabel(char* token);
  // both_passes.c
extern int parseOperands(char* operand, OperandVal* val);
  // dir_table.c
extern Dir* getDir(char* token);
extern int handleDir_1(Dir* dir_ptr, char* token_ptr);
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
  char line_dup[MAX_LINE_LENGTH];
  char* first_token_ptr;
  char* operand_token_ptr;
  Inst* inst_ptr;
  Symbol* symbol_ptr;
  OperandVal* operand_ptr;
  Dir* dir_ptr;
  int i;
  while(fgets(line, sizeof(line), infile_ptr)){
    strcpy(line_dup, line);
    first_token_ptr = strtok(line_dup, " \n\r;"); // tokenize on space, CR, endline, ; to get tk1
    if(isBlank(first_token_ptr)){ // no entry on the line
      continue;
    }
    operand_token_ptr = first_token_ptr;
    // find the next non-null;
    for(i = 0; *operand_token_ptr == '\0' && i < 5; i++){ operand_token_ptr++; } 

    if(isInst(first_token_ptr)){
      if((inst_ptr = getInst(first_token_ptr)) == NULL){
        break; // error
      }
      handleInst_1(inst_ptr, operand_token_ptr); // point to the first non_null;
      continue;
    }else if(isDir(first_token_ptr)){
      // handle dir
       if((dir_ptr = getDir(first_token_ptr)) == NULL){
         break;
       }
       if(handleDir_1(dir_ptr, operand_token_ptr) == 0){ // if reads END
         return 1;
       }
       continue;
    }else if(isLabel(first_token_ptr)){
      // handle Label
      // if((symbol_ptr = getSymbol(line_tok_ptr[1])) == NULL){
      //   break;
      // }
    }
  }
  return 1;
}

int isBlank(char* token)
{
  if(token == NULL || *token == '\n' || *token == '\r'){ return 1; }
  else { return 0; }
}
