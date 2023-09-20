#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void stringModify(char* string, int (*visit)(int))
{
    for (int i = 0; string[i] != '\0'; i++)
    {
        string[i] = visit(string[i]);
    }
}

int main(int argc, char** argv)
{
    if (argc == 1 || argc > 3)
    {
        printf("Usage: %s string1 <upper/lower>\n", argv[0]);
        exit(1);
    }

    printf("Original String: %s\n", argv[1]);

    int (*modifier)(int);

    if (strcmp(argv[2], "upper") == 0)
    {
        modifier = toupper;
    }
    else if (strcmp(argv[2], "lower") == 0)
    {
        modifier = tolower;
    }
    else
    {
        printf("Usage: %s string1 <upper/lower>\n", argv[0]);
        exit(1);
    }

    stringModify(argv[1], modifier);
    printf("Altered String: %s\n", argv[1]);

    return 0;
}
