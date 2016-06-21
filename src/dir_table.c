// Includes
#include "token_data_structures.h"
#include "dir_table.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
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
int handleDir_1(char* command, char* operand);
int handleDir_2(char* command, char* operand);
// Extern Variables
extern uint16_t location_counter;
extern int error_count;
extern FILE* error_file;
// Extern Function Prototypes
extern int parseDirOperand(char* operand, OperandVal* val);
extern void emit(uint8_t value, uint16_t lc); // defined in an srec generator file
extern void triggerEmit(uint16_t lc);        // "", sends the emit file if >32, or if triggered
// Definitions

/* isDir
 * Desc: check is a string is a directive
 * Param: string, dir table
 * Return: boolean; is or isn't a directive
 * Results: none
 */
int isDir(char* token)
{
  int i;
  char dup[strlen(token)];
  strcpy(dup, token);
  for(i = 0; i < strlen(dup); i++){
    dup[i] = toupper(dup[i]);
  }

  for(i = 0; i < TOTAL_DIRS; i++){
    if(strcmp(dup, dir_table[i].name) == 0){
      return 1;
    }
  }
  return 0;
}

/* getDir
 * Desc: retrieves a directive
 * Param: string, dir table
 * Return: pointer to the directive requested
 * Results: -
 */
Dir* getDir(char* token)
{
  int i;
  char dup[strlen(token)];
  strcpy(dup, token);

  for(i = 0; i < strlen(dup); i++){
    dup[i] = toupper(dup[i]);
  }
  for(i = 0; i < TOTAL_DIRS; i++){
    if(strcmp(dup, dir_table[i].name) == 0){
      return &(dir_table[i]);
    }
  }
  return NULL;
}

/* Handle Dir 1
 * Desc: Handle 1st pass Directives
 * Param: command string and operand string
 * Return: boolean, if END is reached
 * Results: modify LC, add symbols
 */
int handleDir_1(char* command, char* operand)
{
  char dup_op[strlen(operand)];
  char dup_com[strlen(command)];
  int i;
  char* misc_ptr; // for misc things`
  char* op_ptr;
  strcpy(dup_op, operand);
  strcpy(dup_com, command);
  for(i = 0; i < strlen(dup_com); i++){
    dup_com[i] = toupper(dup_com[i]);
  }
  uint16_t value = 0;
  Dir* dir_ptr = getDir(dup_com); // get the directive, as is a directive
  op_ptr = &dup_op[0];
  DirName name = dir_ptr->enum_name;
  OperandVal val;
  if(name == ASCII_D){
    if(*op_ptr != '\"'){
      error_count++;
      fprintf(error_file, "Error @%x: ASCII operand must be in a string\n", location_counter);
      // error: ascii must be a string
      return 1;
    }else{ // op_ptr == ", count number of args in string
      op_ptr++;
      misc_ptr = op_ptr;
      for(i = 0; i < strlen(operand) && *op_ptr != '\"'; i++){
        op_ptr++;
      }
      if(i == strlen(operand) || *op_ptr != '\"'){
        error_count++;
        fprintf(error_file, "Error @%x: ASCII operand must have enclosed quotes\n", location_counter);
        // error: went too far 
        return 1;
      }
      *op_ptr = '\0'; // create a boundary at the last quote
      value = strlen(misc_ptr);
    }
  }else{ // Not ascii: some thing else
    if(parseDirOperand(dup_op, &val) == 0){
      error_count++;
      fprintf(error_file, "Error @%x: Error encountered in op_parse\n", location_counter);
      // error: error in op parse
      return 1;
    }
    if(&val != NULL){ // if no operands => ALIGN, or END
      if((val.type0 == LABELTYPE && val.type1 == UNKNOWN) && (name == ORG_D || name == BSS_D)){
        error_count++;
        fprintf(error_file, "Error @%x: No Forward Declaration Allowed for BSS or ORG\n", location_counter);
        // no forward declarations for org or bss
        // error
        return 1;
      }
      value = val.val0;
    }
  }
  switch(name){
    case END_D:
      // stop compilation
      return 0;
    case EQU_D:
      error_count++;
      fprintf(error_file, "Error @%x: EQU must have a Label\n", location_counter);
      // error
      return 1;
    case ALIGN_D:
      if(dup_op[0] != '\0'){ 
        error_count++;
        fprintf(error_file, "Error @%x: No arguments allowed for ALIGN\n", location_counter);
        //error
        return 1;
      }
      location_counter += location_counter % 2;
      return 1;
    default:
      break;
      // parse operands, and get the value of the operands
  }
  if(name == ORG_D){
    location_counter = value;
  }else if(name  == ASCII_D || name == BSS_D){
    location_counter += value;
  }else if(name == BYTE_D){
    if((value & 0xFF) != value){ // check if 8 bit value
      error_count++;
      fprintf(error_file, "Error @%x: BYTE operand not 8 bits\n", location_counter);
      // error- not 8 bits! (or warning?)
    }else{
      location_counter += 1;
    }
  }else if(name == WORD_D){
    location_counter += 2;
  }
  
  return 1;
}

