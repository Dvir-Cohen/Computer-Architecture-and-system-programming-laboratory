#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "LineParser.h"

#define MAX_READ 2048
#define TERMINATED  -1
#define RUNNING 1
#define SUSPENDED 0

typedef struct process{
    cmdLine* cmd;                         /* the parsed command line*/
    pid_t pid; 		                  /* the process id that is running the command*/
    int status;                           /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next;	                  /* next process in chain */
} process;

process **process_list;
void freeProcess(process* p);

char* printStatus(int status){
    switch(status) {
    case TERMINATED:
        return "TERMINATED";
    case RUNNING:
        return "RUNNING";
    case SUSPENDED:
        return "SUSPENDED";
    default:
        return "NO STATUS";
    }
}
void addProcess(process** process_list, cmdLine* cmd, pid_t pid){
   
    int i=0;
    if ((*process_list)== NULL)
    {
        process_list[0]= malloc(sizeof(process));
        process_list[0]->cmd = cmd;
        process_list[0]->pid = pid;
        process_list[0]->status = RUNNING;
        process_list[0]->next = NULL;
    }
    else 
    {
        while (process_list[i]->next !=NULL)
        {
            i++;
        }
        process_list[i+1]= malloc(sizeof(process));
        process_list[i+1]->cmd = cmd;
        process_list[i+1]->pid = pid;
        process_list[i+1]->status = RUNNING;
        process_list[i+1]->next = NULL;
        process_list[i]->next= process_list[i+1];      
    }    
}

void printProcessList(process** process_list){
    int i=1,j;
    process * tmp = (*process_list);
    updateProcessList(process_list);
    printf("index\tPID\tSTATUS\tCommand\n");
    while (tmp != NULL)
    {
        printf("%d\t",i);
        printf("%d\t", tmp-> pid);
        printf("%s\t", printStatus(tmp->status));
        j=0;
        while (tmp->cmd->arguments[j]!=NULL)
        {
             printf("%s ", tmp->cmd->arguments[j] );
             j++;
        }
        printf("\n");

        i++;

            tmp= tmp->next;
    }
    DeleteTerminated();   
    
}

void DeleteTerminated (){
    int i=0;
    process *tmp  = * process_list;
    process *delP;
    
    

    while (tmp != NULL )
    {
        if ( tmp->status == TERMINATED){
            delP =tmp;
            tmp = tmp->next;
            freeProcess(delP);
            * process_list =tmp;
    }
       else  if (tmp->next !=NULL && tmp->next ->status == TERMINATED)
        {
               delP = tmp->next;
                tmp->next = delP->next;
                freeProcess(delP);
                
        }
        else
            tmp = tmp->next;
        
    }
    
}
void execute(cmdLine *pCmdLine, int Dflag){
    int check;
    pid_t pid, w8;
    
    if(!(pid =fork())) 
    {
        if (Dflag)
        {
            fprintf(stderr,"pid of child process is: %d\n\n\n", getpid() );
        }
       
        execvp(pCmdLine->arguments[0],pCmdLine->arguments);
        perror("Error ");
        exit(1);
    }
     addProcess(process_list,pCmdLine,pid);
    if (pCmdLine->blocking == 1)
    {
        do {
            w8= waitpid(pid, &check, WUNTRACED | WCONTINUED);
            if (w8 == -1) {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }

        //    if (WIFEXITED(check)) {
        //         printf("exited, status=%d\n", WEXITSTATUS(check));
        //     } else if (WIFSIGNALED(check)) {
        //         printf("killed by signal %d\n", WTERMSIG(check));
        //     } else if (WIFSTOPPED(check)) {
        //         printf("stopped by signal %d\n", WSTOPSIG(check));
        //     } else if (WIFCONTINUED(check)) {
        //         printf("continued\n");
        //     }
        } while (!WIFEXITED(check) && !WIFSIGNALED(check));
        
    }
}
   
void freeProcess(process* p){
    freeCmdLines(p->cmd);
    free(p);
}
void freeProcessList(process** process_list){
    int i=0;
    while (process_list[i] !=NULL)
    {
        freeProcess(process_list[i]);
        i++;
    }   
}

void updateProcessList(process **process_list){
    int i=0, check;
    pid_t w;
    while (process_list[i] !=NULL)
    {
        w= waitpid(process_list[i]->pid, &check, WNOHANG | WUNTRACED | WCONTINUED);
        if (w == TERMINATED)
            process_list[i]->status = TERMINATED;
        if (w == 0)
        {
            process_list[i]->status = RUNNING;
                        
        }
        if (w == process_list[i]->pid )
        {
            if(WIFSTOPPED(check)){
                process_list[i]->status = SUSPENDED;
            }
            else if (WIFCONTINUED(check))
            {
                process_list[i]->status = RUNNING;
            }
            else if(WIFSIGNALED(check))
                process_list[i]->status = TERMINATED;
            
        }
        
        i++;
    }
}

void updateProcessStatus(process** process_list, int pid, int status){
    while ((*process_list) != NULL)
    {
        if ((*process_list)->pid == pid)
        {
            (*process_list)->status = status;
            break;
        }
        (*process_list)= (*process_list)->next;
    }
}
int main(int argc, char *argv[])
{
  char buf[PATH_MAX];
  char reading[MAX_READ];
  cmdLine *data= malloc(sizeof(cmdLine));
  process_list= malloc(sizeof(process));
  (*process_list)=NULL;
    
  int debug =0, i;
  if (argv[0]==NULL)
    exit(1);
  for(i=1; i<argc; i++){
        if(strcmp(argv[i],"-d")==0)
	        debug=1;
  }
  while (1)
  {
    getcwd(buf, PATH_MAX);
    printf("working dir is:  %s\n", buf);
    fgets(reading, MAX_READ, stdin);
    if (strcmp("\n", reading)==0)
    {
        printf("empty line is not allowed!\n");
    }
    else{
    data = parseCmdLines(reading);
    if (strcmp(data->arguments[0], "quit")==0)
    {
        printf("quiting\n");
        exit(0);
    }
    pid_t pid=getpid();

    if (debug)
    {
        fprintf(stderr, "Executing command is:  %s", reading);
        fprintf(stderr,"pid of process is: %d\n\n\n", pid );
    }
    if (strcmp(data->arguments[0], "cd")==0){ 
        if( data->arguments[1]==NULL){
            printf("no argument\n");
        }
        else if(chdir(data->arguments[1]) != 0) {
            printf("%s\n", data->arguments[1]);
            perror("change directory failed");
        }
    }
    else if(strcmp(data->arguments[0], "kill")==0){
        if( data->arguments[1]==NULL){
            printf("no argument\n");
        }
        kill((pid_t)atoi(data->arguments[1]),SIGINT);
    }
    else if(strcmp(data->arguments[0], "wake")==0){
        if( data->arguments[1]==NULL){
            printf("no argument\n");
        }
         printf("in wake\n");
        kill((pid_t)atoi(data->arguments[1]),SIGCONT);
    }
    else if(strcmp(data->arguments[0], "suspend")==0){
        if( data->arguments[1]==NULL){
            printf("no argument\n");
        }
        printf("in suspend\n");
        kill((pid_t)atoi(data->arguments[1]),20);
    }
    else if (strcmp(data->arguments[0], "procs")==0){
            printProcessList(process_list);
        } 
    else
    { 
        execute(data, debug);
    }
    fflush(stdin);
    }
    
  }
    

freeCmdLines(data);
return 0;
}