#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "pilot.h"
#include "move.h"
#include "copilot.h"

#ifdef INTOX
char * intox_address = "127.0.0.1";
int intox_port = 12301;
#endif

int main(int argc, char *argv[])
{
    if (argc > 1) {
#ifdef INTOX
        intox_port = atoi(argv[1]);
#endif
    }

    pilot_init();

    move_t move={MOVE_FORWARD, 100};

    while(access("go.txt", F_OK) != 0) {
        usleep(50000);
    }
    printf("Signal reçu, c'est parti !\n");

    pilot_start_move(move);

    while(!pilot_stop_at_target()) {
        usleep(50000);
    }

    while(1) { usleep(100000); }
    return 0;
}