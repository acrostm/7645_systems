#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc == 1 || argc > 3)
    {
        printf("Usage: %s string1 string2\n", argv[0]);
        exit(1);
    }

    printf("%s %s\n", argv[1], argv[2]);
}
