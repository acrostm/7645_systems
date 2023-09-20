/* Implements functions for converting strings to uppercase and lowercase
* Author: Sourav Mukherjee, sourav@fdu.edu
*/

#include <ctype.h> /* For tolower and toupper */
#include "string_functions.h"

void stringToUpper(char* input)
{
    int index;

    for (index = 0; input[index] != '\0'; input++){
        input[index] = toupper(input[index]);
    }
}

void stringToLower(char* input)
{
    int index;

    for(index = 0; input[index] != '\0'; input++){
        input[index] = tolower(input[index]);
    }
}