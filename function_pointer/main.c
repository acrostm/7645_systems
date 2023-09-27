/* Demonstrates how to implement the visitor pattern in C using function pointers
* Author: Sourav Mukherjee, sourav@fdu.edu
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string_functions.h"

void visitStrings(
    char** arrayOfStrings, /* A char* is a string (array of char), a char** is an array of strings */
    size_t countOfStrings, /* Number of strings */
    void (*visit)(char* input) /* The visitor for each string */)
{
    int index;

    for (index = 0; index < countOfStrings; index++)
    {
        visit(arrayOfStrings[index]);
    }
}

int main(int argc, char** argv)
{
    char** arrayOfStrings;
    int index, countOfStrings, arraySize;

    if (argc == 1)
    {
        printf("Usage : %s string1 string 2 ...\n", argv[0]);
        exit(1);
    }

    countOfStrings = argc - 1;

    arrayOfStrings = (char**)malloc(countOfStrings * sizeof(char*));
    for (index = 0; index < countOfStrings; index++)
    {
      arraySize = strlen(argv[index + 1]) + 1; /* strlen does not include the end of string character \0 */
      arrayOfStrings[index] = (char*)malloc(arraySize * sizeof(char));
      memcpy(arrayOfStrings[index], argv[index + 1], arraySize);
    }

    visitStrings(arrayOfStrings, countOfStrings, stringToUpper);
    for (index = 0; index < countOfStrings; index++)
    {
      printf("To upper: %s\n", arrayOfStrings[index]);
    }

    visitStrings(arrayOfStrings, countOfStrings, stringToLower);
    for (index = 0; index < countOfStrings; index++)
    {
      printf("To lower: %s\n", arrayOfStrings[index]);
    }

    for(index = 0; index < countOfStrings; index++)
    {
      free(arrayOfStrings[index]);
    }

    free(arrayOfStrings);

    exit(0);
}