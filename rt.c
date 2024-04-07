/*--------------------------------------------------------------- 
..Project: dict3
  rt.c :  
          = the implementation of module radixTree of the project.
-----------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "rt.h"

/**
* Creates, initialises and returns a new tree node.
*/
rt_t *rtCreate(rt_t *node, char *key, place_t *data) {
    node = malloc(sizeof(*node));
    assert(node);
    node->prefixBits = strlen(key);
    node->prefix = key;
    node->branchA = NULL;
    node->branchB = NULL;

    if (data == NULL) {
        node->dataList = NULL;
    }
    else if (data != NULL) {
        node->dataList = listCreate();
        listAppend(node->dataList, data);
    }
    return node;
}

/**
* Inserts the "key" of common prefix and corresponding place "data"
* into a "node" of the radix tree. 
* Returns a radix tree with each parent node only storing the prefix
* stem that is common to all its children nodes. 
*/
rt_t *rtInsert(rt_t *node, char *key, place_t *data) {
    // When the node is empty, initialise a node and insert place data
    if (node == NULL) {
        node = rtCreate(node, key, data);
        return node;
    }

    // When the node is not empty, compare trading name bits and modify
    int stemLen = 0; // Length of the common prefix bit stem
    char *stem = stemFinder(node->prefix, key, &stemLen); // Common prefix
    char *newKey1; // Temporary bit key for already existing parent
    char *newKey2; // Temporary bit key fornew input key 
    int singleBit; // 0 or 1 to inform about the next bit after stem
    rt_t *temp = NULL; // Temporary parent node

    // When the data is a duplicate, store in the dataList of the node
    if (stem == NULL) {
        free(key);
        listAppend(node->dataList, data); 
    }
    // When first few bits match and the rest are different, split the 
    // matching prefix stem and set it as the new parent node. Move 
    // down each of previous parent node key and new input key to the
    // left and right leaf node. 
    else if (stemLen > 0 && strlen(node->prefix) != stemLen) {
        newKey1 = stemSplit(node->prefix, stemLen);
        newKey2 = stemSplit(key, stemLen);
        singleBit = getBit(newKey2, 0);

        // Use temporary node to not lose any further down tree structure
        // When adding new nodes in the middle
        // Use temp as the new parent node with common prefix stem found
        temp = rtInsert(temp, stem, NULL);

        // When the very next bit after prefix stem is 0 for new key & 
        // 1 for previous parent key
        if (singleBit == 1) {
            // Move previous parent key down to left leaf node
            temp->branchA = node;
            // Update the attributes
            strcpy(temp->branchA->prefix, newKey1);
            temp->branchA->prefixBits = strlen(newKey1);
            free(newKey1);
            // create a node & insert data for new key into right leaf node
            temp->branchB = rtInsert(temp->branchB, newKey2, data);
            // Bring back new tree segment to the radix tree
            node = temp;
        }
        // When the very next bit after prefix stem is 1 for new key & 
        // 0 for previous parent key
        else if (singleBit == 0) {
            // Move previous parent key down to right leaf node
            temp->branchB = node;
            // Update the attributes
            strcpy(temp->branchB->prefix, newKey1);
            temp->branchB->prefixBits = strlen(newKey1); 
            free(newKey1);
            // create a node & insert data for new key into left leaf node
            temp->branchA = rtInsert(temp->branchA, newKey2, data);
            // Bring back new tree segment to the radix tree
            node = temp;
        }
        free(key);
    }
    // When the new key matches all bits of the parent node and it's 
    // string length equals to the length of the prefix stem of the 
    // parent, no string trimming or node creation is required. Move 
    // it down to leaf node for further prefix comparisons.
    else if (stemLen > 0 && strlen(node->prefix) == stemLen) {
        newKey2 = stemSplit(key, stemLen);
        singleBit = getBit(newKey2, 0);
        free(stem);
        free(key);

        if (singleBit == 1) {
            node->branchB = rtInsert(node->branchB, newKey2, data);
        }
        else if (singleBit == 0) {
            node->branchA = rtInsert(node->branchA, newKey2, data);
        }
    }
    return node;
}

/**
* Converts a char string into explicit 8-bit binary representation string. 
* Returns a binary representation string.
*/
char *stringToBinary(char *str, int strType) {
    // Ensure the input string is not empty
    if (str == NULL) return 0;
    
    // Each char is one byte (8 bits) and + 1 for null terminator
    int len = strlen(str);
    char *binary = malloc(len * 8 + 8 + 1);
    binary[0] = '\0';
    
    // Interate through loop to create a string consisting of 0 and
    // 1 binary representation for the whole trading name 
    for(int i = 0; i < len; i++) {
        char letter = str[i];
        for(int j = 7; j >= 0; j--){
            if(letter & (1 << j)) {
                strcat(binary,"1");
            } 
            else {
                strcat(binary,"0");
            }
        }
    }

    if (strType == KEY) {
        // Insert binary representation for null terminator
        for(int k = 7; k >= 0; k--){
            strcat(binary,"0");
        }
    }
    return binary;
}

