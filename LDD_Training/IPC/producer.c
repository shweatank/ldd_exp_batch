#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

void Custom_Sig(int signum) {
	if(signum == SIGINT) {
		printf("Signal INT Arrived\n");
	}
	else if(signum == SIGTSTP) {
		printf("Signal STOP Arrived\n");
	}
	return;
}

int main() {
	printf("my pid = %d\n", getpid());
	signal(SIGINT, SIG_IGN);

	signal(SIGTSTP, SIG_IGN);
//	signal(2, SIG_IGN);
//	signal(2, SIG_DFL);
	while(1);
	return 0;
}
