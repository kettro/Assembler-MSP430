// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table_data_structures.h"
#include <unistd.h>
#include <sys/stat.h>
// Defines
#define MAX_LINE_LENGTH     256
// Local Variables
// Local Function Prototypes
int first_pass(int in_file_desc);
int isBlank(char* token);
int isComment(char* token);
int isLabel(char* token);
int isDir(char* token);
// Extern Variables
// Extern Function Prototypes
extern int isInst(char* token);
extern Symbol* getSymbol(char* name);
extern void addSymbol(char* name, uint16_t value, SymbolType type);
extern int findUnknowns(Symbol* unknown_array[]);
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
  char* line_tok_ptr;
  char* tokens[6];
  int i = 0;
  while(fgets(line, sizeof(line), infile_ptr)){
    line_tok_ptr = strtok(line, ";");
    if(line_tok_ptr == NULL){ break; }
    tokens[0] = strtok(line_tok_ptr, " \n\r"); // tokenize on space, and endline
    if(isBlank(tokens[0])){
      continue;
    }else if(isLabel(tokens[0])){
      
      // handle Label: add to st
    }else if(isInst(tokens[0])){
      // handle inst: look for operands, 
    }else if(isDir(tokens[0])){
      // handle dir
    }
    tokens[i] = strtok(NULL, " \n\r");
  }
  // get the file imported via the file_desc
  // get the first line
  // tokenize the first line
  // loop; parse each term.
  // per term: isBlank? isComment? is Label? isInst? isDir?
  // break on line end
  // break on error
}

int isBlank(char* token)
{
}

int isComment(char* token){}

int isLabel(char* token){}

int isDir(char* token){}

