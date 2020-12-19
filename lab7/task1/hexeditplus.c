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
  int fileLen;
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
   unsigned int length, location;
    char input[50];
    if(strcmp(s->file_name,"")== 0){
        printf ("ERROR: no file name\n");
        return;
    }
    FILE * file = fopen (s->file_name,"r");
    if (file == NULL){
        printf ("ERROR: can not open '%s'\n",s->file_name);
        return;
    }
    
    //fflush(stdin);
    printf("Please enter <location> <length>:\n");
    //fgets(input,50,stdin);
    //sscanf(input,"%x  %d",&location, &length);
    scanf("%x%d",&location, &length);
    if (s->debug_mode) 
        printf("---\nfile name: %s\nlocation: %X\nlength: %d\n---\n", s->file_name,location,length);
    fseek(file,location,SEEK_SET);
    fread(s->mem_buf,length,s->unit_size,file);
    printf ("Loaded %d units into memory\n", length);
    s->mem_count= length;
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

char* unitToFormatHex(int unit) {
    static char* formats[] = {"%#hhX\n", "%#hX\n", "No such unit", "%#X\n"};
    return formats[unit-1];}
char* unitToFormatDec(int unit) {
    static char* formats[] = {"%hhu\n", "%hu\n", "No such unit", "%u\n"};
    return formats[unit-1];}

void MemoryDisplay (state *s){
    int  unit;
    char* address;
    char input[50];
    printf("Please enter <units> <address>\n");

    //fgets(input,sizeof(input),stdin);
    //sscanf(input,"%d  %p",&unit, &address);
    
    scanf ("%d%p",&unit,&address);
    if (address==0 )
           address = (char*)(s->mem_buf);

    if (s->display_mode == 0){
        printf("Decimal\n=======\n");
        while (unit  > 0) {
        int var = *((int*)(address));
        fprintf(stdout,(unitToFormatDec(s->unit_size)), var);
        address += s->unit_size;
        unit-=1;
        }
    } else {
        printf("Hexadecimal\n=======\n");
        while (unit  > 0) {
        int var = *((int*)(address));
        fprintf(stdout, unitToFormatHex(s->unit_size), var);
        address += s->unit_size;
        unit-=1;
        
        }
    }
}

void SaveIntoFile (state * s){
    int sourceAddress,targetLocation,length;
    printf("Please enter <source-address> <target-location> <length>\n");
    scanf("%x%x%d", &sourceAddress,&targetLocation, &length);
    FILE * file = fopen (s->file_name, "r+");
    if (file == NULL){
        printf ("ERROR: can not write into file\n");
        return;
    }
    fseek(file,0,SEEK_END);
    s->fileLen= ftell(file);
    printf("size: %d\n",s->fileLen);
    fseek(file,0,SEEK_SET);
    if (targetLocation >  s->fileLen )
        printf("Error: target location is greater then the actuall file\n ");
    else{
        fseek (file,targetLocation,SEEK_SET);
        if(sourceAddress==0)
            fwrite(s->mem_buf, s->unit_size, length, file);
        else{
            printf("target lo: %x\n", targetLocation);
            printf("source: %x\n", sourceAddress);
            fseek (file,targetLocation,SEEK_SET);
            fwrite(&sourceAddress, s->unit_size, length, file);
        }
    }
    fclose(file);
}

void MemoryModify(state * s){
   int location, val;
   printf("Please enter <location> <val>\n");
   scanf ("%X %X",&location,&val);
   if (s->debug_mode)
        printf("location: %x\nval: %x\n", location,val);
     for (int i = 0; i < location+6; i++)
     {
         printf("%x",s->mem_buf[i] );
     }
     printf("\n");
    memcpy(&s->mem_buf[location], &val, s->unit_size);
     for (int i = 0; i < location+6; i++)
     {
         printf("%x",s->mem_buf[i] );
     }
     printf("\n");
}


void quit(state* s){
    if (s->debug_mode)
        printf("quitting\n");
    exit(0);
    }


int main(int argc, char **argv){
    int i, input, bound, debugFlag;
    state * s = (state *) malloc (sizeof(state));
    s->unit_size =1;
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
    
    
