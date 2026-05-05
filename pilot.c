#include "pilot.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

encoder_t enc;
int encoder_target = 0;

void pilot_init(){
    robot_init();
}

void pilot_start_move(move_t move){
    robot_reset_encoder();

    if (move.type==MOVE_FORWARD){
        robot_start_forward();
        encoder_target=round(34.6*move.magnitude);

    }

    else if(move.type==MOVE_TURN ){
        robot_turn(DIR_DROITE);
        encoder_target= round(2.53*move.magnitude);

    }



}

bool pilot_stop_at_target(){
    bool ret=false;
    enc = robot_get_encoder();
    if (abs(enc.left)>=encoder_target){
        robot_stop();
        ret = true;
    } 
    return ret;
}
