#include "autopilot.h"
#include "robot.h"
#include "pilot.h"
#include "move.h"
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define FRONT_THRESHOLD        120
#define NEAR_SIDE_THRESHOLD    100
#define SIDE_OPEN_THRESHOLD    100
#define TURN_STEP              5
#define FORWARD_STEP           5


static bool isFrontClear(proximity_t proximityValues)
{
    return proximityValues.front >= FRONT_THRESHOLD;
}

static bool isNearLeftClear(proximity_t proximityValues)
{
    return proximityValues.center_left >= NEAR_SIDE_THRESHOLD;
}

static bool isNearRightClear(proximity_t proximityValues)
{
    return proximityValues.center_right >= NEAR_SIDE_THRESHOLD;
}

static bool canGoForward(proximity_t proximityValues)
{

    return isFrontClear(proximityValues)
        && isNearLeftClear(proximityValues)
        && isNearRightClear(proximityValues);
}

static bool isLeftOpen(proximity_t proximityValues)
{
    return proximityValues.left >= SIDE_OPEN_THRESHOLD;
}

static bool isRightOpen(proximity_t proximityValues)
{
    return proximityValues.right >= SIDE_OPEN_THRESHOLD;
}

static void doForwardStep(int cm)
{
    move_t m;
    m.type = MOVE_FORWARD;
    m.magnitude = cm;

    pilot_start_move(m);
    while (!pilot_stop_at_target()) {
        usleep(1000);
    }
}

static void doLeftTurn(int mag)
{
    move_t m;
    m.type = MOVE_TURN;
    m.magnitude = (-1)*mag;

    pilot_start_move(m);
    while (!pilot_stop_at_target()) {
        usleep(1000);
    }
}

static void doRightTurn(int mag)
{
    move_t m;
    m.type = MOVE_TURN;
    m.magnitude = mag;

    pilot_start_move(m);
    while (!pilot_stop_at_target()) {
        usleep(1000);
    }
}


void autopilot_init(void)
{
    pilot_init();
    srand(time(NULL));
}

void autopilot_run(void){
    while (1){
        
        proximity_t proximityValues = robot_get_proximity();
        #if DEBUG
            printf("L=%d F=%d R=%d\n",proximityValues.center_left, proximityValues.front, proximityValues.center_right); 
        #endif

        if (canGoForward(proximityValues)) {
            doForwardStep(FORWARD_STEP);
            #if DEBUG
                printf("Decision: FORWARD\n");
            #endif
        }
        else {
            bool left_open = isLeftOpen(proximityValues);
            bool right_open = isRightOpen(proximityValues);
            #if DEBUG   
                printf("Can go forward: NO\n");     
                printf("Left Open?:%d Right Open?:%d\n",left_open,right_open);
            #endif

            if (left_open && right_open) {
                int choice = rand() % 2; //nombre au hasard entre 0 ou 1

                #if DEBUG
                    printf("Decision: BOTH OPEN -> RANDOM\n");
                    printf("Random choice: %s\n", choice == 0 ? "LEFT" : "RIGHT");
                #endif

                if (choice == 0) {
                    doLeftTurn(TURN_STEP);
                } else {
                    doRightTurn(TURN_STEP);
                }
            }
            else if (left_open) {
                doLeftTurn(TURN_STEP);
                #if DEBUG
                    printf("Decision: TURN LEFT\n");
                #endif
            }
            else if (right_open) {
                doRightTurn(TURN_STEP);
                #if DEBUG
                    printf("Decision: TURN RIGHT\n");
                #endif
            }
            else {
                int choice = rand() % 2;

                #if DEBUG
                    printf("Decision: BLOCKED -> RANDOM TURN\n");
                    printf("Random choice: %s\n", choice == 0 ? "LEFT" : "RIGHT");
                #endif

                if (choice == 0) {
                    doLeftTurn(TURN_STEP);
                } else {
                    doRightTurn(TURN_STEP);
                }
            }
        }

        usleep(50000);
    }
}