/* handle Dir 2
 * Desc: Handling 2nd Pass Directives
 * Param: command and operand strings
 * Return: boolean, if END reached, or if an error is reached
 * Results: emit data and trigger emits on discontinuities
 */
int handleDir_2(char* command, char* operand)
{
  char dup_op[strlen(operand)];
  char dup_com[strlen(command)];
  int i;
  char* op_ptr;
  strcpy(dup_op, operand);
  strcpy(dup_com, command);
  for(i = 0; i < strlen(dup_com); i++){
    dup_com[i] = toupper(dup_com[i]);
  }
  op_ptr = &(dup_op[0]);
  uint16_t value = 0;
  Dir* dir_ptr = getDir(dup_com); // get the directive, as is a directive
  DirName name = dir_ptr->enum_name;
  OperandVal val;
  char* ascii_str;

  //determine emit_length
  uint8_t ascii_length = 0; // default max to 2
  if(operand != NULL && name != ASCII_D){ // get values for std dirs
    value = strtol(dup_op, NULL, 10);
    val.type1 = 2;
    if(parseDirOperand(operand, &val) == 0){
      //error
    }
    value = val.val0;
  }else if(operand != NULL && name == ASCII_D){ // ascii is special!
    if(*op_ptr == '\"'){ // yay
      op_ptr++;
      ascii_str = strtok(op_ptr, "\"");
      ascii_length += strlen(ascii_str);
    }else{ return 0; } // error
  }
  char emit_array[ascii_length]; // reserve an array large enough to emit anything
  switch(name){
    case END_D:
      triggerEmit(location_counter);
      return 0; // end compilation
    case EQU_D:
      return 1;
    case BSS_D:
      location_counter += value;
      triggerEmit(location_counter);
      return 1;
    case ALIGN_D:
      if(location_counter % 2){
        location_counter++;
        triggerEmit(location_counter);
      }
      return 1;
    case ORG_D:
      location_counter = value;
      triggerEmit(location_counter);
      return 1;
    case BYTE_D:
      // need to get the value into memory at the location_counter: for all 3 below
      ascii_length = 1;
      emit_array[0] = value & 0xFF;
      break;
    case WORD_D:
      // double check the endedness of the msp
      ascii_length = 2;
      emit_array[0] = value & 0xFF;
      emit_array[1] = (value >> 8) & 0xFF;
      break;
    case ASCII_D:
      strcpy(emit_array, ascii_str);
      break;
    default:
      fprintf(error_file, "Error @%x: Directive not recognized\n", location_counter);
      //error
      return 1;
  }
  // send off the data in emits;
  for(i = 0; i < ascii_length; i++){
    emit((uint8_t)emit_array[i], location_counter);
  }
  return 1;
}
