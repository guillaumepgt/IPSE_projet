#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "autopilot.h"
#include "copilot.h"
#include "input_detector.h"
#include "pilot.h"
#include "surprises.h"

#ifdef INTOX
char * intox_address = "127.0.0.1";
int intox_port = 12301;
#endif

etat_surprise_t mon_etat;

int main(int argc, char *argv[])
{
    char* mode = "auto";

    if (argc > 1) {
#ifdef INTOX
        intox_port = atoi(argv[1]);
#endif
    }
    if (argc > 2) {
        mode = argv[2];
    }

    if (strcmp(mode, "auto") == 0) {
        autopilot_init();
    }
    else if (strcmp(mode, "file") == 0) {
        pilot_init();
        copilot_init("circuit1.tsv");
    }
    else if (strcmp(mode, "manual") == 0) {
        input_detector_init();
        surprises_init(&mon_etat);
    }

    if (strcmp(mode, "manual") != 0) {
        while(access("go.txt", F_OK) != 0) {
            usleep(50000);
        }
    }

    if (strcmp(mode, "auto") == 0) {
        autopilot_run();
    }
    else if (strcmp(mode, "file") == 0) {
        copilot_run();
    } 
    else if (strcmp(mode, "manual") == 0) {
        input_detector_run();
    }

    return 0;
}