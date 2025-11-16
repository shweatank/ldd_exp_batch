#include <stdio.h>
#include <signal.h>

int ed(){
	return 0;
}

void sssend(){
	printf("sig int hit\n");	
}

void ssstop(){
	printf("sig int hit\n");	
	signal(SIGKILL, (void*)ed);
}

int main(){
	
	signal(SIGINT, sssend);
	signal(SIGTSTP, ssstop);
	while(1){};
	
}
