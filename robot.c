#include "robot.h"
#include "mrpiz.h"
#include <unistd.h>
#include <stdio.h>

#ifdef INTOX
extern char * intox_address;
extern int intox_port;
#endif

static int speed_pct = 100;

void robot_init(void)
{

    if (mrpiz_init() == -1) {
        mrpiz_error_print("Problème d'initialisation avec le simulateur");
    }
}

void robot_start_forward(void)
{
    mrpiz_motor_set(MRPIZ_MOTOR_BOTH, speed_pct);
}

void robot_stop(void)
{
    mrpiz_motor_set(MRPIZ_MOTOR_BOTH, 0);
}

void robot_turn(direction_t dir)
{
    switch (dir)
    {
    case DIR_DROITE:
        mrpiz_motor_set(MRPIZ_MOTOR_LEFT, speed_pct);
        mrpiz_motor_set(MRPIZ_MOTOR_RIGHT, -speed_pct);
        break;

    case DIR_GAUCHE:
        mrpiz_motor_set(MRPIZ_MOTOR_LEFT, -speed_pct);
        mrpiz_motor_set(MRPIZ_MOTOR_RIGHT, speed_pct);
        break;
    }
}

encoder_t robot_get_encoder(void)
{
    encoder_t ret;
    ret.left = mrpiz_motor_encoder_get(MRPIZ_MOTOR_LEFT);
    ret.right = mrpiz_motor_encoder_get(MRPIZ_MOTOR_RIGHT);
    return ret;
}

void robot_reset_encoder(void)
{
    mrpiz_motor_encoder_reset(MRPIZ_MOTOR_BOTH);
}

proximity_t robot_get_proximity(void)
{
    proximity_t p;

    p.left = mrpiz_proxy_sensor_get(1);
    p.front = mrpiz_proxy_sensor_get(3);
    p.right = mrpiz_proxy_sensor_get(5);

    p.center_left = mrpiz_proxy_sensor_get(2);
    if (p.center_left<0) p.center_left=p.left;
    p.center_right = mrpiz_proxy_sensor_get(4);
    if (p.center_right<0) p.center_right=p.right;


    return p;
}