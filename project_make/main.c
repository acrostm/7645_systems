/*
* Demonstrates the usage of the swap function
* Author: Sourav Mukherjee, sourav@fdu.edu
*/

#include <stdio.h>
#include <stdlib.h>

#include "swap2.h"
#include "person.h"

int main()
{
    int firstInteger, secondInteger;
    Person firstPerson = {"Adam", "Anthony"};
    Person secondPerson = {"Bob", "Bell"};

    firstInteger = 1;
    secondInteger = 2;

    printf("------------------------------\n");
    printf("BEFORE SWAP\n");
    printf("------------------------------\n");


    printf("first integer = %d, second integer = %d\n", firstInteger, secondInteger);
    printf("first person = %s %s, ", firstPerson.firstName, firstPerson.lastName);
    printf("second person = %s %s\n", secondPerson.firstName, secondPerson.lastName);

    printf("------------------------------\n");
    printf("AFTER SWAP\n");
    printf("------------------------------\n");

    swap(&firstInteger, &secondInteger, sizeof(int));

    printf("first integer = %d, second integer = %d\n", firstInteger, secondInteger);

    swap(&firstPerson, &secondPerson, sizeof(Person));

    printf("first person = %s %s, ", firstPerson.firstName, firstPerson.lastName);
    printf("second person = %s %s\n", secondPerson.firstName, secondPerson.lastName);

    exit(0);

}