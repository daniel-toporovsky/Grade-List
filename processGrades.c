/**************************************
*  Source file for the processing grades
*  code module which reads a list of course
*  participants and their grades from a file,
*  and then it prints a summary to the standard output.
**************************************/
#include "gradeList.h"
#include <stdio.h>
#include <string.h>

/*************************************/
/**       SYMBOLIC CONSTANTS        **/
/*************************************/
#define MAX_LINE_LENGTH 200

/*************************************/
/**      FUNCTION  DECLARATIONS      **/
/*************************************/
/*********************************
* getNextNumberValue
* params:
* array (char*)
* Returns the pointer to
* the next word in array.
* If next word or array is empty string returns NULL.
* Uses static variables to keep information
* from prev execution
*********************************/
char* getNextWord(char* array);

/*********************************
* getNumberFromString
* params:
* string (char*)
* Returns the value (in type int) of
* the first number in array.
* If the 1st word is not a valid number,
* returns 101.
*********************************/
int getNumberFromString(char* string);

int main (int argc, char *argv[]) {
    // Checking for correct input
    if (argc == 1) {
        fprintf (stderr, "Error: no file name provided\n");
        return 1;
    }
    if (argc > 2) {
        fprintf (stderr, "Error: expecting only one input argument\n");
        return 2;
    }
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        fprintf (stderr, "Error: %s cannot be open for reading\n", argv[1]);
        return 3;
    }

    // Iterating over file and appending new nodes
    int exitStatus = 0;
    GradeNode gradeList = NULL;
    int line = 0;
    char input[MAX_LINE_LENGTH+2] = {'\0'};
    while (fgets(input, MAX_LINE_LENGTH+1, file)) {
        char *firstName = getNextWord(input);
        char *lastName = getNextWord(input);
        int grade = getNumberFromString(getNextWord(input));
        getNextWord("");            // Resetting global variables
        line++;

        if (grade > 100 || !firstName || !lastName) {
            fprintf(stderr, "Line %i has faulty format\n", line);
            exitStatus = 4;
        }
        else {
            GradeNode curr = newGradeNode(firstName, lastName, grade);
            if (curr) gradeList = appendGradeNode(gradeList, curr);
            else {
                fprintf(stderr, "Out of memory after reading %i lines\n", line);
                freeGradeList(curr);
                freeGradeList(gradeList);
                fclose(file);
                return 11;
            }
        }
    }

    GradeNode sorted = mergeSortGradeList(gradeList);
    printGradeList(sorted);
    freeGradeList(sorted);
    fclose(file);
    return exitStatus;
}

/*************************************/
/**      FUNCTION  DEFINITIONS      **/
/*************************************/
/*********************************
* getNextWord
* params:
* array (char*)
* Returns the pointer to
* the next word in array.
* If next word or array is empty string returns NULL.
* Uses static variables to keep information
* from prev execution
*********************************/
char* getNextWord(char* array) {
    static char* prevPointer   = NULL;
    static char* nextPointer = NULL;

    if (array == prevPointer) {
        if (!*nextPointer) return NULL;

        array = nextPointer;
        int i = 0;
        while (nextPointer[i] != '\n' && nextPointer[i] != ' ' && nextPointer[i]) i++;
        if (nextPointer[i] == ' ') nextPointer[i++] = '\0';
        while (nextPointer[i] == ' ') i++;
        nextPointer = &nextPointer[i];
        return array;
    }
    else {
        prevPointer = array;
        if (!*array) return NULL;

        // Skip leading spaces
        while (*array == ' ') {
            array++;
        }

        // Check for empty string after skipping spaces
        if (!*array) return NULL;

        // Save the next value
        int i = 0;
        while (array[i] != '\n' && array[i] != ' ' && array[i]) i++;
        if (array[i] == ' ') array[i++] = '\0';
        while (array[i] == ' ') i++;
        nextPointer = &array[i];
        return array;
    }
}

/*********************************
* getNumberFromString
* params:
* string (char*)
* Return the value (in type char) of
* the first number in array.
* If 1st word is not a valid number,
* or the array is empty returns 101.
*********************************/
int getNumberFromString(char* string) {
    int value = 0;

    // Check for empty list
    if (!*string) return 101;

    // If the first character is not a digit the - invalid number
    if (*string < '0' || *string > '9') return 101;

    // Parse the number
    while (*string >= '0' && *string <= '9') {
        value = value * 10 + (*string - '0');
        string++;
    }

    // If the next char is invalid - the number is invalid
    if (*string != '\n' && *string != ' ' && *string) return 101;

    return value;
}
