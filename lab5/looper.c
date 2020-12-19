#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void signalHendler(int sig){
	char *sign;
	sign = strsignal(sig);
	printf("signal recived: %s\n", sign);
	signal(sig, SIG_DFL);
	if (sig ==SIGCONT)
	{
		signal(SIGTSTP, signalHendler);
	}
	if (sig ==SIGTSTP)
	{
		signal(SIGCONT, signalHendler);
	}
	raise(sig);
}
int main(int argc, char **argv){ 

	printf("Starting the program\n");

	while(1) {
		sleep(2);
		signal(SIGTSTP, signalHendler);
		signal(SIGINT, signalHendler);
		signal(SIGCONT, signalHendler);
	}

	return 0;
}