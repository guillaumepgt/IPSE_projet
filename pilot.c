#include "pilot.h"
#include "robot_copy.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static int encoder_target = 0;

void pilot_init(void){
    robot_init();
}

void pilot_start_move(move_t move){
    robot_reset_encoder();

    if (move.type == MOVE_FORWARD) {
        robot_start_forward();
        encoder_target = round(34.6 * move.magnitude);
    }
    else if (move.type == MOVE_TURN) {
        // CORRECTION DU BUG : On remet la distinction gauche/droite !
        if (move.magnitude < 0) robot_turn(DIR_GAUCHE);
        else robot_turn(DIR_DROITE);

        encoder_target = round(2.53 * abs(move.magnitude));
    }
}

bool pilot_stop_at_target(void){
    encoder_t enc = robot_get_encoder();
    if (abs(enc.left) >= encoder_target){
        robot_stop();
        return true;
    }
    return false;
}

void pilot_offset_target(int offset) {
    encoder_target += offset;
}