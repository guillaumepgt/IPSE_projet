#include "robot.h"
#include "mrpiz.h"
#include <unistd.h>

// PRIVATE VARIABLES ----------------------------------------------

static int speed_pct = 100;

// PUBLIC FUNCTIONS DEFINITIONS -----------------------------------

extern void robot_init(void)
{
//Initialisation du robot et vérification du bon démarrage
    if (mrpiz_init() == -1) {
        mrpiz_error_print("Problème d'initialisation avec le simulateur");
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

extern encoder_t robot_get_encoder() {
	encoder_t enc;
    enc.left=mrpiz_motor_encoder_get(MRPIZ_MOTOR_LEFT);
    enc.right=mrpiz_motor_encoder_get(MRPIZ_MOTOR_RIGHT);
    return enc;
}

extern void robot_reset_encoder(void) {
    mrpiz_motor_encoder_reset(MRPIZ_MOTOR_BOTH);
}