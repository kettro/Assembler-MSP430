#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "symbol_table_data_structures.h"
#include "token_data_structures.h"
#include "inst_table.h"
#include "dir_table.h"

// Local Defines
#define NUMBER_OF_REGISTERS     21
// Local Variables
uint16_t table_length = 0;
Symbol Root;
Symbol register_list[NUMBER_OF_REGISTERS];
// Local Prototypes
void addSymbol(char* name, uint16_t value, SymbolType type);
Symbol* getSymbol(char* name);
void updateSymbol(Symbol* symbol_ptr, uint16_t value);
int findUnknowns(void);
void initSymbolTable(void);
int isLabel(char* token);
int handleLabel_1(char* label, char* operand);
int handleLabel_2(Symbol* symbol_ptr, char* operand);
// Extern Variables
extern uint16_t location_counter;
// Extern Prototypes
extern int isDir(char* operand);
extern int handleDir_1(char* command, char* operand);
extern int parseDirOperand(char* operand, OperandVal* val);
extern int isInst(char* operand);
extern int handleInst_1(char* command, char* operands);
// Definitions

void addSymbol(char* name, uint16_t value, SymbolType type)
{

  if(getSymbol(name) == NULL){ // new symbol
    Symbol* new_symb = malloc(1 * sizeof(Symbol));
    new_symb->name = malloc(1 * strlen(name));
    strcpy(new_symb->name, name);
    new_symb->type = type;
    new_symb->value = value;
    new_symb->next = Root.next;
    Root.next = new_symb;
    table_length++;
  } else{ // not a new symbol: error
    fprintf(stderr, "Error: attempting to add a duplicate entry: %s\n", name);
  }
}

Symbol* getSymbol(char* name)
{
  int i;
  for(i = 0; i < NUMBER_OF_REGISTERS; i++){
    if(strcmp(name, register_list[i].name) == 0){
      return &(register_list[i]);
    }
  }
  Symbol* temp;
  temp = Root.next;
  while(temp){
    if(strcmp(temp->name, name) == 0){ return temp; }
    else{ temp = temp->next; }
  }
  return NULL;
}

void updateSymbol(Symbol* symbol_ptr, uint16_t value)
{
  if(symbol_ptr->type == REGISTER){ return; } // no updating registers
  symbol_ptr->value = value;
  symbol_ptr->type = KNOWN;
}

int findUnknowns(void)
{
  Symbol* temp = Root.next;
  int i = 0;
  while(temp != NULL){
    if(temp->type == UNKNOWN){
      i++;
      // error
      printf("Unknown found for symbol %s\n", temp->name);
    }
    temp = temp->next;
  }
  return i;
}

/* Initialize the Register List, and the Root
 * Params: none
 * Return: none
 * Result: Root established, register_list populated
 */
void initSymbolTable(void)
{
  Root.name = NULL;
  Root.next = NULL;
  Root.value = 0;
  int i;
  for(i = 0; i < NUMBER_OF_REGISTERS; i++){
    register_list[i].type = REGISTER;
    register_list[i].next = NULL;
    register_list[i].name = malloc(4 * sizeof(char));
  }
  strcpy(register_list[0].name, "R0");
  register_list[0].value = 0;
  strcpy(register_list[1].name, "PC");
  register_list[1].value = 0;
  strcpy(register_list[2].name, "R1");
  register_list[2].value = 1;
  strcpy(register_list[3].name, "SP");
  register_list[3].value = 1;
  strcpy(register_list[4].name, "R2");
  register_list[4].value = 2;
  strcpy(register_list[5].name, "SR");
  register_list[5].value = 2;
  strcpy(register_list[6].name, "CG1");
  register_list[6].value = 2;
  strcpy(register_list[7].name, "R3");
  register_list[7].value = 3;
  strcpy(register_list[8].name, "CG2");
  register_list[8].value = 3;
  strcpy(register_list[9].name, "R4");
  register_list[9].value = 4;
  strcpy(register_list[10].name, "R5");
  register_list[10].value = 5;
  strcpy(register_list[11].name, "R6");
  register_list[11].value = 6;
  strcpy(register_list[12].name, "R7");
  register_list[12].value = 7;
  strcpy(register_list[13].name, "R8");
  register_list[13].value = 8;
  strcpy(register_list[14].name, "R9");
  register_list[14].value = 9;
  strcpy(register_list[15].name, "R10");
  register_list[15].value = 10;
  strcpy(register_list[16].name, "R11");
  register_list[16].value = 11;
  strcpy(register_list[17].name, "R12");
  register_list[17].value = 12;
  strcpy(register_list[18].name, "R13");
  register_list[18].value = 13;
  strcpy(register_list[19].name, "R14");
  register_list[19].value = 14;
  strcpy(register_list[20].name, "R15");
  register_list[20].value = 15;
}

int isLabel(char* token)
{
  // if isalphanum
  Symbol* symbol_ptr;
  if(isalpha(*token)){ // can't be a number
    // could be a label: not muxh else it could be
    return 1;
  }
  return 0;
}

int handleLabel_1(char* label, char* operand)
{
  // handle labels in 1st pass
  // need to also handle EQU, as well as adding things to the ST
  // get next token (not actually an operand - should be inst or dir)
  Symbol* symbol_ptr = getSymbol(label);
  if(symbol_ptr == NULL){
    addSymbol(label, 0, UNKNOWN); // add the symbol as an unknown if not found initially
    symbol_ptr = getSymbol(label);
  }
  char dup[strlen(operand)];
  strcpy(dup, operand);
  uint16_t value;
  uint16_t temp_lc = location_counter;
  char* command = strtok(dup, " \n\r;");
  char* argument = strtok(NULL, "\n\r;"); // args are the next non null
  char is_equ_str[strlen(command)];
  strcpy(is_equ_str, command);
  int i;
  for(i = 0; i < strlen(is_equ_str); i++){ 
    is_equ_str[i] = toupper(is_equ_str[i]); 
  }
  OperandVal val;
  if(isDir(command)){
    if(strcmp(command, "EQU") == 0){
      // is EQU: handle differently than else
      parseDirOperand(argument, &val); // get the values of the operands
      if(val.type1 == UNKNOWN){
        //error- must be known for EQU
        return 1; 
      }
      updateSymbol(symbol_ptr, val.val0);
      return 1; // update and we're done. no inc of LC
    }else{
      // standard directive; add the location counter, and then handle the directive
      updateSymbol(symbol_ptr, temp_lc);
      if(handleDir_1(command, argument) == 0){
        // END encountered
        return 0;
      }
      return 1; 
    }
  }else if(isInst(command)){

  }
  else{
    // error; must be either isnt or dir next

  }

  return 1;
}

/*
int handleLabel_2(Symbol* symbol_ptr, char* operand)
{
  if(location_counter != symbol_ptr->value){ return 0;} // error: someone fucked up
  return 1;
  // handle labels in 2nd pass: aka, skip.
}
*/
