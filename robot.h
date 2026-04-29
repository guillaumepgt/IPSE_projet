/**
 * @file robot.h
 * @brief Interface publique du robot.
 *
 * Ce fichier contient les déclarations nécessaires à l'utilisation
 * du robot
 */
#ifndef _ROBOT_H_
#define _ROBOT_H_

#include <stdbool.h>
#include "remote.h"

typedef enum {
    DIR_GAUCHE=0,
    DIR_DROITE
}direction_t;

typedef struct{
    int left;
    int right;
} encoder_t;

typedef struct{
    int left;
    int center_left;
    int front;
    int center_right;
    int right;
} proximity_t;

void robot_init();
void robot_start_forward();
void robot_stop();
void robot_turn(direction_t direction);
void robot_set_speed_pct(int pct);
int robot_get_speed_pct(void);

bool robot_obstacle_detected();

encoder_t robot_get_encoder(void);
void robot_reset_encoder(void);

proximity_t robot_get_proximity(void);


#endif