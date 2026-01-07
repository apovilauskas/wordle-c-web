#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define WORD_LEN 6 

char **dictionary = NULL;
int wordCount = 0;

// Comparison function for qsort and bsearch
int cmp(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

// Load words from file into dynamic array
void loadDictionary(const char *filepath) {
    FILE *file = fopen("words.txt", "r");

    char buffer[WORD_LEN];
    while (fgets(buffer, WORD_LEN, file)) {
        buffer[strcspn(buffer, "\n")] = '\0'; // remove newline

        dictionary = realloc(dictionary, (wordCount + 1) * sizeof(char *));
        dictionary[wordCount] = malloc(WORD_LEN);
        strcpy(dictionary[wordCount], buffer);
        wordCount++;
    }
    fclose(file);

    // Sort dictionary for binary search
    qsort(dictionary, wordCount, sizeof(char *), cmp);
}

// Pick a random word from dictionary
const char* selectSecretWord() {
    if (wordCount == 0) return NULL;

    srand(time(NULL));
    int index = rand() % wordCount;
    return dictionary[index];
}

// Check if guess is valid using binary search
int isValidGuess(const char* guess) {
    return bsearch(&guess, dictionary, wordCount, sizeof(char *), cmp) != NULL;
}

// Free dictionary memory
void freeDictionary() {
    for (int i = 0; i < wordCount; i++)
        free(dictionary[i]);
    free(dictionary);
}
