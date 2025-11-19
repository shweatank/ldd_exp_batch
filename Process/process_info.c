/*
Print process info:
Process ID (PID)
Parent Process ID (PPID)
User ID (UID)
Effective User ID (EUID)
*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    printf("PID   = %d\n", getpid());
    printf("PPID  = %d\n", getppid());
    printf("UID   = %d\n", getuid());
    printf("EUID  = %d\n", geteuid());
    printf("GID   = %d\n", getgid());
    printf("EGID  = %d\n", getegid());

	char cwd[100];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		printf("CWD = %s\n", cwd);
	}
	return 0;
}

