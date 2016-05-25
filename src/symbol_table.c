#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "symbol_table_data_structures.h"

// Local Defines
#define NUMBER_OF_REGISTERS     21
// Local Variables
uint16_t table_length = 0;
Symbol Root;
Symbol register_list[NUMBER_OF_REGISTERS];
// Local Prototypes
void addSymbol(char* name, uint16_t value, SymbolType type);
Symbol* getSymbol(char* name);
int findUnknowns(Symbol* unknown_array[]);
void initSymbolTable(void);
// Extern Variables
// Extern Prototypes
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
    // 
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

int findUnknowns(Symbol* unknown_array[])
{
  Symbol* temp = Root.next;
  int i = 0;
  while(temp != NULL){
    if(temp->type == UNKNOWN){
      unknown_array[i] = temp;
      i++;
    }
    temp = temp->next;
  }
  if(temp == NULL){ return 0; }
  else{ return 1; }
}

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
