// Includes
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "token_data_structures.h"
#include "symbol_table_data_structures.h"
// Defines
// Local Variables
static char loc_cntr_inc_via_addrmode[] = {0, 2, 2, 2, 0, 0, 2};
// Local Prototypes
int parseOperands(char* operand, OperandVal* val);
int parseDirOperand(char* operand, OperandVal* val);
int parseType1(char* operand, OperandVal* src);
int parseType2(char* operands, OperandVal* src, OperandVal* dst);
int parseType3(char* operand, OperandVal* dst);
// Extern Variables
// Extern Prototypes
extern void addSymbol(char* name, uint16_t value, SymbolType type);
extern Symbol* getSymbol(char* name);
extern int isLabel(char* token);
// Defintions

// Most of this is lifted verbatim from Dr.Hughes' supplied function
// personal additions include the struct use, as well as some sections
int parseOperands(char* operand, OperandVal* val)
{
  Symbol* symbol_ptr;
  char* operand_ptr;
  char* register_ptr;
  char* alphanum_ptr;
  int immediate_num;
  int sanity;

  operand_ptr = operand;

  switch(*operand_ptr)
  {
    case '&':{ // absolute addr;
      val->mode = ABSOLUTE;
      symbol_ptr = getSymbol(operand_ptr + 1);
      val->val0 = symbol_ptr->value;
      val->type0 = LABELTYPE;
      return 1;
    }
    case '@':{
      operand_ptr++;
      alphanum_ptr = operand_ptr;
      sanity = 0;

      while(isalnum(*operand_ptr) && sanity++ < 20){ operand_ptr++; }
      if(*operand_ptr == '+'){
        // indirect alphanum_ptr
        val->mode = INDIRECT_AA;
        *operand_ptr = '\0';
      } else{
        val->mode = INDIRECT;
      }

      if(*operand_ptr != '\0'){
        if(val->mode == INDIRECT_AA){
          operand_ptr--;
          *operand_ptr = '+';
        }
        val->mode = BAD_ADDR;
      }

      symbol_ptr = getSymbol(alphanum_ptr); // get symbol of the label: REG, or not?
      if(symbol_ptr == NULL || symbol_ptr->type != REGISTER){
        val->mode = BAD_ADDR; // indir can't use non-registers I guess?
        return 0;
      }
      val->val0 = symbol_ptr->value;
      val->type0 = REGISTER;
      return 1;
    }
    case '#':{
      // mode = immediate_num
      // signed, unsigned, and hex
      operand_ptr++;

      if(isLabel(operand_ptr)){ // label
        if((symbol_ptr = getSymbol(operand_ptr)) == NULL){
          // no symbol found: add in 
          addSymbol(operand_ptr, 0, UNKNOWN);
        }
        val->val0 = symbol_ptr->value;
      }else{ // constant
        if(*operand_ptr == '$'){ // hex
          operand_ptr++;
          val->val0 = strtol(operand_ptr, NULL, 16); // allows for #$FFFF, for example
        }else{ // non-hex, decimal
          val->val0 = strtol(operand_ptr, NULL, 10); // allows for both -# && +#
        }
      }
      val->mode = IMMEDIATE;
      val->type0 = LABELTYPE;
      return 1;
    }
    default:{
      if(isalpha(*operand_ptr)){
        // could be register, relative, or indexed addressing...
        alphanum_ptr = operand_ptr;
        if(strchr(operand_ptr, '(') != NULL){
          // indexed
          val->mode = INDEX;
          alphanum_ptr = strtok(operand_ptr, "(");
          symbol_ptr = getSymbol(alphanum_ptr);
          val->type1 = LABELTYPE;
          val->val1 = symbol_ptr->value;

          alphanum_ptr = strtok(NULL, ")");
          symbol_ptr = getSymbol(alphanum_ptr);
          val->type0 = REGISTER;
          val->val0 = symbol_ptr->value;
          return 1;
        }
        symbol_ptr = getSymbol(alphanum_ptr); // get the symbol from the start of the string;
        switch(symbol_ptr->type){
          case REGISTER:
            val->mode = REG_DIRECT;
            val->type0 = REGISTER;
            return 1;
          case LABELTYPE:
            val->mode = RELATIVE;
            val->type0 = LABELTYPE;
            return 1;
        }
      }
      return 0;
    }
  }
}

