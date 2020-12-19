#include "util.h"
extern int system_call(int, ...);

#define SYS_EXIT 1
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define STDOUT 1
#define STDIN 0
#define STDERR 2
#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDRW 2
#define O_CREATE 64
#define O_APPEND 1024

int main(int argc, char **argv) {
    int output = STDOUT;
    int input = STDIN;
    char arr_const[] = {'A'-'a'};
    char* arr = arr_const;
    int debug =0, i, returnInput;
    char c[1];
    

    for(i=1; i<argc; i++){
        if(strcmp(argv[i],"-D")==0)
	        debug=1;
        if(argv[i][0] == '-' && argv[i][1] == 'o') {
            output = system_call(SYS_OPEN,argv[i]+2, O_CREATE | O_RDRW, 0777);
            if(debug==1){
                system_call(SYS_WRITE,STDERR,"system call: SYS_OPEN: 5", 25 );
                system_call(SYS_WRITE,STDERR,"      return value: ", 21 );
                system_call(SYS_WRITE,STDERR,itoa(output), sizeof(output) );
                system_call(SYS_WRITE,STDERR,"\n", 2 );
            }
        }
        if(argv[i][0] == '-' && argv[i][1] == 'i') {
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
        }
    }
    returnInput = system_call(SYS_READ,input,c,1);
    if(debug==1){
                system_call(SYS_WRITE,STDERR,"system call: SYS_READ: 3", 25 );
                system_call(SYS_WRITE,STDERR,"      return value: ", 21 );
                system_call(SYS_WRITE,STDERR,itoa(returnInput), sizeof(returnInput) );
                system_call(SYS_WRITE,STDERR,"\n", 2 );
            }
    while(returnInput > 0){
        
        if ((c[0] >= 'a' && c[0]<='z'))
            c[0]= c[0]+ arr[0];
        
       
        returnInput= system_call(SYS_WRITE,output,c, 1);
        if(debug==1){
                system_call(SYS_WRITE,STDERR,"system call: SYS_WRITE: 4", 26 );
                system_call(SYS_WRITE,STDERR,"      return value: ", 21 );
                system_call(SYS_WRITE,STDERR,itoa(returnInput), sizeof(returnInput) );
                system_call(SYS_WRITE,STDERR,"\n", 2 );
            }
        returnInput = system_call(SYS_READ,input,c,1);
        if(debug==1){
                system_call(SYS_WRITE,STDERR,"system call: SYS_READ: 3", 25 );
                system_call(SYS_WRITE,STDERR,"      return value: ", 21 );
                system_call(SYS_WRITE,STDERR,itoa(returnInput), sizeof(returnInput) );
                system_call(SYS_WRITE,STDERR,"\n", 2 );
            }
    }
     
    if(output!=STDOUT){
       returnInput =system_call(SYS_CLOSE, output,0,0);
        if(debug==1){
                system_call(SYS_WRITE,STDERR,"system call: SYS_CLOSE: 6", 26 );
                system_call(SYS_WRITE,STDERR,"      return value: ", 21 );
                system_call(SYS_WRITE,STDERR,itoa(returnInput), sizeof(returnInput) );
                system_call(SYS_WRITE,STDERR,"\n", 2 );
            }
        
    }
    return 0;
}