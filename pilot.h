#ifndef PILOT_H
#define PILOT_H

#include "move.h"
#include <stdbool.h>
#include "robot_copy.h"



extern int encoder_target;

extern void pilot_init();
extern void pilot_start_move(move_t move);
extern bool pilot_stop_at_target();




#endif