int parseDirOperand(char* operand, OperandVal* val)
{
  // parsing a directive's operands: not the same as an inst's!
  char dup[strlen(operand)];
  strcpy(dup, operand);
  char* operand_ptr = &(dup[0]);
  Symbol* symbol_ptr;
  // ensure that only 1 argument for a dir, unless takes no args
  if(*operand_ptr == '\0'){ // no operand
    val = NULL;
    return 1;
  }
  if(isLabel(dup)){
    // is a label;
    if((symbol_ptr = getSymbol(operand_ptr)) == NULL){ // only for 1st pass
      // symbol unfound
      addSymbol(operand_ptr, 0, UNKNOWN);
      val->type1 = UNKNOWN; // trick here: only 1 operand, but this lets me spec the label as K/U
    }
    val->type1 = symbol_ptr->type;
    val->type0 = LABELTYPE;
    val->val0 = symbol_ptr->value;
    return 1;
  }else if(*operand_ptr == '$'){ // hex
      val->val0 = strtol(operand_ptr + 1, NULL, 16);
      val->type0 = KNOWN;
      return 1;
  }else if(isdigit(*operand_ptr)){ // decimal
      val->val0 = strtol(operand_ptr, NULL, 10);
      val->type0 = KNOWN;
      return 1;
  }
    // must be an error
  val->mode = BAD_ADDR;
  return 1;
}

int parseType1(char* operand, OperandVal* src)
{
  // parsing a type1 inst operand
  // 1 operand, can be any of the 7 addr_modes
  // check for only 1 operand
  char dup[strlen(operand)];
  strcpy(dup, operand);
  char* op1_ptr = strtok(dup, " ,\n\r");
  if(strtok(NULL, " ,;\n\r\t")){ // token on all the things
    //error
    return 0;
  }
  if(parseOperands(op1_ptr, src) == 0){
    // error: some addr garbage or whatever
    return 0;
  }
  return 1;

}
int parseType2(char* operands, OperandVal* src, OperandVal* dst)
{
  // parsing type2 operands
  // 2 operands:
  // src => 7 addr modes, dst => 4 addr_modes
  // divide up; check that both aren't null
  char dup[strlen(operands)];
  strcpy(dup, operands);
  char* op1_ptr;
  char* op2_ptr;
  char* has_a_comma = strchr(dup, ',');
  if(!has_a_comma){
    // error: need a comma for type 2, for 2 arguments
    return 0;
  }else{
    *has_a_comma = '\0';
    op2_ptr = has_a_comma + 1; // set the op2 to the 1st value after the comma
  }
  op1_ptr = strtok(dup, " ;\n\r\t");
  op2_ptr = strtok(op2_ptr, " ;\n\r\t");

  if(!(op1_ptr && op2_ptr)){
    // error: one of the operands is hella broken
    return 0;
  }
  if(!(parseOperands(op1_ptr, src) && parseOperands(op2_ptr, dst))){
    // error: badd addr_type, or some shit
    return 0;
  }
  return 1;

}
int parseType3(char* operand, OperandVal* dst)
{
  char dup[strlen(operand)];
  strcpy(dup, operand);
  char* op1_ptr = strtok(dup, " ,\n\r");
  if(strtok(NULL, " ,;\n\r\t")){ // token on all the things
    //error
    return 0;
  }
  if(!strtok(NULL, " ;\n\t\r")){
    // error: too many operands.
    return 0;
  }
  if(parseOperands(op1_ptr, dst) == 0){
    // error: some addr garbage or whatever
    return 0;
  }
  if(dst->val0 % 2){
    //error: odd location;
  }
  return 1;
  // parse jumps
}
