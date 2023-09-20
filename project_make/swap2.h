#pragma once /* Makes sure this file is included at most once */

/*
* Declares the swap function
* Author: Sourav Mukherjee, sourav@fdu.edu
*/

#include <stddef.h> /* For size_t */

void swap(void* first, void* second, size_t objectSize);