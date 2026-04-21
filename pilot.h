//
// Created by Prigent on 31/03/2026.
//

#ifndef TD4_PILOT_H
#define TD4_PILOT_H

#include <stdbool.h>

#include "move.h"

extern void pilot_init(void);
extern void pilot_start_move(move_t move);
extern bool pilot_stop_at_target();

#endif //TD4_PILOT_H