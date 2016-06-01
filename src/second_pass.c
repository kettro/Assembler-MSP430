// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Defines
#define MAX_LINE_LENGTH   256
// Local Variables
static char loc_cntr_inc_via_addrmode[] = {0,2,2,2,2,0,0,2};
// Local Function Prototypes
int secondPass(char* infilename, char* outfilename);
// Extern Variables
extern uint16_t location_counter;
// Extern Function Prototypes
extern int isBlank(char* token);
extern int isDir(char* token);
extern int isInst(char* token);
extern int isLabel(char* token);
// Definitions

// Purpose: Generate Instructions from Records
// Produce S-Records
// Input: ST, IT, DT, .asm file
// Output .s19 file

// get line from file;
// Label: skip to next token
// Inst: 
//  get LC, and Inst type
//  handleInst_2 => get operand(s), generate data to emit
//  build emit args, inc LC appropriately
//  emit
//
// Dir:
//  get LC, and Dir Type
//  handleDir_2
//  emit as needed;

int secondPass(char* infilename, char* outfilename)
{
  FILE* infile = fopen(infilename, "r");
  FILE* outfile = fopen(outfilename, "w");
  char line[MAX_LINE_LENGTH];
  char dup[MAX_LINE_LENGTH];

  char* first_token_ptr;
  char* operand_token_ptr;
  char nul = '\0';
  int i = 0;
  location_counter = 0;

  while(fgets(line, sizeof(line), infile)){
    printf("LC: %d\n", location_counter);
    strcpy(dup, line);
    first_token_ptr = strtok(dup, " ;");
    first_token_ptr = strtok(NULL, " \n\t\r");
    if(isBlank(first_token_ptr)){
      continue;
    }
    operand_token_ptr = strtok(NULL, ";\n\r");
    if(operand_token_ptr == NULL){
      operand_token_ptr = &nul;
    }
    if(isInst(first_token_ptr)){
      // handleInst_2();
    }
    else if(isDir(first_token_ptr)){
      // handleDir_2();
    }
    else if(isLabel(first_token_ptr)){
      // handleLabel_2() => is just a front for handleInst_2 and handledir_2
    }
  }
}
