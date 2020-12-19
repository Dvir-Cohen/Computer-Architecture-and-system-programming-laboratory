#define _GNU_SOURCE
#include "util.h"
#include <dirent.h> 

#define SYS_EXIT 1
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_GETDENTS 141
#define STDOUT 1
#define STDIN 0
#define STDERR 2
#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDRW 2
#define O_CREATE 64
#define O_APPEND 1024
#define dataSize 8192

struct linux_dirent {
               unsigned long  d_ino;     /* Inode number */
               unsigned long  d_off;     /* Offset to next linux_dirent */
               unsigned short d_reclen;  /* Length of this linux_dirent */
                                 /* length is actually (d_reclen - 2 -
                                    offsetof(struct linux_dirent, d_name)) */
               
               char           pad;       /*// Zero padding byte*/
               char           d_type;    /* File type (only since Linux
                                          2.6.4); offset is (d_reclen - 1)*/
               char           d_name[];  /* Filename (null-terminated) */
               
           };



int main (int argc , char* argv[], char* envp[]){
    int dir =0,  dir_pointer = 0, d_type = 0;
    int debug =0, i, returnInput, pFlag=0;
    char *tempNameFile, *pName, *type;
    char data[dataSize];
    struct linux_dirent * directory;

    for(i=1; i<argc; i++){
        if(strcmp(argv[i],"-D")==0)
	        debug=1;
        if(argv[i][0] == '-' && argv[i][1] == 'p') {
            pName= argv[i]+2;
            pFlag=1;
        }
       /* if(argv[i][0] == '-' && argv[i][1] == 'a') {
            input = system_call(SYS_OPEN,argv[i]+2,O_RDONLY, 0777);
            if(debug==1){
                system_call(SYS_WRITE,STDERR,"system call: SYS_OPEN: 5", 25 );
                system_call(SYS_WRITE,STDERR,"      return value: ", 21 );
                system_call(SYS_WRITE,STDERR,itoa(input), sizeof(input) );
                system_call(SYS_WRITE,STDERR,"\n", 2 );
            }
            if(input<0){
                system_call(SYS_WRITE,STDOUT,"no such file\n", 14 );
                system_call(SYS_EXIT,0x55,0,0);
            }
        }*/
    }
    
    returnInput=system_call(SYS_WRITE,STDERR,"flame2 starting!\n", 18 );
    if(debug==1){
                system_call(SYS_WRITE,STDERR,"system call: SYS_WRITE: 4", 26 );
                system_call(SYS_WRITE,STDERR,"      return value: ", 21 );
                system_call(SYS_WRITE,STDERR,itoa(returnInput), sizeof(returnInput) );
                system_call(SYS_WRITE,STDERR,"\n", 2 );
            }
    dir = system_call(SYS_OPEN,".", O_RDONLY,0777);
    if (dir< 0)
    {
        system_call(SYS_WRITE,STDOUT,"no such directory\n", 19 );
        system_call(SYS_EXIT,0x55,0,0);   
    }
    returnInput =  system_call(SYS_GETDENTS, dir, data, dataSize);
    if (returnInput< 0)
    {
        system_call(SYS_WRITE,STDOUT,"no files\n", 19 );
        system_call(SYS_EXIT,0x55,0,0);   
    }
    directory = (struct linux_dirent *) (data + dir_pointer);
    i=1;
    while(directory->d_reclen != 0){
        
        tempNameFile= directory->d_name -2;

        if(strlen(tempNameFile) > 0){
            if((pFlag==0)||(strncmp(pName,tempNameFile,strlen(pName))==0)){
                system_call(SYS_WRITE,STDOUT,itoa(i),3);
                system_call(SYS_WRITE,STDOUT,".  ", 4 );
                returnInput= system_call(SYS_WRITE,STDOUT,tempNameFile,strlen(tempNameFile));
                system_call(SYS_WRITE,STDOUT,"\n", 1 );
                i++;
                if(debug==1){
                    system_call(SYS_WRITE,STDERR,"system call: SYS_WRITE: 4", 26 );
                    system_call(SYS_WRITE,STDERR,"      return value: ", 21 );
                    system_call(SYS_WRITE,STDERR,itoa(returnInput), sizeof(returnInput) );
                    system_call(SYS_WRITE,STDERR,"\n", 2 );
                }
            
                if(debug==1){
                    d_type = *(data + dir_pointer + directory->d_reclen - 1);
                    type = (d_type == DT_REG) ?  "regular" :
                                    (d_type == DT_DIR) ?  "directory" :
                                    (d_type == DT_FIFO) ? "FIFO" :
                                    (d_type == DT_SOCK) ? "socket" :
                                    (d_type == DT_LNK) ?  "symlink" :
                                    (d_type == DT_BLK) ?  "block dev" :
                                    (d_type == DT_CHR) ?  "char dev" : "???";
                    system_call(SYS_WRITE,STDERR,"name file is: ", 14 );
                    system_call(SYS_WRITE,STDERR,tempNameFile, strlen(tempNameFile) );
                    system_call(SYS_WRITE,STDERR,"      length of file: ", 22);
                    system_call(SYS_WRITE,STDERR,itoa(directory->d_reclen), sizeof(directory->d_reclen) );
                    system_call(SYS_WRITE,STDERR,"      file type is: ", 20);
                    /*system_call(SYS_WRITE,STDERR,itoa(file_type), sizeof(file_type) );*/
                    system_call(SYS_WRITE,STDERR,type, strlen(type) );
                    system_call(SYS_WRITE,STDERR,"\n", 2 );
                }
            }    
        }

        dir_pointer =dir_pointer+ directory->d_reclen;
        directory = (struct linux_dirent *) (data + dir_pointer);
    }

    return 0;
}