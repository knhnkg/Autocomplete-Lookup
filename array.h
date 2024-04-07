/*---------------------------------------------------------------- 
..Project: dict2
  array.h :  
          = the interface of module sortedArray of the project.
------------------------------------------------------------------*/


#ifndef _ARRAY_H_
#define _ARRAY_H_
#include "data.h"

/*------------ Constant definitions ------------------------------*/
#define INIT_SIZE 2     // Initial size of array
#define BYTE 8          // 1 byte = 8 bits

/*------------ Data definitions ----------------------------------*/
struct array {
	place_t **A;        // Array of data for places
	int size;           // Current array size
	int n;              // Current number of elements used
};
typedef struct array array_t;

/*------------ Function definitions -----------------------------*/
/**
* Creates and returns an empty array.
* The following function was adapted from W3.5 of Workshop Week 3.
*/
array_t *arrayCreate();

/**
* Ensures that array's size exceeds its number of elements.
* The following function was adapted from W3.5 of Workshop Week 3.
*/
void arrayEnsureSize(array_t *arr);

/**
* Inserts a place "data" into sorted array "arr", while ensuring that
* "arr" remains sorted. 
* The following function was adapted from W3.5 of Workshop Week 3.
*/
void sortedArrayInsert(array_t *arr, place_t *p);

/**
* Performs binary search for place with matching "trading name" to the
* input "query" in sorted array "arr".
* Returns the pointer to the found place, NULL if not found
* Also keeps track of and reports: "bitCounter" - # of bits compared
*                                  "charCounter" - # of characters compared
*                                  "strCounter" - # of strings compared
*/
void *findAndTraverse(array_t *arr, char *query);

/**
* Compares two string inputs character by character to check
* if the two are of the idetical string.
* Also keeps track of the number of characters compared. 
* Returns an integer out of -1, 1 or 0 to indicate whether if 
* the two strings are identical or not.
*/
int compareString(const char *str1, const char *str2, int *charCounter);

/**
* Checks for adjacent data with identical trading name to the
* input "query" in sorted array "arr". 
* Returns the number of characters compared. 
* Also keeps track of the number of strings compared.
*/
int adjacentValueSearch(array_t *arr, char *query, int dictSize, int matchIndex, int *strCounter);

/**
* Compares the query to the trading name of each of the data 
* stored in the sorted array "arr" and prints its information 
* if they match.
*/
void arraySearch(array_t *arr, char *query, FILE *outFile);

/**
* Frees the memory allocated for the array "arr".
*/
void arrayFree(array_t *arr);

#endif

