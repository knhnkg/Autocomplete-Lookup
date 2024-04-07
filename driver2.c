/*--------------------------------------------------------------- 
..Project: dict2
  driver2.c : 
             = the main program for 'Stage 2' of the project 

* Created by Hyunkyung Kwon - 1346599 (hyunkyung@student.unimelb.edu.au)
18/09/2023
*
* Autocomplete dictionary that stores information from a 
* dataset about the City of Melbourne.
* Allows the lookup of data by partial key (trading_name).
*
* To run the program type: 
* ./dict2 2 tests/dataset_1000.csv output.txt
*
* Command line input parameters:
*   ./dict2                   Program name
*   2                         Stage
*   tests/dataset_1000.csv    Input data file
*   output.txt                Output data file
-----------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "data.h"
#include "list.h"
#include "array.h"

/*------------------ Function definitions -----------------------*/
array_t *getPlaceArray (list_t *list);
list_t *getPlaceList (FILE *inFile);
void stage2Querying (FILE *queryInput, FILE *terminalOutput, FILE *inFile, FILE *outFile);


int main(int argc, char **argv) {
    FILE *inFile = fopen(argv[2], "r");
    assert(inFile);
    FILE *outFile = fopen(argv[3], "w");
	assert(outFile);

    stage2Querying(stdin, stdout, inFile, outFile);
	
    fclose(inFile);
    fclose(outFile);

    return 0;
}

/**
* Stores the pointer to place data from the linked list of place data. 
* Returns sorted array of places.
*/
array_t *getPlaceArray (list_t *list) {
    array_t *placesArr = arrayCreate();

    int i = 0;
    node_t *curr = list->head;
    while(curr) {
        sortedArrayInsert(placesArr, curr->data);
        curr = curr->next;
        i++;
    }

    return placesArr;
}

/**
* Reads and stores place data from the data input file. 
* Returns linked list of places.
* The following function was adapted from W3.8 of Workshop Week 3.
*/
list_t *getPlaceList (FILE *inFile) {
    // Creates a linked list to store place data
    list_t *places = listCreate();

    placeSkipHeaderLine(inFile);

    // Reads and stores data for each place until the end of the data input file
    place_t *p;
	while ((p = placeRead(inFile))) {
		listAppend(places, p);
	}

    return places;
}

/**
* Reads each quey input and search for matching data stored in the sorted array "placesArr".
*/
void stage2Querying (FILE *queryInput, FILE *terminalOutput, FILE *inFile, FILE *outFile) {
    list_t *placesList = getPlaceList(inFile);
    array_t *placesArr = getPlaceArray(placesList);

    char *query = NULL;
    size_t len = 0;
    while (getline(&query, &len, queryInput) != -1) {
        // For each quey string, removes the newline character and terminate the string
        int len = strlen(query);
        if (len > 0 && query[len - 1] == '\n') {
            query[len - 1] = '\0';
        }

        findAndTraverse(placesArr, query);
        arraySearch(placesArr, query, outFile);
    }

    free(query);
    listFree(placesList);
    arrayFree(placesArr);
}

