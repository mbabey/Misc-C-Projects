#include <stdio.h>
#include <stdlib.h>

/** Required number of command line arguments. */
#define NUM_ARGS 3

/** Size of input buffer. */
#define INBUFSIZE 100

/**
 * Struct for laptop info contained in the input file.
 * @name: the name of the laptop
 * @price: the price of the laptop
 */
struct laptop {
    char name[50];
    float price;
};

/**
 * printArray. Formats and prints the values at the pre-allocated memory
 * positions.
 *
 * @param size - the size of the array
 * @param laptopArray - the pre-allocated memory
 */
void printArray(struct laptop *laptopArray, int size) {
    printf("%-15s%s", "Name", "Price\n");
    for (int i = 0; i < size; i++) {
        printf("%-15s%.2f\n", (laptopArray + i)->name,
               (laptopArray + i)->price);
    }
}

/**
 * fillRemainder. Fills the original array with the remaining values in a subarray.
 *
 * @param array - the original array; to be sorted
 * @param comboIndex - the point of insertion into the original array
 * @param tempSide - the temp subarray
 * @param sideSize - the size of tempSide
 * @param sideIndex - the index at which the remaining values in the subarray
 *                      begin
 */
void fillRemainder(struct laptop *array, int comboIndex, struct laptop *tempSide,
                       int sideSize, int sideIndex) {
    while (sideIndex < sideSize) {
        *(array + comboIndex) = *(tempSide + sideIndex);
        sideIndex++;
        comboIndex++;
    }
}

/**
 * mergeUtil. Utility function for merge. Compares values in the left- and
 * right-side temp arrays, then assigns the values to the original array in
 * ascending order. Any remaining values in either of the are appended temp
 * arrays are appended.
 *
 * @param array - the original array; to be sorted
 * @param comboIndex - the point of insertion into the original array
 * @param tempLeft - the temp left-side array
 * @param leftSize - the size of tempLeft
 * @param tempRight - the temp right-side array
 * @param rightSize - the size of tempRight
 */
void mergeUtil(struct laptop *array, int comboIndex, struct laptop *tempLeft,
               int leftSize, struct laptop *tempRight, int rightSize) {
    int leftIndex = 0, rightIndex = 0;
    while (leftIndex < leftSize && rightIndex < rightSize) {
        if ((tempLeft + leftIndex)->price < (tempRight + rightIndex)->price) {
            *(array + comboIndex) = *(tempLeft + leftIndex);
            leftIndex++;
        } else {
            *(array + comboIndex) = *(tempRight + rightIndex);
            rightIndex++;
        }
        comboIndex++;
    }
    if (leftIndex < leftSize) {
        fillRemainder(array, comboIndex, tempLeft, leftSize, leftIndex);
    }
    if (rightIndex < rightSize) {
        fillRemainder(array, comboIndex, tempRight, rightSize, rightIndex);
    }
}

/**
 * merge. Creates two temporary arrays for the right and left halves of an input
 * array. Then, compares the values in those arrays and reorders them in ascending
 * order.
 *
 * @param array - the original array; to be sorted
 * @param startIndex - the start index of the array segment to be sorted
 * @param midIndex - the middle index of the array segment to be sorted
 * @param endIndex - the end index of the array segment to be sorted
 */
void merge(struct laptop *array, int startIndex, int midIndex, int endIndex) {
    int leftSize = midIndex - startIndex + 1;
    int rightSize = endIndex - midIndex;
    struct laptop tempLeft[leftSize];
    struct laptop tempRight[rightSize];
    for (int i = 0; i < leftSize; i++) {
        *(tempLeft + i) = *(array + startIndex + i);
    }
    for (int i = 0; i < rightSize; i++) {
        *(tempRight + i) = *(array + (midIndex + 1) + i);
    }
    mergeUtil(array, startIndex, tempLeft, leftSize, tempRight, rightSize);
}

/**
 * mergeSort. Recursively splits an array at a halfway point until the array has
 * been broken down into its elements. Then, merges the elements back together in
 * ascending order.
 *
 * @param array - the orignal array; to be sorted.
 * @param startIndex - the start index of the array segment to be sorted
 * @param endIndex - the end index of the array segment to be sorted
 */
