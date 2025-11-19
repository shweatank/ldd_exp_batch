//Using syscall to print directly to console
#include <stdio.h>
#include <unistd.h>

int main(){
	write(1,"devashree",9);
}
