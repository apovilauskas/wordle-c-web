#include <stdio.h>
#include <string.h>


// Purpose: It compares the guess 
// to the secret_word and determines the status of each letter (Correct, Present, Absent).
// 0 for absent letter, 1 for yellow (present), 2 for green (correct)
int* evaluateGuess(const char* secret_word, const char* guess){
    int* array = malloc(5 * sizeof(int));
    for(int i=0; i<5; i++){
        if(secret_word[i] == guess[i]){
            array[i] == 2;
        }
    }

    for (int i = 0; i < 5; i++){
        if (array[i] == 0) 
        {
            for (int j = 0; j < 5; j++)
            {
                if (i != j && guess[i] == secret_word[j] && array[j] != 2)
                {
                    array[i] = 1;
                    break;
                }
            }
        }
    }

    return array;

}

// Purpose: Takes the guess and the raw result array (e.g., [2, 1, 0, 2, 1]) 
// and formats it into a JSON string (like {"0": 2, "1": 1, "2": 0, "3": 2, "4": 1}). 
// JSON is the standard way to send data to the frontend.
char* createJsonResult(int results[]){
    char* json = malloc(64);

    sprintf(json,
        "{\"0\": %d, \"1\": %d, \"2\": %d, \"3\": %d, \"4\": %d}",
        results[0], results[1], results[2], results[3], results[4]
    );

    return json; 

}


