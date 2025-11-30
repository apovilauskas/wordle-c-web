#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Needed for toupper()

#include "words.h"
#include "logic.h"

#define WORD_LENGTH 5
#define WORDS_FILE "words.txt"

int main(int argc, char *argv[]) {

    if (argc < 2) {
        // handle bad parameters
        return 1;
    }

    const char *raw_input = argv[1];

    if (strlen(raw_input) != WORD_LENGTH) {
        // handle wrong input like in real wordle
        return 0;
    }

    // CASE CONVERSION TO UPPER
    char guess[WORD_LENGTH + 1]; 
    
    for (int i = 0; i < WORD_LENGTH; i++) {
        guess[i] = toupper(raw_input[i]);
    }
    guess[WORD_LENGTH] = '\0'; // Null-terminate the string

    // SET UP WORDS
    loadDictionary(WORDS_FILE); 
    
    const char *secret_word = selectSecretWord();

    // DICTIONARY CHECK
    if (!isValidGuess(guess)) {
        //should not let to evaluate the guess
        return 0; 
    }

    // evaluateGuess should return an integer array (e.g., [0, 2, 1, 0, 0])
    int *results = (int*)evaluateGuess(secret_word, guess);

    // Convert the integer array into the final JSON string
    char *json_output = createJsonResult(guess, results);
    
    printf("%s\n", json_output);

    free(results);
    free(json_output);

    return 0;
}