// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Defines
#define MAX_LINE_LENGTH   256
// Local Variables
//static char loc_cntr_inc_via_addrmode[] = {0,2,2,2,2,0,0,2};
// Local Function Prototypes
int secondPass(void);
// Extern Variables
extern uint16_t location_counter;
extern FILE* asm_file;
// Extern Function Prototypes
extern int isBlank(char* token);
extern int isDir(char* token);
extern int isInst(char* token);
extern int isLabel(char* token);
extern void handleInst_2(char* command, char* operands);
extern int handleDir_2(char* command, char* operands);
extern int handleLabel_2(char* label, char* operand);
// Definitions

/* Second Pass
 * Desc: perform the 2nd pass
 * Param: .asm file, symbol table
 * Return: none
 * Results: produces .s19 file
 */
int secondPass(void)
{
  char line[MAX_LINE_LENGTH];
  char dup[MAX_LINE_LENGTH];

  char* first_token_ptr;
  char* operand_token_ptr;
  char nul = '\0';
  location_counter = 0;

  while(fgets(line, sizeof(line), asm_file)){
    strcpy(dup, line);
    first_token_ptr = strtok(dup, " \n\r\t");
    if(isBlank(first_token_ptr)){
      continue;
    }
    operand_token_ptr = strtok(NULL, "\n\r\t");
    if(operand_token_ptr == NULL){
      operand_token_ptr = &nul;
    }
    if(isInst(first_token_ptr)){
      handleInst_2(first_token_ptr, operand_token_ptr);
      continue;
    }
    else if(isDir(first_token_ptr)){
      handleDir_2(first_token_ptr, operand_token_ptr);
      continue;
    }
    else if(isLabel(first_token_ptr)){
      handleLabel_2(first_token_ptr, operand_token_ptr);
      continue;
    }
  }
  return 1;
}
