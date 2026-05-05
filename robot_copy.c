#include "robot_copy.h"
#include "mrpiz.h"
#include <unistd.h>
#include "pilot.h"

#include <stdbool.h>

// PRIVATE VARIABLES ----------------------------------------------

static int speed_pct = 60;

static int cible_spin=360*2.53;

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
    int consigne = speed_pct;

    mrpiz_motor_set(MRPIZ_MOTOR_BOTH, consigne);
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


void robot_set_speed(int pourcentage){
    mrpiz_motor_set(MRPIZ_MOTOR_BOTH, pourcentage);
}


void robot_spin(void){
    int codeur_depart=mrpiz_motor_encoder_get(MRPIZ_MOTOR_LEFT);
    int codeur_actuel=codeur_depart;
    while(abs(codeur_actuel-codeur_depart)<cible_spin){
        codeur_actuel=mrpiz_motor_encoder_get(MRPIZ_MOTOR_LEFT);
        mrpiz_motor_set(MRPIZ_MOTOR_LEFT, speed_pct);
        mrpiz_motor_set(MRPIZ_MOTOR_RIGHT, -speed_pct);
    }
    robot_stop();
    encoder_target += (codeur_actuel-codeur_depart);
}