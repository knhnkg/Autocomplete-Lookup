/*--------------------------------------------------------------- 
..Project: dict2
  array.c :  
          = the implementation of module sortedArray of the project.
-----------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "array.h"

/**
* Creates and returns an empty array.
* The following function was adapted from W3.5 of Workshop Week 3.
*/
array_t *arrayCreate() {
	array_t *arr = malloc(sizeof(*arr));
	assert(arr);
	int size = INIT_SIZE;
	arr->size = size;
	arr->A = malloc(size * sizeof(*(arr->A)));
	assert(arr->A);
	arr->n = 0;
	return arr;
}

/**
* Ensures that array's size exceeds its number of elements.
* The following function was adapted from W3.5 of Workshop Week 3.
*/
void arrayEnsureSize(array_t *arr) {
	if (arr->n == arr->size) {
		arr->size <<= 1;
		arr->A = realloc(arr->A, arr->size * sizeof(*(arr->A)));
		assert(arr->A);
	}
}

/**
* Inserts a place "data" into sorted array "arr", while ensuring that
* "arr" remains sorted. 
* The following function was adapted from W3.5 of Workshop Week 3.
*/
void sortedArrayInsert(array_t *arr, place_t *p) {
	arrayEnsureSize(arr);

	int i = arr->n-1;
	// Interate through the loop to find the correct position
	while ((i>= 0) && (strcmp(p->tradingName, arr->A[i]->tradingName) < 0)) {
		arr->A[i + 1] = arr->A[i];
		i--;
	}
	arr->A[i + 1] = p;
	arr->n++;
}

/**
* Performs binary search for place with matching "trading name" to the
* input "query" in sorted array "arr".
* Returns the pointer to the found place, NULL if not found
* Also keeps track of and reports: "bitCounter" - # of bits compared
*                                  "charCounter" - # of characters compared
*                                  "strCounter" - # of strings compared
*/
void *findAndTraverse(array_t *arr, char *query) {
	int dictSize = arr->n; // Number of elements in the array
	int strcmp; // 0 for a match and -1 or 1 for a mismatch
	int bitCounter; // Keeps track of number of bits compared
	int charCounter = 0; // Keeps track of number of characters compared
	int strCounter = 0; // Keeps track of number of strings compared

	int mid, srt = 0, end = arr->n - 1;
	while (srt <= end) {
		mid = srt + (end - srt) / 2;

		// Compare the trading name for each data with query input
		strcmp = compareString(arr->A[mid]->tradingName, query, &charCounter);
		strCounter++;
		
		// Write comparison details to the stdout file if a match is found
		if (strcmp == 0) {
			// Also check for adjacent data for more matching place data
			charCounter = charCounter + adjacentValueSearch(arr, query, dictSize, mid, &strCounter);
			bitCounter = charCounter * BYTE;
			fprintf(stdout, "%s --> b%d c%d s%d\n", query, bitCounter, charCounter, strCounter);
			return arr->A[mid];
		}
		// For a mismatch, increment or decrement index for another comparison
		else if (strcmp < 0) {
			srt = mid + 1;
		}
		else {
			end = mid - 1;
		}
	}

	return NULL;
}

/**
* Compares two string inputs character by character to check
* if the two are of the idetical string.
* Also keeps track of the number of characters compared. 
* Returns an integer out of -1, 1 or 0 to indicate whether if 
* the two strings are identical or not.
*/
int compareString(const char *str1, const char *str2, int *charCounter) {
	while (*str1 != '\0' && *str2 != '\0') {
		// Record all character compared regardless of whether the two
		// are identical or not
		(*charCounter)++;
		if (*str1 < *str2) return -1;
		else if (*str1 > *str2) return 1;
		str1++;
		str2++;
	}
	// Increment for the null terminator
	(*charCounter)++;

	// When one string is longer than the the other, it must be a mismatch
	if (*str1 == '\0' && *str2 != '\0') return -1;

	return 0;
}

/**
* Checks for adjacent data with identical trading name to the
* input "query" in sorted array "arr". 
* Returns the number of characters compared. 
* Also keeps track of the number of strings compared.
*/
int adjacentValueSearch(array_t *arr, char *query, int dictSize, int matchIndex, int *strCounter) {
	int counter = 0; // Temporary counter for characters compared

	//Iterate through loop for upper index check
	for (int i = matchIndex - 1; i >= 0; i--) {
		(*strCounter)++;
		if (compareString(arr->A[i]->tradingName, query, &counter) == 0) continue;
		// As data is sorted, stop checking once mismatch found
		else break;
	}

	//Iterate through loop for below index check
	for (int j = matchIndex + 1; j < dictSize; j++) {
		(*strCounter)++;
		if (compareString(arr->A[j]->tradingName, query, &counter) == 0) continue;
		// As data is sorted, stop checking once mismatch found
		else break;
	}

	return counter;
}

/**
* Compares the query to the trading name of each of the data 
* stored in the sorted array "arr" and prints its information 
* if they match.
*/
void arraySearch(array_t *arr, char *query, FILE *outFile) {
	fprintf(outFile, "%s\n", query);
	int counter = 0;

	int i = 0;
	while (i < arr->n) {
		// Write information for place data in outfile if mathces to query
		if (compareString(arr->A[i]->tradingName, query, &counter) == 0) {
			dataWrite(arr->A[i], outFile);
		}
		i++;
	}
}

/**
* Frees the memory allocated for the array "arr".
*/
void arrayFree(array_t *arr) {
	assert(arr != NULL);

	free(arr->A);
	free(arr);
}

