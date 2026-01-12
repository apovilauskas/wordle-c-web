#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game_state.h"
#include "words.h"

#define MAX_GAMES 1000

GameState games[MAX_GAMES];
int game_count = 0;

void init_game_states() {
    srand(time(NULL));
    loadDictionary("words.txt"); 
}

char* create_new_game() {
    if (game_count >= MAX_GAMES) {
        game_count = 0;
    }
    
    GameState* game = &games[game_count];
    
    // Generate simple session ID
    snprintf(game->session_id, sizeof(game->session_id), 
             "%ld%04d", time(NULL), rand() % 10000);
    
    const char* word = selectSecretWord();
    strncpy(game->secret_word, word, 5);
    game->secret_word[5] = '\0';
    
    game->attempts = 0;
    
    game_count++;
    
    return game->session_id;
}

char* get_secret_word(const char* session_id) {
    for (int i = 0; i < game_count; i++) {
        if (strcmp(games[i].session_id, session_id) == 0) {
            return games[i].secret_word;
        }
    }
    return NULL;
}

GameState* get_game_state(const char* session_id) {
    for (int i = 0; i < game_count; i++) {
        if (strcmp(games[i].session_id, session_id) == 0) {
            return &games[i];
        }
    }
    return NULL;
}