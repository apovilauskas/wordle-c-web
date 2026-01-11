#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// Purpose: It compares the guess 
// to the secret_word and determines the status of each letter (Correct, Present, Absent).
// 0 for absent letter, 1 for yellow (present), 2 for green (correct)
int* evaluateGuess(const char* secret_word, const char* guess) {
    int* array = calloc(5, sizeof(int));
    int secret_used[5] = {0, 0, 0, 0, 0};

    // Pass 1: Greens
    for (int i = 0; i < 5; i++) {
        if (secret_word[i] == guess[i]) {
            array[i] = 2;
            secret_used[i] = 1;
        }
    }

    // Pass 2: Yellows
    for (int i = 0; i < 5; i++) {
        if (array[i] == 0) {
            for (int j = 0; j < 5; j++) {
                if (!secret_used[j] && guess[i] == secret_word[j]) {
                    array[i] = 1;
                    secret_used[j] = 1;
                    break; // Move to next 'i'
                }
            }
        }
    }
    return array;
}

char* createJsonResult(int results[]) {
    // Allocate memory for JSON string
    char* json = malloc(100);
    
    // Convert [2,1,0,2,1] to "21021" string format
    char result_str[6];
    for (int i = 0; i < 5; i++) {
        result_str[i] = '0' + results[i]; // Convert int to char
    }
    result_str[5] = '\0';
    
    // Create JSON: {"result":"21021"}
    snprintf(json, 100, "{\"result\":\"%s\"}", result_str);
    
    return json;
}