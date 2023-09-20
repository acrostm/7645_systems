/********************************************************
 * Examples of type specific and generic swap functions
 * Author: Sourav Mukherjee, sourav@fdu.edu
 ********************************************************/

#include<stdio.h> /* For standard input/output */
#include<stdlib.h> /* For exit */
#include<string.h> /* For memcpy */

void swapIntegers(int* x, int* y)
{
    int temp;
    
    temp = *x;
    *x = *y;
    *y = temp;
}

typedef struct 
{
    char firstName[20];
    char lastName[20];
} Person;

void swapPersons(
    Person* firstPerson, 
    Person* secondPerson)
{
    Person temp;
    
    temp = *firstPerson;
    *firstPerson = *secondPerson;
    *secondPerson = temp;    
}

void swap(void* first, void* second, size_t objectSize)
{
    char* temp = (char *)malloc(objectSize);

    memcpy(temp, first, objectSize);
    memcpy(first, second, objectSize);
    memcpy(second, temp, objectSize);

    free(temp);
}

int main()
{
    int firstInteger, secondInteger;
    Person firstPerson = {"Adam", "Anthony"};
    Person secondPerson = {"Bob", "Bell"};

    firstInteger = 1;
    secondInteger = 2;

    printf("------------------------------\n");
    printf("BEFORE FIRST SWAP\n");
    printf("------------------------------\n");


    printf("first integer = %d, second integer = %d\n", firstInteger, secondInteger);
    printf("first person = %s %s, ", firstPerson.firstName, firstPerson.lastName);
    printf("second person = %s %s\n", secondPerson.firstName, secondPerson.lastName);
    
    printf("------------------------------\n");
    printf("AFTER FIRST SWAP\n");
    printf("------------------------------\n");

    swapIntegers(&firstInteger, &secondInteger);

    printf("first integer = %d, second integer = %d\n", firstInteger, secondInteger);

    swapPersons(&firstPerson, &secondPerson);

    printf("first person = %s %s, ", firstPerson.firstName, firstPerson.lastName);
    printf("second person = %s %s\n", secondPerson.firstName, secondPerson.lastName);

    printf("------------------------------\n");
    printf("AFTER SECOND SWAP\n");
    printf("------------------------------\n");

    swap(&firstInteger, &secondInteger, sizeof(int));

    printf("first integer = %d, second integer = %d\n", firstInteger, secondInteger);

    swap(&firstPerson, &secondPerson, sizeof(Person));  

    printf("first person = %s %s, ", firstPerson.firstName, firstPerson.lastName);
    printf("second person = %s %s\n", secondPerson.firstName, secondPerson.lastName);
  
    exit(0);

}