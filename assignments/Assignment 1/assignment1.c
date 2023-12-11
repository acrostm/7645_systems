#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int computeSquare(int number)
{
    return number*number;
}

int computeCube(int number)
{
    return number*number*number;
}


void transformArrayOfNumbers(int *numbers, int numberCount, int (*visit)(int)/* TODO: Add arguments so that this can be called from main() */)
{
    /*
        TODO:
        Apply a visitor (e.g., computeSquare or computeCube) to transform each element of an integer array
    */

   for (int i = 0; i < numberCount; i++)
   {
       numbers[i] = visit(numbers[i]);
   }
    
}


int main(int argc, char* argv[])
{
    int* numbers;
    int numberCount;
    int index;

    /* 
        TODO:
        Handle command line arguments per the given requirements
    */
   if (argc == 1 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
    {
        printf("Usage: %s <mode> <number_1> <number_2> ... <number_n>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    numberCount = argc - 2;/* TODO: calculate length of the array */
    numbers = (int*)malloc(numberCount * sizeof(int)); /* TODO: allocate the array and check if it fails */
    /* Hints:
        (1) If the size of each integer is x bytes and the number of integers is y
        then the array needs x*y bytes.
        (2) malloc returns void*, therefore the (int*) casting is required.
    */

    /* 
        TODO:
        Populate the integer array based on command line arguments
    */
    for (int i = 0; i < numberCount; i++)
    {
        numbers[i] = atoi(argv[i+2]);
        
        if (numbers[i] < 1)
        {
            printf("the minimum value of n is 1\n");
            exit(EXIT_FAILURE);
        }
    }

    if (strcmp(argv[1], "square") == 0)
    {
        transformArrayOfNumbers(
            numbers, 
            numberCount, 
            computeSquare);
    }

    else if (strcmp(argv[1], "cube") == 0)
    {
        transformArrayOfNumbers(
            numbers, 
            numberCount,
            computeCube);
    }

    else
    {
        printf("mode must be square or cube.\n");
        exit(EXIT_FAILURE);
    }

    /*
        TODO:
        Print out the transformed integers, one integer per line
    */
   for (int i = 0; i < numberCount; i++)
   {
         printf("%d\n", numbers[i]);
   }

   free(numbers);
    /* TODO: Deallocate the array */
    exit(EXIT_SUCCESS);
}