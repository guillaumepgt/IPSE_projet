#ifndef _ROBOT_H_
#define _ROBOT_H_

typedef enum{
    DIR_GAUCHE,
    DIR_DROITE
} direction_t;

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

void robot_init(void);
void robot_start_forward(void);
void robot_stop(void);
void robot_turn(direction_t dir);

encoder_t robot_get_encoder(void);
void robot_reset_encoder(void);

proximity_t robot_get_proximity(void);

#endif