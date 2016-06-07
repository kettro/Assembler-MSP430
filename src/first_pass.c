// Includes
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
// Defines
#define MAX_LINE_LENGTH     256
// Local Variables
static char loc_cntr_inc_via_addrmode[] = {0 , 2, 2, 2, 0, 0, 2};
int error_count = 0;
// Local Function Prototypes
int firstPass(void);
int isBlank(char* token);
// Extern Variables
extern uint16_t location_counter;
extern FILE* asm_file;
// Extern Function Prototypes
  // inst_table.c
extern int isInst(char* token);
extern void handleInst_1(char* command, char* token_ptr);
  // symbol_table.c
extern int isLabel(char* token);
extern int handleLabel_1(char* labelname, char* operand);
  // dir_table.c
extern int handleDir_1(char* command, char* token_ptr);
extern int isDir(char* token);
// Definitions

/* First Pass
 * Desc: perform the 1st parseOperands
 * Param: FILE* to the infile .asm
 * Return: boolean, if there is an error in the 1st pass
 * Results: produces a symbol table, checks syntax
 */
int firstPass(void)
{
  location_counter = 0;
  char line[MAX_LINE_LENGTH];
  char line_dup[MAX_LINE_LENGTH]; // to preserve the line, for reuse
  char* first_token_ptr; // track the first token
  char* operand_token_ptr; // track the operands
  char nul = '\0';
  int i;
  while(fgets(line, sizeof(line), asm_file)){
    strcpy(line_dup, line);
    first_token_ptr = strtok(line_dup, " \n\t\r;"); // tokenize on space, CR, endline, ; to get tk1
    if(isBlank(first_token_ptr)){ // no entry on the line
      continue;
    }
    operand_token_ptr = strtok(NULL, "\n\r\t"); // tokenize, have the opers and the comments
    if(operand_token_ptr == NULL){ // case where nothing after the operand
      operand_token_ptr = &nul;
    }

    // Check if the 1st token is an instruction
    if(isInst(first_token_ptr)){
      handleInst_1(first_token_ptr, operand_token_ptr);
      // next loop please
      continue;
    }
    // Check if the 1st token is a directive
    else if(isDir(first_token_ptr)){
      if(handleDir_1(first_token_ptr, operand_token_ptr) == 0){ 
        /*is END*/ 
        break;
      }
      // next loop please
      continue;
    }
    // Check if the 1st token is a label (or potential label)
    else if(isLabel(first_token_ptr)){
      // check if the sysmbol is already in the table
      if(handleLabel_1(first_token_ptr, operand_token_ptr) == 0){
        // if handleLabel => 0: found END, end compilation
        break;
      }
      // next loop please
      continue;
    }
  }
  if(error_count != 0){
    return 0;
  }
  return 1;
}

/* isBlank
 * Desc: check if a line is blank
 * Param: a string
 * Return: boolean, is or isn't blank
 */
int isBlank(char* token)
{
  if(token == NULL || *token == '\n' || *token == '\r' || *token == '\0'){ return 1; }
  else { return 0; }
}
