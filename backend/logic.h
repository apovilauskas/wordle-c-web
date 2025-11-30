#infdef LOGIC_H
#define LOGIC_H

int* evaluateGuess(const char* secret_word, const char* guess);
// Purpose: This is the heart of the game. It compares the guess 
// to the secret_word and determines the status of each letter (Correct, Present, Absent).
// 0 for absent letter, 1 for yellow (present), 2 for green (correct)
//in .c it should have int* results = malloc(5 * sizeof(int)); ... return results

char* createJsonResult(const char* guess, int results[]);
// Purpose: Takes the guess and the raw result array (e.g., [2, 1, 0, 2, 1]) 
// and formats it into a JSON string (like {"0": 2, "1": 1, "2": 0, "3": 2, "4": 1}). 
// JSON is the standard way to send data to the frontend.


#endif
