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
Symbol register_list[] = {
  {"R0", 0, REGISTER, NULL },
  {"PC", 0, REGISTER, NULL },
  {"R1", 1, REGISTER, NULL },
  {"SP", 1, REGISTER, NULL },
  {"R2", 2, REGISTER, NULL },
  {"SR", 2, REGISTER, NULL },
  {"CG1",2, REGISTER, NULL },
  {"R3", 3, REGISTER, NULL },
  {"CG2",3, REGISTER, NULL },
  {"R4", 4, REGISTER, NULL },
  {"R5", 5, REGISTER, NULL },
  {"R6", 6, REGISTER, NULL },
  {"R7", 7, REGISTER, NULL },
  {"R8", 8, REGISTER, NULL },
  {"R9", 9, REGISTER, NULL },
  {"R10",10, REGISTER, NULL },
  {"R11",11, REGISTER, NULL },
  {"R12",12, REGISTER, NULL },
  {"R13",13, REGISTER, NULL },
  {"R14",14, REGISTER, NULL },
  {"R15",15, REGISTER, NULL },
};
// Local Prototypes
void addSymbol(char* name, uint16_t value, SymbolType type);
Symbol* getSymbol(char* name);
void updateSymbol(Symbol* symbol_ptr, uint16_t value);
int findUnknowns(void);
void initSymbolTable(void);
int isLabel(char* token);
int handleLabel_1(char* label, char* operand);
int handleLabel_2(char* label, char* operand);
// Extern Variables
extern uint16_t location_counter;
extern int error_count;
extern FILE* error_file;
// Extern Prototypes
extern int isDir(char* operand);
extern int handleDir_1(char* command, char* operand);
extern int handleDir_2(char* command, char* operand);
extern int parseDirOperand(char* operand, OperandVal* val);
extern int isInst(char* operand);
extern int handleInst_1(char* command, char* operands);
extern int handleInst_2(char* command, char* operands);
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
    error_count++;
    fprintf(error_file, "Error @%d: attempting to add a duplicate entry: %s\n", location_counter, name);
  }
}

Symbol* getSymbol(char* name)
{
  int i;
  char reg_comp[strlen(name)];
  for(i = 0; i < strlen(name); i++){
    reg_comp[i] = toupper(name[i]);
  }
  for(i = 0; i < NUMBER_OF_REGISTERS; i++){
    if(strcmp(reg_comp, register_list[i].name) == 0){
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
  addSymbol("ZERO", 0, KNOWN); // a built-in for the language
}

int isLabel(char* token)
{
  // if isalphanum
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
  if(*operand == '\0'){ // no operand = bare symbol
    updateSymbol(symbol_ptr, location_counter);
    return 1;
  }
  char dup[strlen(operand)];
  strcpy(dup, operand);
  char* command = strtok(dup, " \t\n\r;"); // shouldn't be needed, but being safe
  char* argument = strtok(NULL, "\n\r\t"); // args are the next non null
  char is_equ_str[strlen(command)];
  strcpy(is_equ_str, command);
  int i;
  for(i = 0; i < strlen(is_equ_str); i++){ 
    is_equ_str[i] = toupper(is_equ_str[i]); 
  }
  OperandVal val;
  if(isDir(command)){
    if(strcmp(is_equ_str, "EQU") == 0){
      // is EQU: handle differently than else
      parseDirOperand(argument, &val); // get the values of the operands
      if(val.type1 == UNKNOWN){
        error_count++;
        fprintf(error_file, "Error @%d: EQU must take only KNOWN labels\n", location_counter);
        //error- must be known for EQU
        return 1; 
      }
      updateSymbol(symbol_ptr, val.val0);
      return 1; // update and we're done. no inc of LC
    }else{
      // standard directive; add the location counter, and then handle the directive
      updateSymbol(symbol_ptr, location_counter);
      if(handleDir_1(command, argument) == 0){
        // END encountered
        return 0;
      }
      return 1; 
    }
  }else if(isInst(command)){
    updateSymbol(symbol_ptr, location_counter);
    handleInst_1(command, argument);
    return 1;
  }
  else{
    error_count++;
    fprintf(error_file, "Error @%x: Labels must be followed by an inst or dir\n", location_counter);
    // error; must be either isnt or dir next
    return 1;
  }
}


int handleLabel_2(char* label, char* operand)
{
  // handle labels in 2nd pass: aka, skip.
  if(*operand == '\0'){
    return 1;
  }
  char dup[strlen(operand)];
  strcpy(dup, operand);
  Symbol* symbol_ptr = getSymbol(label);
  if(symbol_ptr->value != location_counter){
    fprintf(error_file, "Warning @%x: an initial Label's value must match the LC\n", location_counter);
    // error= a initial label must match the lc! 
  }
  char* command = strtok(dup, " \n\r\t");
  char* argument = strtok(NULL, "\n\r\t");
  if(isInst(command)){
    handleInst_2(command, argument);
  }else if(isDir(command)){
    if(handleDir_2(command, argument) == 0){
      // is end:
      return 0;
    }
  }else{
    error_count++;
    fprintf(error_file, "Error @ 2nd pass: label not followed by dir or inst\n");
    // error;
    return 1;
  }
  return 1;
}