/**
* Compares each bit of the two strings to find the common prefix stem.
* Also records the length of the common prefix stem. 
* Returns a string of common bits. 
*/
char *stemFinder(const char* str1, const char* str2, int *stemLen) {
    int stemSize = INIT_STEM_SIZE;
    // Initiate memory allocation with default size as the legnth of
    // the common prefix stem is unknown at the start
    char *stem = malloc(stemSize);
    int index = 0;

    while (*str1 != '\0' && *str2 != '\0') {
        // Increase stem string size if required
        if (index >= stemSize - 1) {
            stemSize *= INCREASE_STEM_SIZE;
            stem = realloc(stem, stemSize);
        }

        // When mismatch detected, add null terminator at the end of
        // the prefix stem found and return it
		if (*str1 < *str2 || *str1 > *str2) {
            stem[index] = '\0';
            return stem;
        }
        stem[index] = *str1;
        (*stemLen)++;
        index++;
		str1++;
		str2++;
	}
    stem[index] = '\0';

    // When two trading names are identical return NULL to indicate it
    if (*str1 == '\0' && *str2 == '\0') {
        free(stem);
        return NULL;
    }
	return stem;
}

/**
* Creates a new binary representation string for the "key", excluding 
* the bits up until the "stemLen" index.
* Returns a new shorter string with deleted common prefix. 
*/
char *stemSplit(char *key, int stemLen) {
    // Allocate memory for string without common stem + 1 for '\0'
    char *newKey = malloc(strlen(key) - stemLen + 1);
    int newSrtIndex = stemLen;
    int len = strlen(key);

    // Append bits from the input key from stemLen index to exclude 
    // the common prefix stem
    int i = 0;
    while (newSrtIndex < len) {
        newKey[i] = key[newSrtIndex]; 
        i++;
        newSrtIndex++;
    }
    newKey[i] = '\0';
    return newKey;
}

/**
* Check whether the bit at a certain index "stemLen" is 0 or 1.
* Returns the integer - 0 or 1 for the bit at a particular index. 
*/
int getBit(const char *key, int stemLen) {
    int bit;

    // Iterate through the loop to move the pointer to the index
    // of the very next bit after the common prefix stem
    if (stemLen != 0) {
        for (int i=0; i<=stemLen; i++) {
            key++;
        }
    }

    if (*key == '0') {
        bit = 0;
    }
    else if (*key == '1') {
        bit = 1;
    }
    return bit;
}

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
int findAndTraverse(rt_t *node, const char *query, int *bits, int *link, 
                        int *commonChar, int *strings, FILE *outFile) {
    // Compare each bit for the common prefix stem of the node and
    // the input query
    char *prefix;
    if (node != NULL) {
        prefix = node->prefix;
        while (*prefix != '\0' && *query != '\0') {
            (*bits)++;
            if (*prefix != *query) {
                return -1;
            }
            query++;
            prefix++;
        }

        // When trading name is shorter than the query, there must be
        // no match for the query input
        if (*query != '\0' && *prefix == '\0' && node->branchA == NULL) {
            return -1;
        }
        // When query is the same as or shorter than the trading name, 
        // match is found - setting the current node as the parent, 
        // call rtSearch function to print all place data stored in the
        // linked list of every lead node below
        else if (*query == '\0') {
            (*strings)++;
            rtSearch(node, outFile);
            return 0;
        }

        // When the end of the common prefix stem is reached and there are
        // more leaf nodes, check the next bit for query to decide which 
        // direction to choose (0 -> left, 1 -> right)
        int singleBit = getBit(query, 0);

        // Keep track of the common characters detected and the number of 
        // times for moving down to a leaf node
        *commonChar = *commonChar + node->prefixBits;
        if (node->prefixBits <= 8) (*link) += 1;
        else if (node->prefixBits > 8) (*link) += (node->prefixBits + (8-1))/8;

        // Move down to a leaf node for further bit comparisons
        if (singleBit == 0) {
            findAndTraverse(node->branchA, query, &(*bits), &(*link), 
                                &(*commonChar), &(*strings), outFile);
        }
        else if (singleBit == 1) {
            findAndTraverse(node->branchB, query, &(*bits), &(*link), 
                                &(*commonChar), &(*strings), outFile);
        }
    }
    return 0;
}

/**
* Writes data information for places stored in a linked list for a 
* tree node into the output file. 
* Explores the left node fully before moving on to the right node.
*/
int rtSearch(rt_t *node, FILE *outFile) {
    if (node != NULL) {
        // Ensure the data list is not empty
        if (node->dataList != NULL) {
            node_t *curr = node->dataList->head;
            // Write place data into output file 
            while (curr != NULL) {
                dataWrite(curr->data, outFile);
                curr = curr->next;
            }
        }

        // Resursive call of function with branchA first and then
        // call for branchB to explore the left node fully before 
        // moving to the right node
        rtSearch(node->branchA, outFile);
        rtSearch(node->branchB, outFile);
    }
    else if (node == NULL) {
        return -1;
    }

    return 0;
}

/**
* Frees the memory allocated for a tree node and its attributes.
*/
void nodeFree(rt_t *node) {
    free(node->prefix);
    if (node->dataList != NULL) {
        listPartialFree(node->dataList);
    }
    if (node->branchA != NULL) {
        nodeFree(node->branchA);
    }
    if (node->branchB != NULL) {
        nodeFree(node->branchB);
    }
    free(node);
}

