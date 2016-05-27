#ifndef DIR_TABLE_H
#define DIR_TABLE_H

#define DIR_NAME_LENGTH     16
#include "enum_types.h"

typedef struct Dir{
  char name[DIR_NAME_LENGTH];
  DirName enum_name;
}Dir;

#endif
