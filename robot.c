#include "robot.h"
#include "mrpiz.h"
#include <unistd.h>

// --- AJOUT POUR LA LIAISON AVEC MAIN.C ---
#ifdef INTOX
extern char * intox_address;
extern int intox_port;
#endif
// -----------------------------------------

// PRIVATE VARIABLES ----------------------------------------------

static int speed_pct = 100;

// PUBLIC FUNCTIONS DEFINITIONS -----------------------------------

extern void robot_init(void)
{
    if (mrpiz_init() == -1) {
    }
}

extern void robot_start_forward(void)
{
    mrpiz_motor_set(MRPIZ_MOTOR_BOTH, speed_pct);
}

extern void robot_stop(void)
{
    mrpiz_motor_set(MRPIZ_MOTOR_BOTH, 0);
}

extern void robot_turn(direction_t dir)
{
    switch (dir)
    {
        case DIR_DROITE:
            mrpiz_motor_set(MRPIZ_MOTOR_LEFT,speed_pct);
            mrpiz_motor_set(MRPIZ_MOTOR_RIGHT,-speed_pct);
            break;

        case DIR_GAUCHE:
            mrpiz_motor_set(MRPIZ_MOTOR_RIGHT,speed_pct);
            mrpiz_motor_set(MRPIZ_MOTOR_LEFT,-speed_pct);
            break;

        default:
            break;
    }
}

extern encoder_t robot_get_encoder(){
    encoder_t ret;
    ret.left=mrpiz_motor_encoder_get(MRPIZ_MOTOR_LEFT);
    ret.right=mrpiz_motor_encoder_get(MRPIZ_MOTOR_RIGHT);
    return ret;
}

extern void robot_reset_encoder(){
    mrpiz_motor_encoder_reset(MRPIZ_MOTOR_BOTH);
}