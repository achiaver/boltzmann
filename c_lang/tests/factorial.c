#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int num = atoi(argv[1]);
    int j = 1;

    for (int i = 1; i < num; i++)
    {
        j  = j * i;
    }
    printf("The factorial of %d is %d\n", num, j);
    return 0;
}
