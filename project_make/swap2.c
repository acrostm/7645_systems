/*
* Implements the swap function
* Author: Sourav Mukherjee, sourav@fdu.edu
*/

#include "swap2.h"
#include <string.h> /* For memcpy */
#include <stdlib.h> /* For free */

void swap(void* first, void* second, size_t objectSize)
{
    char* temp = (char *)malloc(objectSize);

    memcpy(temp, first, objectSize);
    memcpy(first, second, objectSize);
    memcpy(second, temp, objectSize);

    free(temp);
  }