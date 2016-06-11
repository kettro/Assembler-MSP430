# Assembler MSP430

This is an assembler for the MSP430 MCU.
It compiles .asm files to .s19 files. 

## Instructions

The assembler will interpret one of the 31 instructions 
defined in its architecture. These include instructions such
as instructions like MOV x,y; JMP x; and  SWBP x

Instructions can be capital or lowercase. 
Arguments must be space by commas, but may have spaces or tabs. 
Comments may be inserted after a semicolon(;). 

##Directives

The assembler understands 8 directives, used to store data, or 
modify the Program Counter's location. These are:
* ALIGN
* ASCII
* BSS
* BYTE
* END
* EQU
* ORG
* WORD

ALIGN will align the LC to the next even byte. 

ASCII will store a string in adjacent locations. Escape characters are as of yet unsupported. 

BSS will reserve a set number of bytes.

BYTE will reserve one byte with the specfied value 

END will end compilation

ORG will change the location counter to the specified location

WORD will reserve one word, with the value specified

## Output

Outputs are stored in the output folder. This includes the executable, 
output/a.
