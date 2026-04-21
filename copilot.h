#ifndef MOVE_PROVIDER_H
#define MOVE_PROVIDER_H

#include "move.h"

int move_provider_update_next_move(move_t* move);

void move_provider_update_nb_move(int* nb);

void move_provider_update_all_moves(move_t* moves);

int copilot_init(const char* name_file);


#endif