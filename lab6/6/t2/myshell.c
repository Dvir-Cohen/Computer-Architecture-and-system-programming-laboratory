#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include "LineParser.h"

#define MAX_READ 2048

typedef struct PairLink {
    char *name;
    char *val;
    struct PairLink *next;
} PairLink;
PairLink **pair_list;



void SetPair(PairLink **pair_list, char *x, char *y){
   int i=0, handel=0;
    if ((*pair_list)== NULL)
    {
        pair_list[0]= malloc(sizeof(PairLink));
        pair_list[0]->name = x;
        pair_list[0]->val = y;
        pair_list[0]->next = NULL;
    }
    else 
    {
    while (pair_list[i]->next != NULL)
    {   
        if (strcmp(pair_list[i]->name, x)==0)
        {
            pair_list[i]->val = y;
            handel=1;
        }
        i++;
    }
    if(!handel){
        if (strcmp(pair_list[i]->name, x)==0){
            pair_list[i]->val = y;
        }
        else
        {
            pair_list[i+1] = malloc(sizeof(PairLink));
            pair_list[i+1]->name = x;
            pair_list[i+1]->val = y;
            pair_list[i+1]->next = NULL;
            pair_list[i]->next = pair_list[i+1];
        }
    }

    }
}
void PrintVarList(PairLink **pair_list){
    int i=1;
    PairLink * tmp = (*pair_list);
    printf("index\tname\tvalue\n");
    while (tmp != NULL)
    {
        printf("%d\t",i);
        printf("%s\t", tmp->name);
        printf("%s\n", tmp->val);
        i++;
        tmp= tmp->next;
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
        if(pCmdLine->inputRedirect != NULL){
            close(STDIN_FILENO);
            open(pCmdLine->inputRedirect, O_RDONLY);
        }
        if(pCmdLine->outputRedirect != NULL){
            close(STDOUT_FILENO);
            open(pCmdLine->outputRedirect, O_CREAT| O_RDWR| O_APPEND,0777);
        }
        execvp(pCmdLine->arguments[0],pCmdLine->arguments);
        perror("Error ");
        exit(1);
    }
    if (pCmdLine->blocking == 1)
    {
        do {
            w8= waitpid(pid, &check, WUNTRACED | WCONTINUED);
            if (w8 == -1) {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }
        }
        while (!WIFEXITED(check) && !WIFSIGNALED(check));
        
    }
}

int main(int argc, char *argv[])
{
  char buf[PATH_MAX];
  char reading[MAX_READ];
  cmdLine *data= malloc(sizeof(cmdLine));
  pair_list= malloc(sizeof(PairLink));
  (*pair_list)=NULL;
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
    printf("working dir is:  %s$ ", buf);
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
        else{
            if (strcmp(data->arguments[1], "~")==0)
                replaceCmdArg(data, 1, getenv("HOME"));
            if(chdir(data->arguments[1]) != 0) {
            printf("%s\n", data->arguments[1]);
            perror("change directory failed");
            /*fprintf(stderr,"change directory failed\n");*/ 
        }
        }
    }
    else if(strcmp(data->arguments[0], "set")==0)
    {
        if (data->arguments[1] != NULL && data->arguments[2] != NULL)
        {
            SetPair(pair_list,data->arguments[1],data->arguments[2]);
        }
        else
            fprintf(stderr,"not enough arguments for set\n");
    }
    else if (strcmp(data->arguments[0], "vars")==0)
    {
        PrintVarList(pair_list);
    }
    
    else
    {   i=0;
        while (data->arguments[i]!=NULL)
        {
            if (data->arguments[i][0]== '$')
            {
                char *var= malloc(strlen(data->arguments[i])-1);
                int j;
                for ( j=0; j< strlen(data->arguments[i])-1; j++ )
                    var[j]= data->arguments[i][j+1];
                var[j]='\0';
                PairLink *tmp = (*pair_list);
                while (tmp != NULL)
                {
                    if (strcmp(tmp->name, var)==0)
                    {
                       replaceCmdArg(data, i, tmp->val);
                       break;
                    }
                    tmp= tmp->next;
                }
                if (tmp == NULL)
                    printf("variables are not found\n");
                free(var);
            }
            
            i++;
        }
        
        
        execute(data, debug);
    }
    
    }
  }
  freeCmdLines(data);
  return 0;
}

