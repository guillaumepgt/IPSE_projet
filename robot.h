#ifndef _ROBOT_H_
#define _ROBOT_H_

// PUBLIC TYPES ---------------------------------------------------

typedef enum{
    DIR_GAUCHE,
    DIR_DROITE
}direction_t;

typedef struct{
    int left;
    int right;
} encoder_t;

// PUBLIC FUNCTIONS DECLARATIONS ----------------------------------

extern void robot_init(void);
extern void robot_start_forward(void);
extern void robot_stop(void);
extern void robot_turn(direction_t dir);

extern encoder_t robot_get_encoder();
extern void robot_reset_encoder();

#endif  /* _ROBOT_H_ */