/*---------------------------------------------------------------- 
..Project: dict3
  rt.h :  
          = the interface of module radixTree of the project.
------------------------------------------------------------------*/


#ifndef _RT_H_
#define _RT_H_
#include "data.h"
#include "list.h"

/*------------ Constant definitions ------------------------------*/
#define QUERY 0
#define KEY 1
#define INIT_STEM_SIZE 16
#define INCREASE_STEM_SIZE 2
#define BYTE 8          // 1 byte = 8 bits

/*------------ Data definitions ----------------------------------*/
typedef struct rt rt_t;
struct rt {
    int prefixBits;            // Number of common bits
    char *prefix;              // Binary representation of common bits
    rt_t *branchA;             // Pointer to leaf node that begins with 0
    rt_t *branchB;             // Pointer to leaf node that begins with 1
    list_t *dataList;          // Linked list of data for places
};

/*------------ Function definitions -----------------------------*/
/**
* Creates, initialises and returns a new tree node.
*/
rt_t *rtCreate(rt_t *node, char *key, place_t *data);

/**
* Inserts the "key" of common prefix and corresponding place "data"
* into a "node" of the radix tree. 
* Returns a radix tree with each parent node only storing the prefix
* stem that is common to all its children nodes. 
*/
rt_t *rtInsert(rt_t *node, char *key, place_t *data);

/**
* Converts a char string into explicit 8-bit binary representation string. 
* Returns a binary representation string.
*/
char *stringToBinary(char* str, int strType);

/**
* Compares each bit of the two strings to find the common prefix stem.
* Also records the length of the common prefix stem. 
* Returns a string of common bits. 
*/
char *stemFinder(const char *str1, const char *str2, int *stemLen);

/**
* Creates a new binary representation string for the "key", excluding 
* the bits up until the "stemLen" index.
* Returns a new shorter string with deleted common prefix. 
*/
char *stemSplit(char* key, int newSrtIndex);

/**
* Check whether the bit at a certain index "stemLen" is 0 or 1.
* Returns the integer - 0 or 1 for the bit at a particular index. 
*/
int getBit(const char *key, int stemLen);

/**
* Traverse through the tree nodes to search for place with matching 
* "trading name" to the input "query" in sorted array "arr".
* Explores branchA fully before moving on to branchB.
* Returns -1 if no match found or 0 after successful traversing. 
* Also keeps track of: "bits" - # of bits compared
*                      "commonChar" - # of common characters
*                      "link" - # of moving down to leaf nodes
*                      "strings" - # of strings compared
*/
int findAndTraverse(rt_t *node, const char *query, int *bits, int *link, int *commonChar, int *strings, FILE *outFile);

/**
* Writes data information for places stored in a linked list for a 
* tree node into the output file. 
* Explores the left node fully before moving on to the right node.
*/
int rtSearch(rt_t *node, FILE *outFile); 

/**
* Frees the memory allocated for a tree node and its attributes.
*/
void nodeFree(rt_t *node);

#endif

