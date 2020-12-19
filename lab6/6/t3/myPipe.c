#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#define MAX_READ 2000


void debuger(int flag, char * str){
    if (flag)
        fprintf(stderr,"%s\n", str);
}

pid_t ForkChild1(int debug,int* reader_writer, char ** cmd){
    int dupl;
    pid_t pid;

    if(!(pid =fork())){
        debuger (debug,"(child1>redirecting stdout to the write end of the pipe…)");
        fclose(stdout);
        dupl= dup(reader_writer[1]);
        close (reader_writer[1]);
        debuger(debug,"(child1>going to execute cmd: …)");
        execvp(cmd[0], cmd);
        perror("Error ");
        exit(1);
    }
    return pid;
}

pid_t ForkChild2(int debug, int * reader_writer, char ** cmd){
    int dupl;
    pid_t pid;
    
    if(!(pid =fork()))  {
        debuger (debug,"(child2>redirecting stdin to the read end of the pipe…)");
        fclose(stdin);
        dupl= dup(reader_writer[0]);
        close (reader_writer[0]);
        debuger (debug,"(child2>going to execute cmd: …)\n" );
        execvp(cmd[0], cmd);
        perror("Error ");
        exit(1);
        }
    return pid;
}


int main(int argc, char *argv[]){    
    char * parseCmd1,* parseCmd2;
    char * word;
    char * Cmd1 [MAX_READ], * Cmd2 [MAX_READ];
    char reading[MAX_READ];
    int debug =0, i, check, dupl;
    pid_t child1_pid,child2_pid , w8;

    if (argv[0]==NULL)
        exit(1);
    for(i=1; i<argc; i++){
        if(strcmp(argv[i],"-d")==0)
	        debug=1;
    }
    
    printf("Insert your request:\n");
    fgets(reading, MAX_READ, stdin);
    //parse input string
    parseCmd1=strtok (reading,"|");
    parseCmd2=strtok (NULL,"\n");
    
    word = strtok(parseCmd1, " ");
    for ( i = 0; word != NULL; i++){
        Cmd1[i] = word;
        word = strtok(NULL, " ");
    }
    
    word = strtok(parseCmd2, " ");
    for ( i = 0; word != NULL; i++){
        Cmd2[i] = word;
        word = strtok(NULL, " ");
    }

    // create Pipe
    int reader_writer[2];
    if (pipe(reader_writer) == -1) {
        perror("pipe");
        return(0);
    }

    //Fork 1 child process
    debuger (debug, "(parent_process>forking…)");
    child1_pid = ForkChild1(debug,reader_writer, Cmd1);
    if (debug)
        fprintf(stderr ,"(parent_process>created process with id:%d)\n", child1_pid);
    
    debuger(debug,"(parent_process>closing the write end of the pipe…)");
    close (reader_writer[1]);

    //Fork  2 child process
    debuger(debug ,"(parent_process>forking…)" );
    child2_pid = ForkChild2(debug, reader_writer, Cmd2);
    if (debug)
        fprintf(stderr,"(parent_process>created process with id: %d)\n", child2_pid);

    
    debuger(debug,"(parent_process>closing the read end of the pipe…)");
    close (reader_writer[0]);

    //waiting
    debuger (debug, "(parent_process>waiting for child processes to terminate…)");
    do {
            w8= waitpid(child2_pid, &check, WUNTRACED | WCONTINUED);
            if (w8 == -1) {
                 perror("waitpid");
                 exit(EXIT_FAILURE);
             }

    } while (!WIFEXITED(check) && !WIFSIGNALED(check));  
    
    debuger (debug, "(parent_process>exiting…)");

}