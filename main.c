/**
 * @file main.c
 * @brief Programme principal du robot
 *
 * Ce programme lit en boucle une commande clavier et traite la commande.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "autopilot.h"
#include "input_detector.h"

#ifdef INTOX
char * intox_address = "127.0.0.1";
int intox_port = 12301;
#endif

int main(int argc, char *argv[])
{
    input_detector_init();
    input_detector_run();

/*
    if (argc > 1) {
#ifdef INTOX
        intox_port = atoi(argv[1]);
#endif
    }

    autopilot_init();

    while(access("go.txt", F_OK) != 0) {
        usleep(50000);
    }

    autopilot_run();
    */
}

