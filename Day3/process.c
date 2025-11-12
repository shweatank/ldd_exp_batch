#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/wait.h>
int main()
{
pid_t pid=fork();
int y=433;
if(pid==0)
{
    y=440;
    printf("child process created successfullyi:--%d\n",&y);
    printf("parent id:--%d child id:--%d\n",getppid(),getpid());
    //replace child with new program.
    execlp("ls","ls","-l",NULL);
    perror("execlp fails\n");
}
else if(pid<0)
{
    printf("child creation is successfully failed\n");
    return -1;
}
else
{
    printf("parent is waiting for child:--%d\n",pid);
    wait(NULL);
    printf("Child Compleated.parent exiting\n");
}
pid=fork();
if(pid==0)
{
    printf("child process created successfully:--%d\n",&y);
    printf("parent id:--%d child id:--%d\n",getppid(),getpid());
    //replace child with new program.
    execlp("./hello","hello","SAI",NULL);
    perror("execlp fails\n");
}
else if(pid<0)
{
    printf("child creation is successfully failed\n");
    return -1;
}
else
{
    printf("parent is waiting for child:--%d\n",pid);
    wait(NULL);
    printf("Child Compleated.parent exiting\n");
}
return 0;
}

