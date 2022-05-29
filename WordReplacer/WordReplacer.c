#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUL_CHAR 0
#define BUF_LEN 512

/**
 * Finds occurences of the user-entered old word in the input line and replaces
 * them with the user-entered new word.
 * Works by incrementing pointers along oldLine and newLine. When the pointer in
 * oldLine matches the pointed-to location by strstr, adds the newWord to
 * newLine, and increments the newLine pointer by the length of the newWord and
 * the oldLine pointer by the length of the old word. If the pointer in oldLine
 * does not match the pointed-to location by strstr, simply copies the character
 * at the pointers' index from oldLine into newLine and increments both pointers
 * by one.
 *
 * @param oldLine - the line from the input file to scan for matching words.
 * @param oldWord - the user-entered word to be replaced
 * @param oldWordLen - the length of the user-entered word to be replaced
 * @param newWord - the user-entered word to replace
 * @param newWordLen - the length of the user-entered word to replace
 * @param ofptr - pointer to the output file
 * @return the number of words replaced on this line
 */
int replaceWords(char *oldLine, char *oldWord, size_t oldWordLen, char *newWord, size_t newWordLen, FILE *ofptr) {
    char newLine[BUF_LEN] = "";
    int oldLineIndex = 0;
    int newLineIndex = 0;
    int wordsReplaced = 0;

    while (*(oldLine + oldLineIndex) != NUL_CHAR) {
        if (strstr(oldLine + oldLineIndex, oldWord) == oldLine + oldLineIndex) {
            strncat(newLine + newLineIndex, newWord, newWordLen);
            oldLineIndex += (int) oldWordLen;
            newLineIndex += (int) newWordLen;
            wordsReplaced++;
        } else {
            strncat(newLine + newLineIndex, oldLine + oldLineIndex, 1);
            newLineIndex++;
            oldLineIndex++;
        }
    }
    fprintf(ofptr, "%s", newLine);
    return wordsReplaced;
}

/**
 * Takes a pointer to an input file and scans it line by line. Sends each
 * line to replaceWords.
 *
 * @param ifptr - pointer to the input file
 * @param ofptr - pointer to the output file
 * @param oldWord - the user-entered word to be replaced
 * @param newWord - the user-entered word to replace
 *
 * @return the total number of words replaced
 */
int scanFile(FILE *ifptr, FILE *ofptr, char *oldWord, char *newWord) {
    char oldLine[BUF_LEN] = "";
    char reset[BUF_LEN] = "";
    size_t oldWordLen = strlen(oldWord);
    size_t newWordLen = strlen(newWord);
    int wordsReplaced = 0;

    while (fgets(oldLine, BUF_LEN, ifptr) != NULL) {
        wordsReplaced += replaceWords(oldLine, oldWord, oldWordLen, newWord, newWordLen, ofptr);
        strcpy(oldLine, reset);
    }
    return wordsReplaced;
}

/**
 * Closes the input and output files.
 * Deletes the input file and renames the output file to the name of
 * the input file.
 *
 * @param ifptr - pointer to the input file
 * @param ofptr - pointer to the output file
 * @param iFileName - name of the input file
 * @param tempFile - name of the temporary file
 */
void closeFiles(FILE *ifptr, FILE *ofptr, char *iFileName, char *tempFile) {
    fclose(ifptr);
    fclose(ofptr);
    remove(iFileName);
    rename(tempFile, iFileName);
}


/**
 * Prints the results of the program on the console.
 *
 * @param wordsReplaced - the number of words replaced
 * @param newWord - the user-entered word to be replaced
 * @param oldWord - the user-entered word to replace
 */
void resultsToConsole(int wordsReplaced, char *newWord, char *oldWord) {
    if (wordsReplaced > 0) {
        printf("Successfully replaced %d instances of \'%s\' with \'%s\'.\n", wordsReplaced, oldWord, newWord);
    } else {
        printf("Could not find any occurrences of \'%s\'. Nothing altered.\n", newWord);
    }
}

/**
 * Checks the files and reports to the user if they cannot be opened. Exits
 * the program if that is the case.
 *
 * @param ifptr - pointer to the input file
 * @param ofptr - pointer to the output file
 */
void checkFiles(FILE *ifptr, FILE *ofptr) {
    if (ifptr == NULL || ofptr == NULL) {
        printf("Error opening file.");
        fclose(ifptr);
        fclose(ofptr);
        exit(EXIT_FAILURE);
    }
}

/**
 * Checks the words to see if they match. Terminates the program if that is
 * the case.
 *
 * @param oldWord - the word to be replaced
 * @param newWord - the word to replace oldWord
 */
void checkWords(char *oldWord, char *newWord) {
    if (strcmp(oldWord, newWord) == 0) {
        printf("The words are the same. Nothing altered.\n");
        exit(EXIT_SUCCESS);
    }
}

/**
 * Checks the number of arguments passed to the program. Ensures the number
 * of arguments is 2. Otherwise, terminates the program.
 *
 * @param argc - the number of arguments
 */
void checkArgs(int argc) {
    if (argc > 2) {
        printf("Too many arguments.\n");
        exit(EXIT_FAILURE);
    }
    if (argc < 2) {
        printf("Too few arguments. Please enter an input file name as "
               "an argument.\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Opens files and calls numerous functions to edit their contents
 * and close them.
 *
 * @param iFileName - the name of the input file
 * @param oldWord - the word to be replaced
 * @param newWord - the word to replace oldWord
 */
void openFiles(int argc, char *argv[]) {
    checkArgs(argc);
    char *iFileName = argv[1];
    char *tempFile = "temp.txt";
    FILE *ifptr;
    FILE *ofptr;

    char oldWord[50];
    printf("Enter word to replace:");
    gets(oldWord);
    oldWord[strlen(oldWord)] = NUL_CHAR;
    char newWord[50];
    printf("Replace \'%s\' with:", oldWord);
    gets(newWord);
    newWord[strlen(newWord)] = NUL_CHAR;
    checkWords(oldWord, newWord);

    ifptr = fopen(iFileName, "r");
    ofptr = fopen(tempFile, "w");
    checkFiles(ifptr, ofptr);
    int wordsReplaced = scanFile(ifptr, ofptr, oldWord, newWord);
    resultsToConsole(wordsReplaced, newWord, oldWord);
    closeFiles(ifptr, ofptr, iFileName, tempFile);
}

/**
 * Drives the program.
 *
 * @param argv - number of command line args, an int
 * @param argc - array of command line args, an array of char arrays
 * @return exit code 0 for run success
 */
int main(int argc, char *argv[]) {
    openFiles(argc, argv);

    return 0;
}
