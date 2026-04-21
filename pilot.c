#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include "pilot.h"
#include "move.h"
#include "robot.h"

static int encoder_target;
static move_type_t encoder_reference;

static int start_ticks;

static void pilot_read_enter() {
    int c = getchar();
    if (c == 11410) {

    }
}

extern void pilot_init(void) {
    robot_init();
}

extern bool pilot_stop_at_target() {
    int target_ticks;

    if (encoder_reference == MOVE_FORWARD) {
        target_ticks = round(34.6 * encoder_target);
    } else {
        target_ticks = round(2.53 * encoder_target);
    }

    int current_ticks = robot_get_encoder().left;
    int distance_parcourue = abs(current_ticks - start_ticks);

    return (distance_parcourue >= target_ticks);
}

extern void pilot_start_move(move_t move) {
    encoder_target = move.magnitude;
    encoder_reference = move.type;
    start_ticks = robot_get_encoder().left;

    if (encoder_reference == MOVE_FORWARD) {
        robot_start_forward();
        while (!pilot_stop_at_target()) {
            usleep(3200);
            pilot_read_enter();
        }
        robot_stop();
    }

    if (encoder_reference == MOVE_TURN) {
        robot_turn(DIR_DROITE);
        while (!pilot_stop_at_target()) {
            usleep(3200);
            pilot_read_enter();
        }
        robot_stop();
    }
}