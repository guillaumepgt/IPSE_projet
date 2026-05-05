#ifndef COPILOT_H
#define COPILOT_H

#include "move.h"

int copilot_init(const char* name_file);
void copilot_run(void);

void move_provider_update_nb_move(int* nb);
void move_provider_update_all_moves(move_t* moves);

#endif