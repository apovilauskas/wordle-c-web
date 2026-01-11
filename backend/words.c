#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define WORD_LEN 6 

char **dictionary = NULL;
int wordCount = 0;

// Comparison function for qsort and bsearch
int cmp(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void loadDictionary(const char *filepath) {
    printf("DEBUG: Loading dictionary from %s...\n", filepath);
    
    FILE *file = fopen(filepath, "r"); // Explicitly looking for words.txt
    if (!file) {
        printf("ERROR: Could not open dictionary! Make sure it is in the same folder as the .exe\n");
        return;
    }

    char buffer[128]; // Larger buffer to be safe
    while (fgets(buffer, sizeof(buffer), file)) {
        // Remove newlines and carriage returns
        buffer[strcspn(buffer, "\r\n")] = '\0';

        // Skip empty lines
        if (strlen(buffer) == 0) continue;

        // Force Uppercase
        for(int i=0; buffer[i]; i++) {
            buffer[i] = toupper((unsigned char)buffer[i]);
        }

        dictionary = realloc(dictionary, (wordCount + 1) * sizeof(char *));
        dictionary[wordCount] = malloc(WORD_LEN + 1);
        strncpy(dictionary[wordCount], buffer, WORD_LEN);
        wordCount++;
    }
    fclose(file);

    // Sort dictionary for binary search
    qsort(dictionary, wordCount, sizeof(char *), cmp);

    printf("DEBUG: Dictionary loaded. Total words: %d\n", wordCount);
    if (wordCount > 0) {
        printf("DEBUG: First word is: '[%s]'\n", dictionary[0]);
        printf("DEBUG: Last word is:  '[%s]'\n", dictionary[wordCount-1]);
    }
}

const char* selectSecretWord() {
    if (wordCount == 0) return NULL;
    int index = rand() % wordCount;
    return dictionary[index];
}

int isValidGuess(const char* guess) {
    if (dictionary == NULL || wordCount == 0) {
        printf("ERROR: Dictionary is empty during guess!\n");
        return 0;
    }

    // Create a local uppercase copy of the guess
    char temp[32]; 
    strncpy(temp, guess, 31);
    temp[31] = '\0'; // Safety null-terminator
    
    // Convert to uppercase
    for(int i=0; temp[i]; i++) {
        temp[i] = toupper((unsigned char)temp[i]);
    }

    printf("DEBUG: Searching for '[%s]' in %d words...\n", temp, wordCount);

    // bsearch requires a pointer to the string pointer
    const char *key = temp; 
    void* found = bsearch(&key, dictionary, wordCount, sizeof(char *), cmp);

    if (found) {
        printf("DEBUG: Match FOUND!\n");
        return 1;
    } else {
        printf("DEBUG: Match NOT found.\n");
        return 0;
    }
}