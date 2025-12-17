/*
 * calculator_struct.c
 *
 * Calculator using a struct of function pointers (like file_operations)
 */

#include <stdio.h>

/* ---------------------------------
   Calculator Operation Functions
   --------------------------------- */

int add(int a, int b) {
    printf("Add: %d + %d = %d\n", a, b, a + b);
    return a + b;
}

int sub(int a, int b) {
    printf("Sub: %d - %d = %d\n", a, b, a - b);
    return a - b;
}

int mul(int a, int b) {
    printf("Mul: %d * %d = %d\n", a, b, a * b);
    return a * b;
}

float divi(int a, int b) {
    if (b == 0) {
        printf("Cannot divide by zero!\n");
        return 0;
    }
    float result = (float)a / b;
    printf("Div: %d / %d = %.2f\n", a, b, result);
    return result;
}

/* ---------------------------------
   Struct holding calculator APIs
   --------------------------------- */

struct calc_ops {
    int   (*add)(int, int);
    int   (*sub)(int, int);
    int   (*mul)(int, int);
    float (*divi)(int, int);
};

/* Initialize the operations table */
struct calc_ops calculator = {
    .add  = add,
    .sub  = sub,
    .mul  = mul,
    .divi = divi
};

/* ---------------------------------
   Demo / Test
   --------------------------------- */

int main(void)
{
    int a = 20, b = 5;

    calculator.add(a, b);
    calculator.sub(a, b);
    calculator.mul(a, b);
    calculator.divi(a, b);

    return 0;
}

