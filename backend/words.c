#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define WORD_LEN 6 

// Main dictionary (for guessing validation)
char **dictionary = NULL;
int wordCount = 0;

// Secret dictionary (for selecting the answer)
char **secretDictionary = NULL;
int secretWordCount = 0;

// Comparison function for qsort and bsearch
int cmp(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void loadDictionary(const char *filepath) {
    printf("DEBUG: Loading dictionary from %s...\n", filepath);
    
    FILE *file = fopen(filepath, "r");
    if (!file) {
        printf("ERROR: Could not open dictionary! Make sure words.txt is in the backend folder\n");
        return;
    }

    char buffer[128]; // Larger buffer to be safe
    while (fgets(buffer, sizeof(buffer), file)) {
        // Remove newlines and carriage returns
        buffer[strcspn(buffer, "\r\n")] = '\0';

        // Skip empty lines
        if (strlen(buffer) == 0) continue;

        // Force Uppercase
        for(int i = 0; buffer[i]; i++) {
            buffer[i] = toupper((unsigned char)buffer[i]);
        }

        dictionary = realloc(dictionary, (wordCount + 1) * sizeof(char *));
        dictionary[wordCount] = malloc(WORD_LEN + 1);
        strncpy(dictionary[wordCount], buffer, WORD_LEN);
        dictionary[wordCount][WORD_LEN] = '\0'; // Null-terminate
        wordCount++;
    }
    fclose(file);

    // Sort dictionary for binary search
    qsort(dictionary, wordCount, sizeof(char *), cmp);

    printf("DEBUG: Dictionary (words.txt) loaded. Total words: %d\n", wordCount);
    if (wordCount > 0) {
        printf("DEBUG: First word is: '[%s]'\n", dictionary[0]);
        printf("DEBUG: Last word is:  '[%s]'\n", dictionary[wordCount-1]);
    }
}

void loadSecretDictionary(const char *filepath) {
    printf("DEBUG: Loading secret dictionary from %s...\n", filepath);
    
    FILE *file = fopen(filepath, "r");
    if (!file) {
        printf("ERROR: Could not open secret dictionary! Make sure secret.txt is in the backend folder\n");
        return;
    }

    char buffer[128]; // Larger buffer to be safe
    while (fgets(buffer, sizeof(buffer), file)) {
        // Remove newlines and carriage returns
        buffer[strcspn(buffer, "\r\n")] = '\0';

        // Skip empty lines
        if (strlen(buffer) == 0) continue;

        // Force Uppercase
        for(int i = 0; buffer[i]; i++) {
            buffer[i] = toupper((unsigned char)buffer[i]);
        }

        secretDictionary = realloc(secretDictionary, (secretWordCount + 1) * sizeof(char *));
        secretDictionary[secretWordCount] = malloc(WORD_LEN + 1);
        strncpy(secretDictionary[secretWordCount], buffer, WORD_LEN);
        secretDictionary[secretWordCount][WORD_LEN] = '\0'; // Null-terminate
        secretWordCount++;
    }
    fclose(file);

    // Sort secret dictionary for consistency
    qsort(secretDictionary, secretWordCount, sizeof(char *), cmp);

    printf("DEBUG: Secret Dictionary (secret.txt) loaded. Total words: %d\n", secretWordCount);
    if (secretWordCount > 0) {
        printf("DEBUG: First secret word is: '[%s]'\n", secretDictionary[0]);
        printf("DEBUG: Last secret word is:  '[%s]'\n", secretDictionary[secretWordCount-1]);
    }
}

const char* selectSecretWord() {
    if (secretWordCount == 0) {
        printf("ERROR: Secret dictionary is empty!\n");
        return NULL;
    }
    int index = rand() % secretWordCount;
    return secretDictionary[index];
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
    for(int i = 0; temp[i]; i++) {
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