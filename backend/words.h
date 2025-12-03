#ifndef WORDS_H
#define WORDS_H

void loadDictionary(const char *filepath);
// use i/o to store the words from words.txt into a dynamic array

const char* selectSecretWord();
//use rand, srand along with total word count to pick a random word

int isValidGuess(const char* guess);
//bool function to check if guessed word is in the list

#endif
