#ifndef GAME_STATE_H
#define GAME_STATE_H

typedef struct {
    char session_id[37];
    char secret_word[6];
    int attempts;
} GameState;

void init_game_states();
char* create_new_game();
char* get_secret_word(const char* session_id);
GameState* get_game_state(const char* session_id);//for secret word in case of loss

#endif