void mergeSort(struct laptop *array, int startIndex, int endIndex) {
    if (startIndex < endIndex) {
        const int midIndex = (startIndex + endIndex) / 2;
        mergeSort(array, startIndex, midIndex);
        mergeSort(array, midIndex + 1, endIndex);
        merge(array, startIndex, midIndex, endIndex);
    }
}

/**
 * resizeArray. Reallocates memory for the laptopArray to based on the newSize
 * specified.
 *
 * @param laptopArray - the array to resize
 * @param newSize - the new size for the memory
 */
void resizeArray(struct laptop ***laptopArray, int newSize) {
    **laptopArray = (struct laptop*) realloc(**laptopArray, newSize * sizeof(struct laptop));
    if (**laptopArray == NULL) {
        printf("Realloc failed.\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * scanFileAndAddToArray. Scans the input file. Stores names and prices of
 * laptops into memory. Resizes memory as needed to fit new data.
 *
 * @param itemNames - array to store laptop names
 * @param itemPrices - array to store laptop prices
 * @param laptopDataFile - pointer to file containing laptop info
 * @return the number of items in the file
 */
int scanFile(struct laptop ** laptopArray, FILE *laptopDataFile) {
    char buffer[INBUFSIZE] = "";
    int fileItems = 0;
    struct laptop thisLaptop;
    while (fgets(buffer, INBUFSIZE, laptopDataFile) != NULL) {
        resizeArray(&laptopArray, fileItems + 1);
        sscanf(buffer, "%s %f\n", thisLaptop.name, &thisLaptop.price);
        *(*laptopArray + fileItems) = thisLaptop;
        fileItems++;
    }
    return fileItems;
}

/**
 * createPage. Scans the file to determine the number of items. Allocates memory
 * based on the number of items and stores items at that memory. Sorts the items
 * in ascending order based on price, then prints the first itemsPerPage items.
 *
 * @param itemsPerPage - number of items to display per page
 * @param laptopDataFile - the input data file
 */
void createPage(int itemsPerPage, FILE *laptopDataFile) {
    struct laptop *laptopArray = (struct laptop*) malloc(sizeof (struct laptop));
    int fileItems = scanFile(&laptopArray, laptopDataFile);
    mergeSort(laptopArray, 0, fileItems - 1);
    if (fileItems < itemsPerPage) {
        printArray(laptopArray, fileItems);
    } else {
        printArray(laptopArray, itemsPerPage);
    }
    free(laptopArray);
}

/**
 * checkFile. Checks the input file pointer to ensure it is not null; otherwise,
 * exits the program.
 */
void checkFile(FILE *ifptr) {
    if (ifptr == NULL) {
        printf("Could not open file.\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * checkArgs. Check the number of command line arguments. Exit if the number is
 * too little or too great. Check to ensure the third command line argument
 * is greater than 0. Otherwise, exit. The second check also checks that the
 * third argument is an integer.
 *
 * @param argc - the number of command line arguments
 * @param argv - vector containing command line arguments
 */
void checkArgs(int argc, char **argv) {
    if (argc != NUM_ARGS) {
        printf("Invalid number of arguments. Please enter two arguments.\n "
               "The first argument should be an input file and the second argument "
               "should be the number of products desired on each page.\n");
        exit(EXIT_FAILURE);
    }
    if (atoi(argv[2]) <= 0) {
        printf("Invalid number of items per page. Quitting program.\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * openFile. Opens the file. Calls functions to check validity of arguments and
 * file pointer. Then, calls a function to create and print the page of laptops.
 *
 * @param argc - number of command line arguments
 * @param argv - vector of command line arguments
 */
void openFile(int argc, char **argv) {
    checkArgs(argc, argv);
    FILE *ifptr = fopen(argv[1], "r");
    checkFile(ifptr);
    createPage(atoi(argv[2]), ifptr);
    fclose(ifptr);
}

/**
 * Drives the program.
 *
 * @param argc - number of command line arguments
 * @param argv - vector of command line arguments
 * @return 0: run success
 */
int main(int argc, char *argv[]) {
    openFile(argc, argv);
    return 0;
}
