#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMARGS 3
#define BUFSIZE 1024
#define NULLCHAR '\0'

typedef struct node {
    char *word;
    size_t length;
    struct node *next;
} Word;

void checkArgs(int argc, char **argv) {
    if (argc != NUMARGS || atoi(*(argv + 2)) == 0) {
        printf("Invalid command line arguments\n");
        exit(EXIT_FAILURE);
    }
}

FILE *openFile(char *fileName) {
    FILE *ifptr = fopen(fileName, "r");
    if (ifptr == NULL) {
        printf("Could not open file \'%s\'\n", fileName);
        exit(EXIT_FAILURE);
    }
    return ifptr;
}

void checkMem(Word *newWord) {
    if (newWord == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
}

void copyWord(char *buffer, Word **newWord) {
    (*newWord)->length = strlen(buffer);
    (*newWord)->word = (char *) malloc(((*newWord)->length + 1) * sizeof(char));
    strcpy((*newWord)->word, buffer);
    *buffer = NULLCHAR;
}

Word *scanFile(FILE *ifptr) {
    Word *firstWord = NULL;
    Word *newWord = NULL;
    Word *prevWord = NULL;
    char buffer[BUFSIZE] = "";
    while (!feof(ifptr)) {
        newWord = (Word *) malloc(sizeof(Word));
        checkMem(newWord);
        newWord->next = NULL;
        fscanf(ifptr, "%s", buffer);
        copyWord(buffer, &newWord);
        if (firstWord == NULL)
            firstWord = newWord;
        if (prevWord != NULL)
            prevWord->next = newWord;
        prevWord = newWord;
    }
    return firstWord;
}

void freeWords(Word *head) {
    Word *current = NULL;
    while (head != NULL) {
        current = head;
        head = head->next;
        free(current->word);
        free(current);
    }
}

int printSpaces(int spacesOnLine, int placesForSpaces) {
    int spacesPerWord = spacesOnLine / placesForSpaces;
    if (spacesOnLine % placesForSpaces)
        spacesPerWord++;
    spacesOnLine -= spacesPerWord;
    for (; spacesPerWord > 0; spacesPerWord--) {
        printf(" ");
    }
    return spacesOnLine;
}

void justifyAndPrintParagraph(Word *firstWord, int lineLength) {
    Word *startOfLine = firstWord, *endOfLine = firstWord;
    int charsOnLine = 0, wordsOnLine = 1, spacesOnLine;
    while (startOfLine != NULL) {
        do {
            charsOnLine += (int) endOfLine->length;
            endOfLine = endOfLine->next;
            wordsOnLine++;
        } while (endOfLine->next != NULL && charsOnLine + endOfLine->next->length + (wordsOnLine - 1) <
                 lineLength);
        spacesOnLine = lineLength - charsOnLine;
        while (startOfLine != endOfLine->next) {
            printf("%s", startOfLine->word);
            startOfLine = startOfLine->next;
            if (wordsOnLine > 1)
                spacesOnLine = printSpaces(spacesOnLine, --wordsOnLine);
        }
        printf("\n");
        charsOnLine = 0;
        wordsOnLine = 0;
    }
}

void run(char **argv) {
    FILE *ifptr = openFile(*(argv + 1));
    Word *firstWord = scanFile(ifptr);
    justifyAndPrintParagraph(firstWord, atoi(*(argv + 2)));
    fclose(ifptr);
    freeWords(firstWord);
}

int main(int argc, char **argv) {
    checkArgs(argc, argv);
    run(argv);

    return 0;
}
