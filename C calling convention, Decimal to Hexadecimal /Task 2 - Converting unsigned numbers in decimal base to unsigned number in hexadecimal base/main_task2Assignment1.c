#include <stdio.h>
#define	MAX_LEN 34			/* maximal input string size */
					/* enough to get 32-bit string + '\n' + null terminator */
extern int convertor(char* buf);

int main(int argc, char** argv)
{ int i=0;
  char buf[MAX_LEN ];
  while (i==0)
  {
    fgets(buf, MAX_LEN, stdin);		/* get user input string */ 
    if (buf[0]=='q')
    {
      break;
    }
    if(buf[0]< '0' || buf[0]> '9'){
      printf("illegal input\n");
      break;
    }
    else
    {
      convertor(buf);			/* call your assembly function */
    }
  }  

  return 0;
}