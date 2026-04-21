#include "autopilot.h"
#include "robot.h"
#include "pilot.h"
#include "move.h"
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#define WALL_THRESHOLD 120
#define SIDE_OPEN_THRESHOLD 100

static bool wall_in_front(void)
{
    proximity_t p = robot_get_proximity();
    return p.front < WALL_THRESHOLD;
}

static bool isLeftOpen(void)
{
    proximity_t p = robot_get_proximity();
    return p.left >= SIDE_OPEN_THRESHOLD;
}

static bool isRightOpen(void)
{
    proximity_t p = robot_get_proximity();
    return p.right >= SIDE_OPEN_THRESHOLD;
}

static void do_forward_step(int cm)
{
    move_t m;
    m.type = MOVE_FORWARD;
    m.magnitude = cm;

    pilot_start_move(m);
    while (!pilot_stop_at_target()) {
        usleep(1000);
    }
}

static void do_turn_left_45(void)
{
    move_t m;
    m.type = MOVE_TURN;
    m.magnitude = -45;

    pilot_start_move(m);
    while (!pilot_stop_at_target()) {
        usleep(1000);
    }
}

static void do_turn_right_45(void)
{
    move_t m;
    m.type = MOVE_TURN;
    m.magnitude = 45;

    pilot_start_move(m);
    while (!pilot_stop_at_target()) {
        usleep(1000);
    }
}

static void do_half_turn(void)
{
    move_t m;
    m.type = MOVE_TURN;
    m.magnitude=180;
    pilot_start_move(m);
    while (!pilot_stop_at_target()) {
        usleep(1000);
    }
}

void autopilot_init(void)
{
    pilot_init();
}

void autopilot_run(void)
{
    while (1)
    {
        proximity_t p = robot_get_proximity();
        printf("L=%d F=%d R=%d\n", p.left, p.front, p.right);

        if (!wall_in_front()) {
            do_forward_step(5);
        }
        else if (isLeftOpen()) {
            do_turn_left_45();
        }
        else if (isRightOpen()) {
            do_turn_right_45();
        }
        else {
            do_half_turn();
        }

        usleep(50000);
    }
}