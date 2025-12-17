#include<stdio.h>

struct cal{
	int (*ADD)(int x, inty);
	int (*SUB)(int x, inty);
	int (*MUL)(int x, inty);
	int (*DIV)(int x, inty);
};

int add(int x, int y){
	return (x+y);
}


int sub(int x, int y){
        return (x-y);
}

int mul(int x, int y){
        return (x*y);
}

int div(int x, int y){
        return (x/y);
}


int main(){
	struct calculator{
		ADD=add;
		SUB=sub;
		MUL=mul;
		DIV=div;
	}

}
