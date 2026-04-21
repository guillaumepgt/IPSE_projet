#ifndef _ROBOT_H_
#define _ROBOT_H_

#include <stdbool.h>

// PUBLIC TYPES ---------------------------------------------------

typedef enum{
    DIR_GAUCHE,
    DIR_DROITE
}direction_t;

typedef struct{
    int left;
    int right;
}encoder_t;

// PUBLIC FUNCTIONS DECLARATIONS ----------------------------------

extern void robot_init(void);
extern void robot_start_forward(void);
extern void robot_stop(void);
extern void robot_turn(direction_t dir);
extern void robot_reset_encoder(void);
extern encoder_t robot_get_encoder();

#endif  /* _ROBOT_H_ */