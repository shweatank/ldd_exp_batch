#include <stdio.h>

int a = 100;   // global variable

void display() {
    extern int a;   // using global variable
    printf("extern variable a = %d\n", a);
}

int main() {
    display();
    return 0;
}

