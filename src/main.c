#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
// Local Defines
// Local Variables
uint16_t location_counter;
FILE* s19_file;
FILE* asm_file;
FILE* error_file;
// Local Prototypes
// Extern Variables
extern int error_count;
// Extern Prototypes
extern int findUnknowns(void);
extern void initSymbolTable(void);
extern int firstPass(void);
extern void secondPass(void);

extern void testSuite(char test);
// Definitions

int main(int argc, char* argv[])
{
  /*//Testing:
  s19_file = fopen("out.s19", "w");
  error_file = fopen("error.lis", "w");
  testSuite('e'); // Inst table test
*/
  initSymbolTable();
  if(argc < 2 || argc > 3){
    printf("Provide only 1 .asm file\n");
    return 0;
  }
  asm_file = fopen(argv[1], "r");
  s19_file = fopen("out.s19", "w");
  error_file = fopen("error.lis", "w");
  
  firstPass();
  int unknowns = 0;
  if((unknowns = findUnknowns()) != 0 || error_count != 0){
    // error: unknowns!!!
    printf("errors Encountered\n");
    if(unknowns){
      fprintf(error_file, "Unknown Variables Encountered\n");
    }
    if(error_count){
      fprintf(error_file, "Syntax Errors Encountered\n");
    }
    fclose(error_file);
    fclose(s19_file);
    fclose(asm_file);
    return 0;
  }
  
  fclose(asm_file);
  asm_file = fopen(argv[1], "r"); // rewind the file
  secondPass();

  fclose(asm_file);
  fclose(error_file);
  fclose(s19_file);

  return 0;
}
