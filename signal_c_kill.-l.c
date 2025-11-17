#include <signal.h>
#include <stdio.h>

void invokeFunc(){
    printf("Function invoked ctrl+c occured.\n");
}

int main()
{

    
    // signal(2,invokeFunc);
    // signal(SIGINT,invokeFunc);
    // signal(SIGSTOP, invokeFunc);

    while(1);
    

    return 0;
}
