/**
 * @file robot.c
 * @brief Implémentation des fonctions du robot
 * Ce module présente les fonctions pour commander le robt
 */
#include <unistd.h>
#include "robot.h"
#include "mrpiz.h"

static int speed_pct;
static int moving; //boolean


int robot_init(){
    if (mrpiz_init() == -1) {
        mrpiz_error_print("Problème d'initialisation avec le simulateur");
        return 1;
    }
    speed_pct=100;
    moving=0;
    return 0;
}

void robot_set_speed_pct(int pct){
  int new_pct=pct;
  if(pct<10) new_pct=10;
  else if(pct>100) new_pct=100;
  speed_pct=new_pct;
  if(moving) robot_start_forward();
}

int robot_get_speed_pct(){
  return speed_pct;
}

void robot_start_forward(){
      mrpiz_motor_set(MRPIZ_MOTOR_BOTH,speed_pct);
      mrpiz_led_rgb_set (MRPIZ_LED_GREEN);
      moving = 1;
}

void robot_stop(){
  mrpiz_motor_set(MRPIZ_MOTOR_BOTH,0);
  mrpiz_led_rgb_set (MRPIZ_LED_OFF);
  moving = 0;

}

void robot_turn(direction_t dir){
  mrpiz_led_rgb_set (MRPIZ_LED_BLUE);

    if (dir==DIR_DROITE){
        mrpiz_motor_set(MRPIZ_MOTOR_RIGHT,-40);
        mrpiz_motor_set(MRPIZ_MOTOR_LEFT,40);
        usleep(100000);
  }
    if (dir==DIR_GAUCHE){
        mrpiz_motor_set(MRPIZ_MOTOR_LEFT,-40);
        mrpiz_motor_set(MRPIZ_MOTOR_RIGHT,40);
        usleep(100000);
  }
  mrpiz_motor_set(MRPIZ_MOTOR_BOTH,0);
  if(moving) robot_start_forward();
}

bool robot_obstacle_detected(){
  int distance;
  distance = mrpiz_proxy_sensor_get(MRPIZ_PROXY_SENSOR_FRONT_CENTER);
  //printf("distance :  %d        \r",distance);
  return (distance<100);
}

    