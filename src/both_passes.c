// Includes
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "token_data_structures.h"
#include "symbol_table_data_structures.h"
// Defines
// Local Variables
// Local Prototypes
int ParseOperands(char* operand, OperandVal* val);
// Extern Variables
// Extern Prototypes
extern Symbol* getSymbol(char* name);
extern int isLabel(char* token);
// Defintions


int ParseOperands(char* operand, OperandVal* val)
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
        val->mode = BAD_ADDR;
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
        symbol_ptr = getSymbol(operand_ptr);
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

