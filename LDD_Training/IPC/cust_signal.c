#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

void Custom_Sig(int signum) {
	if(signum == SIGINT) {
		printf("Signal Arrived\n");
		printf("Secret Message: Hello\n");
	}
	else if(signum == SIGTSTP) {
		printf("Signal STOP Arrived\n");
	}
	return;
}

int main() {
	printf("my pid = %d\n", getpid());
	signal(SIGINT, Custom_Sig);

	signal(SIGTSTP, Custom_Sig);
//	signal(2, SIG_IGN);
//	signal(2, SIG_DFL);
	while(1);
	return 0;
}
