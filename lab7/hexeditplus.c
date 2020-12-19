#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define FileNameSIZE 100
#define _10K  10000
#define RUN 1

typedef struct {
  char debug_mode;
  char file_name[128];
  int unit_size;
  unsigned char mem_buf[10000];
  size_t mem_count;
  char display_mode;
  /*
   .
   .
   Any additional fields you deem necessary
  */
} state;
struct fun_desc {
    char *name;
    void (*fun)(state *);
};


void debugMode (state* s){
    if (s->debug_mode == 0){
        printf("Debug flag now on\n");
        s->debug_mode=1;
        return;
    }
    printf("Debug flag now off\n");
    s->debug_mode =0;
}
    
void SetFileName (state* s){
    char  fileName[FileNameSIZE];
    printf ("Please enter new file name:\n");
    scanf("%s",fileName);
    if (s->debug_mode)
        printf ("Debug: file name set to '%s\n'", fileName);
    //free (s->file_name);
    strcpy (s->file_name,fileName);
    }

void SetUnitSize(state* s){
    int number;
    printf("Please Enter a number:\n");
    scanf("%d",&number);
    if (number == 1 || number == 2|| number == 4){
        s->unit_size = number;
        if (s->debug_mode) printf("Debug: set size to %d\n", number);
        return;
    }
    printf("ERROR: not valid\n");
}

void loadIntoMem (state * s){
    int length, location;
    if(strcmp(s->file_name,"")== 0){
        printf ("ERROR: no file name\n");
        return;
    }
    FILE * file = fopen (s->file_name,"r");
    if (file == NULL){
        printf ("ERROR: can not open '%s'\n",s->file_name);
        return;
    }
    printf("Please enter <location> <length>:\n");
    scanf("%x%d",&location, &length);
    if (s->debug_mode) 
        printf("---\nfile name: %s\nlocation: %x\nlength: %d\n---\n", s->file_name,location,length);
    fseek(file,location,SEEK_SET);
    fread(s->mem_buf,length,s->unit_size,file);
    printf ("Loaded %d units into memory\n", length);
    fclose (file);
    //printf("*********  %s\n", s->mem_buf);
}

void ToggleDisplayMode (state * s){
    if (s->display_mode == 0){
        printf("Display flag now on, hexadecimal representation\n");
        s->display_mode=1;
        return;
    }
    printf("Display flag now off, decimal representation\n");
    s->display_mode =0;
}

char* unitToFormat(int unit) {
    static char* formats[] = {"%#hhx\n", "%#hx\n", "No such unit", "%#x\n"};
    return formats[unit-1];}


void MemoryDisplay (state *s){
    int adress, unit;
    printf("Please enter <units> <address>\n");
    scanf ("%d%x",&unit,&adress);
    if (adress==0 )
           adress = *(s->mem_buf);

    if (s->display_mode == 0){
        printf("Decimal\n=======\n");
        while (unit  > 0) {
        int var = *((int*)(adress));
        fprintf(stdout, "%d", var);
        adress += s->unit_size;
        unit-=1;
        }
    } else {
        printf("Hexadecimal\n=======\n");
        while (unit  > 0) {
        int var = *((int*)(adress));
        fprintf(stdout, "%x", var);
        adress += s->unit_size;
        unit-=1;
        }
    }
}

void SaveIntoFile (state * s){
    int sourceAddress,targetLocation,length;
    printf("Please enter <source-address> <target-location> <length>\n");
    scanf("%x%d%d", &sourceAddress,&targetLocation, &length);
    FILE * file = fopen (s->file_name, "r+");
    if (file == NULL){
        printf ("ERROR: can not write into file\n");
        return;
    }
    fseek (file,targetLocation,SEEK_SET);
    fwrite(s->mem_buf +sourceAddress,s->unit_size,length,file);
    fclose (file);
}

void MemoryModify(state * s){
   int location, val;
   printf("Please enter <location> <val>\n");
   scanf ("%x%x",&location,&val);
   if (s->debug_mode)
    printf("location: %x\nval: %x", location,val);
    printf ("%s", s->mem_buf);
    s->mem_buf[location*s->unit_size] = val;
    printf ("%s", s->mem_buf);
}


void quit(state* s){
    if (s->debug_mode)
        printf("quitting\n");
    exit(0);
    }


int main(int argc, char **argv){
    int i, input, bound, debugFlag;
    state * s = (state *) malloc (sizeof(state));
    struct fun_desc menu[] = { {"Toggle Debug Mode",debugMode},{ "Set File Name", SetFileName}, { "Set Unit Size", SetUnitSize },{"Load Into Memory", loadIntoMem},{"Toggle Display Mode",ToggleDisplayMode},{"Memory Display", MemoryDisplay},{"Save Into File",SaveIntoFile},{"Memory Modify",MemoryModify},{"Quit", quit},{ NULL, NULL } };
    bound = (sizeof(menu))/(sizeof(menu[0])) - 1;
    while (RUN){
        if (s->debug_mode)
            printf("---\nunit size: %d\nfile name: %s\nmem count:%d\n---\n",s->unit_size,s->file_name,s->mem_count);
        printf ("Choose action:\n");
        for (i=0; menu[i].name != NULL; i++) 
            printf ("%d - %s\n", i, menu[i].name);
        printf ("option: ");
        scanf("%d",&input);
        if (input>= 0 && input <= 8)
             menu[input].fun(s);
        else
        {
            printf("bad input\n");
            break;
        }
        
        //map(s,menu[input].fun);
       
        } 
    }
    
    
