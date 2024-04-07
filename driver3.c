/*--------------------------------------------------------------- 
..Project: dict3
  driver3.c : 
             = the main program for 'Stage 3' of the project 

* Created by Hyunkyung Kwon - 1346599 (hyunkyung@student.unimelb.edu.au)
18/09/2023
*
* Autocomplete dictionary that stores information from a 
* dataset about the City of Melbourne.
* Allows the lookup of data by partial key (trading_name).
*
* To run the program type: 
* ./dict3 3 tests/dataset_1000.csv output.txt
*
* Command line input parameters:
*   ./dict3                   Program name
*   3                         Stage
*   tests/dataset_1000.csv    Input data file
*   output.txt                Output data file
-----------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "data.h"
#include "list.h"
#include "rt.h"

/*------------------ Function definitions -----------------------*/
list_t *getPlaceList (FILE *inFile);
void stage3Querying (FILE *queryInput, FILE *terminalOutput, FILE *inFile, FILE *outFile);


int main(int argc, char **argv) {
    FILE *inFile = fopen(argv[2], "r");
    assert(inFile);
    FILE *outFile = fopen(argv[3], "w");
	assert(outFile);

    stage3Querying(stdin, stdout, inFile, outFile);
	
    fclose(inFile);
    fclose(outFile);

    return 0;
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
* Converts trading name into 8-bit binary representation and store place data in radix tree.
* Reads each quey input and search for matching data in the radix tree "placesTree".
* Finally, calculates and writes number of bit, char and string comparisons. 
*/
void stage3Querying (FILE *queryInput, FILE *terminalOutput, FILE *inFile, FILE *outFile) {
    list_t *placesList = getPlaceList(inFile);
    rt_t *placesTree = NULL;

    // Insert places into radix tree 
    char *key;
    node_t *curr = placesList->head;
    while(curr) {
        key = stringToBinary(curr->data->tradingName, KEY);
        placesTree = rtInsert(placesTree, key, curr->data);
        curr = curr->next;
    }

    char *queryBinary;
    char *query = NULL;
    size_t len = 0;
    while (getline(&query, &len, queryInput) != -1) {
        // For each quey string, removes the newline character and terminate the string
        int len = strlen(query);
        if (len > 0 && query[len - 1] == '\n') {
            query[len - 1] = '\0';
        }

        fprintf(outFile, "%s\n", query);
        queryBinary = stringToBinary(query, QUERY);

        int bits = 0;
        int link = 0;
        int commonChar = 0;
        int strings = 0;

        findAndTraverse(placesTree, queryBinary, &bits, &link, &commonChar, &strings, outFile);
        int bytes = link + strlen(query) - commonChar/BYTE;
        fprintf(terminalOutput, "%s --> b%d c%d s%d\n", query, bits, bytes, strings);
        free(queryBinary);
    }

    free(query);
    nodeFree(placesTree);
    listFree(placesList);
}

