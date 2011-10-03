#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (){
    char a[2] = "A";
    char b[2] = "B";
    int i = 0;

    printf ("%d\n", (i + 1 + 1));
    strcat (a, b);

    printf ("%s\n", a);

    return EXIT_SUCCESS;
}
