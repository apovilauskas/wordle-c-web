#ifndef WORDS_H
#define WORDS_H

void loadDictionary(const char *filepath);
// Load words.txt into a dynamic array for guess validation

void loadSecretDictionary(const char *filepath);
// Load secret.txt into a dynamic array for selecting secret words

const char* selectSecretWord();
// Use rand() with secret word count to pick a random word from secret.txt

int isValidGuess(const char* guess);
// Check if guessed word is in the dictionary (words.txt)

#endif