#include "util.h"

#define SYS_EXIT 1
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define STDOUT 1
#define O_RDRW 2
#define SYS_LSEEK 19
#define SEEK_SET 0
#define SEEK_END 2
#define NAME_POSS 0x291

int main (int argc , char* argv[], char* envp[])
{
  int file_size=0, sourceFile;
  char *name;
  
  if (argv[1] ==0 || argv[2]==0)
  {
    system_call(SYS_EXIT,0x55,0,0);
  }
  name= argv[2];
  sourceFile= system_call(SYS_OPEN,argv[1],O_RDRW,0777);
  file_size=system_call(SYS_LSEEK,sourceFile,0, SEEK_END);
  if(!file_size)
    system_call(SYS_EXIT,0x55,0,0);
  system_call(SYS_LSEEK,sourceFile,0, SEEK_SET);

  system_call(SYS_LSEEK,sourceFile,NAME_POSS, SEEK_SET);
  system_call(SYS_WRITE,sourceFile,name, strlen(name));
  system_call(SYS_WRITE,sourceFile,".\n", 2);
  system_call(6,sourceFile);
  return 0;
}
