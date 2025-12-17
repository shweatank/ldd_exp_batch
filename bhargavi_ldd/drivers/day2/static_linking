#########static and dynamic linking#############



There are two types of librarian creations
One is static library creation 
Second one is dynamic library creation 
Static library creation is done by using the command --->GCC ar rcs library name.a object file names
Here the object files are created by using the command--->GCC - C File name 



Next One is dynamic library creation
To create a dynamic library we use the command next time----->GCC- shared library name.so Object file names
Here the object files are created by using the command----->GCC - C FPIC file name

creating libraries
dy1.c

#include<stdio.h>
x()
{
        printf("inside x function\n");
}
a()
{
        printf("inside a function\n");
}


dy2.c

#include<stdio.h>
y()
{
        printf("inside function y\n");
}
b()
{
        printf("inside function b\n");
}


dy3.c

#include<stdio.h>
int main()
{
        printf("the process id is %d\n",getpid());
        x();
        getchar();
        y();
        getchar();
        a();
        getchar();
        b();
        getchar();

}



static linking(libtest.a)
1)compil without linking
gcc -c dy1.c
gcc -c dy2.c
2)create static library---->ar rcs libtest.a dy1.o dy2.o
3)compile main program with static library-> gcc dy3.c ./libtest.a -o static
4)run----->./static
Here ar means it is a library manager,which is used to combine the object files into a single static library

dynamic linking(libtest.so)
1)compile with -fPIC(position independent code)
gcc -c fPIC dy1.c
gcc -c fPIC dy2.c
2)create dynamic/shared library->gcc -shared -o libtest.so dy1.o dy2.o
3)compile program with shared library---> gcc dy3.c -L. -ltest -o dynamic
4)run--->add current directory to the library path 
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PAH
./dynamic



-L--------->tells gcc where to look for a library file
.l--------->tells gcc which library to link


the main difference is static lib it will be .a
for the dynamic lib it will be .so
 

