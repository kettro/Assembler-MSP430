#ifndef DIR_TABLE_H
#define DIR_TABLE_H

#define DIR_NAME_LENGTH     16
// need to define DIR struct
// need to define DIR enum

typedef enum DirName{
  ALIGN = 0;
}DirName;

typedef struct Dir{
  char name[DIR_NAME_LENGTH];
  DirName enum_name;
  void (*executeDir)(); // maybe have this-> fn pointer?
}Dir;

#endif
