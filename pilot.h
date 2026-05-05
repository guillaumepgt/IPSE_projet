#ifndef PILOT_H
#define PILOT_H

#include "move.h"
#include <stdbool.h>

void pilot_init(void);
void pilot_start_move(move_t move);
bool pilot_stop_at_target(void);

void pilot_offset_target(int offset);

#endif