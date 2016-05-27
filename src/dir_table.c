// Includes
#include "dir_table.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
// Defines
#define TOTAL_DIRS    8
// Local Variables
Dir dir_table[] = {
  {"ALIGN", ALIGN_D},
  {"ASCII", ASCII_D},
  {"BSS", BSS_D},
  {"BYTE",BYTE_D},
  {"END", END_D},
  {"EQU", EQU_D},
  {"ORG", ORG_D},
  {"WORD", WORD_D},
};
// Local Function Prototypes
int isDir(char* token);
Dir* getDir(char* token);
int handleDir_1(Dir* dir_ptr, char* operand);
int handleDir_2(Dir* dir_ptr, char* operand);
// Extern Variables
extern uint16_t location_counter;
// Extern Function Prototypes
extern void emit(uint16_t lc, uint8_t value); // defined in an srec generator file
extern void trigger_emit(uint16_t lc);        // "", sends the emit file if >32, or if triggered
// Definitions

int isDir(char* token)
{
  int i;
  char dup[strlen(token)];
  strcpy(dup, token);
  for(i = 0; i < strlen(dup); i++){
    toupper(dup[i]);
  }

  for(i = 0; i < TOTAL_DIRS; i++){
    if(strcmp(dup, dir_table[i].name) == 0){
      return 1;
    }
    return 0;
  }
}

Dir* getDir(char* token)
{
  int i;
  char dup[strlen(token)];
  strcpy(dup, token);

  for(i = 0; i < strlen(dup); i++){
    toupper(dup[i]);
  }
  for(i = 0; i < TOTAL_DIRS; i++){
    if(strcmp(dup, dir_table[i].name) == 0){
      return &(dir_table[i]);
    }
    return NULL;
  }
}

// Handling 1st Pass Directives
int handleDir_1(Dir* dir_ptr, char* operand)
{
  char dup[strlen(operand)];
  char* op_ptr;
  strcpy(dup, operand);
  uint16_t value = 0;
  DirName name = dir_ptr->enum_name;
  switch(name){
    case END_D:
      // stop compilation
      return 0;
    case EQU_D:
      // error
      return 1;
    case ALIGN_D:
      location_counter += location_counter % 2;
      return 1;
    case ASCII_D:
      // operand is a string: get the number of characters in string
      strtok(dup, "\""); // tokenize of the first "
      op_ptr = strtok(NULL, "\""); // tokenize now on the last ", to get the string
      value = strlen(op_ptr);
      break;
    default:
      value = strtol(dup, NULL, 10);
      break;
      // parse operands, and get the value of the operands
  }
  if(name == ORG_D){
    location_counter = value;
  }else if(name  == ASCII_D || name == BSS_D){
    location_counter += value;
  }else if(dir_ptr->enum_name == BYTE_D){
    if(value & 0xFF != value){ // check if 8 bit value
     // error- not 8 bits! (or warning?)
    }else{
      location_counter++;
    }
  }else if(dir_ptr->enum_name == WORD_D){
    location_counter += 2;
  }
  
  return 1;
}


// Handling 2nd Pass Directives
int handleDir_2(Dir* dir_ptr, char* operand)
{
  // if EQU: skip to next line
  // if END: stop compilation
  // Get operands
  // if BSS: LC += value
  // if BYTE: at LC => value; LC++
  // if WORD: at LC => value; LC+=2
  // if ALIGN: LC = LC%2
  // if ORG: LC = value
  // if ASCII: at LC + i => value[i]
  int i;
  char dup[strlen(operand)];
  char* ascii_str;
  uint8_t emit_length = 2;
  strcpy(dup, operand);
  uint16_t value = 0;
  DirName name = dir_ptr->enum_name;
  if(operand != NULL && name != ASCII_D){ // get values for std dirs
    value = strtol(dup, NULL, 10);
  }else if(operand != NULL && name == ASCII_D){ // ascii is special!
    strtok(dup, "\"");
    ascii_str = strtok(NULL, "\"");
    emit_length = strlen(ascii_str);
  }
  char emit_array[emit_length];
  switch(name){
    case END_D:
      return 0; // end compilation
    case EQU_D:
      return 1;
    case BSS_D:
      location_counter += value;
      return 1;
    case ALIGN_D:
      location_counter += location_counter % 2; // ensure even LC
      trigger_emit(location_counter);
      return 1;
    case ORG_D:
      location_counter = value;
      trigger_emit(location_counter);
      return 1;
    case BYTE_D:
      // need to get the value into memory at the location_counter: for all 3 below
      emit_length = 1;
      emit_array[0] = value & 0xFF;
      break;
    case WORD_D:
      // emit_length = 2; // set above
      emit_array[0] = value & 0xFF;
      emit_array[1] = (value >> 8) & 0xFF;
      break;
    case ASCII_D:
      // emit_length = strlen(ascii_str); // set above
      strcpy(emit_array, ascii_str);
      break;
    default:
      //error
      return 1;
  }
  // send off the data in emits;
  for(i = 0; i < emit_length; i++){
    emit(location_counter, (uint8_t)emit_array[i]);
    location_counter++;
  }
  return 1;
}
