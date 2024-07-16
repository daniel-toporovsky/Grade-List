/**************************************
*  Source file for the grade list
*  code module which maintains a list
*  of course participants and their grades.
**************************************/
#include "gradeList.h"
#include "virtualHeap.h"
#include <stdio.h>
#include <string.h>

/********************************/
/**      TYPE DEFINITIONS      **/
/********************************/
/*** struct type for node in grade list ***/
typedef struct GradeNode_st GradeNode_st;

/*************************************/
/**      FUNCTION  DEFINITIONS      **/
/*************************************/

/*********************************
* Problem 1.1
* function newGradeNode
* params:
* firstName (char*)
* lastName  (char*)
* grade     (int)
* If grade is out of bounds (not in [0,100]),
* then do nothing and return NULL
* Otherwise crate new GradeNode object, which
* includes the strings for first and last names
* allocated outside of the struct.
*********************************/
GradeNode newGradeNode(const char* firstName, const char* lastName, int grade) {
    if (grade < 0 || grade > 100) return NULL;

    // Allocating memory for the object
    GradeNode v;
    v = ourMalloc(sizeof(GradeNode_st));
    if (!v) return NULL;

    /*
     * Allocating memory for the copies of variables
     * that are stored outside the structure.
    */
    v->firstName = ourMalloc(strlen(firstName) * sizeof (char) + 1);
    v->lastName = ourMalloc(strlen(lastName) * sizeof (char) + 1);
    if (!v->firstName || !v->lastName) {
        if (v->firstName) ourFree(v->firstName);
        if (v->lastName) ourFree(v->lastName);
        ourFree(v);
        return NULL;
    }

    // Copying data to the new object
    strcpy(v->lastName, lastName);
    strcpy(v->firstName, firstName);
    v->grade = (char)grade;
    v->next = NULL;
    return v;
}

/*********************************
* Problem 1.2
* function freeGradeList
* params:
* gradeList (GradeNode)
* frees a given grade node and all nodes in
* the list that is "downstream" from it.
* frees all memory - the structs and the strings
* allocated separately
*********************************/
void freeGradeList(GradeNode gradeList) {
    if (!gradeList) return;

    if (gradeList->next) freeGradeList(gradeList->next);
    ourFree(gradeList->firstName);
    ourFree(gradeList->lastName);
    ourFree(gradeList);
}

/*********************************
* Problem 1.3
* function largerGradeNode
* params:
* gradeNode1 (GradeNode)
* gradeNode2 (GradeNode)
* compares the two grade nodes and
* returns a pointer to the grade
* node that is "larger".
* First compare grades,
* Then compare last names,
* Then compare first names.
* If nodes contain identical
* information, then returns
* pointer to gradeNode1
*********************************/
GradeNode largerGradeNode(GradeNode gradeNode1, GradeNode gradeNode2) {
    if (gradeNode1->grade > gradeNode2->grade) return gradeNode1;           // grade1 != grade2
    else if (gradeNode1->grade < gradeNode2->grade) return gradeNode2;
    else {                                                                  // grade1 == grade2
        int lastNameCompare = strcmp(gradeNode1->lastName, gradeNode2->lastName);
        if (lastNameCompare > 0) return gradeNode1;                         // lastName1 != lastName2
        else if (lastNameCompare < 0) return gradeNode2;
        else {                                                              // lastName1 == lastName2
            int firstNameCompare = strcmp(gradeNode1->firstName, gradeNode2->firstName);
            if (firstNameCompare >= 0) return gradeNode1;                   // firstName1 >= firstName2
            else return gradeNode2;
        }
    }
}

/*********************************
* Problem 1.4
* function printGradeList
* params:
* gradeList (GradeNode)
* prints a grade list by printing each node
* in the following format:
* fistName<TAB>lastName<TAB>grade
* If list is empty then just prints:
* Empty grade list
* Otherwise, after printing all nodes, a summary line is printed:
* Average of <N> grades is <AVG>
* where N is the number of grades in the list
* and <AVG> is the average of grades printed
* with 1 decimal digit precision
*********************************/
void printGradeList(GradeNode gradeList) {
    if (!gradeList) {
        printf("Empty grade list\n");
        return;
    }

    GradeNode curr = gradeList;
    double gradeSum = 0;
    int count = 0;
    while (curr) {
        printf("%s\t%s\t%i\n", curr->firstName, curr->lastName, curr->grade);
        gradeSum += curr->grade;
        count++;
        curr = curr->next;
    }
    printf("Average of %i grades is %.1f\n" , count, (gradeSum/count));
}

/*********************************
* Problem 1.5
* function appendGradeNode
* params:
* gradeList (GradeNode)
* gradeNode (GradeNode)
* adds gradeNode to the end of gradeList
* If gradeNode is not a single node, then does not
* modify gradeList.
* Returns the modified list.
*********************************/
GradeNode appendGradeNode(GradeNode gradeList, GradeNode gradeNode) {
    // If the node is not empty and single in the list
    if (gradeNode && !gradeNode->next) {
        if (!gradeList) {
            gradeList = gradeNode;
        }
        else {
            GradeNode curr = gradeList;
            while (curr->next) curr = curr->next;
            curr->next = gradeNode;
        }
    }
    return gradeList;
}

/*********************************
* Problem 1.6
* function mergeSortedGradeLists
* params:
* gradeList1 (GradeNode)
* gradeList2 (GradeNode)
* receives two grade lists sorted from "small"
* to "large" based on largerGradeNode()
* and merges them so that sorting is maintained
* Returns the merged list.
* Assume input lists are sorted.
*********************************/
GradeNode mergeSortedGradeLists(GradeNode gradeList1, GradeNode gradeList2) {
    // Recursion base
    if (!gradeList1) return gradeList2;
    if (!gradeList2) return gradeList1;

    // Starting from the smallest node and merging recursively
    if (largerGradeNode(gradeList1, gradeList2) == gradeList1) {
        gradeList2->next = mergeSortedGradeLists(gradeList2->next, gradeList1);
        return gradeList2;
    }

    gradeList1->next = mergeSortedGradeLists(gradeList1->next, gradeList2);
    return gradeList1;
}

/*********************************
* Problem 1.7
* function mergeSortGradeList
* params:
* gradeList (GradeNode)
* sort a given grade list from "small"
* to "large" based on largerGradeNode()
* using the merge-sort algorithm
*********************************/
GradeNode mergeSortGradeList(GradeNode gradeList) {
    // Recursion base case ot null input
    if (!gradeList || !gradeList->next) return gradeList;

    // Counting elements of the Grade List
    int count = 0;
    GradeNode curr = gradeList;
    while (curr) {
        count++;
        curr = curr->next;
    }

    // Splitting Grade List into two
    curr = gradeList;
    for (int i = 1; i < count/2; ++i) {
        curr = curr->next;
    }
    GradeNode gradeList1 = gradeList;
    GradeNode gradeList2 = curr->next;
    curr->next = NULL;

    // Recursively sort two Grade Lists and merge them together
    gradeList1 = mergeSortGradeList(gradeList1);
    gradeList2 = mergeSortGradeList(gradeList2);
    return mergeSortedGradeLists(gradeList1, gradeList2);
}
