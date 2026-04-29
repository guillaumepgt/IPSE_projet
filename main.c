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
#include "copilot.h"
#include "pilot.h"
#include "move.h"
#include "input_detector.h"

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

    input_detector_init();
    // autopilot_init();
    // pilot_init();
    // copilot_init("e3e_mission_2.tsv");

    while(access("go.txt", F_OK) != 0) {
        usleep(50000);
    }

    input_detector_run();
    // autopilot_run();

    // int nb_mouvements = 0;
    // move_provider_update_nb_move(&nb_mouvements);
    //
    // if (nb_mouvements > 0) {
    //     move_t *mon_chemin = malloc(nb_mouvements * sizeof(move_t));
    //     if (mon_chemin != NULL) {
    //         move_provider_update_all_moves(mon_chemin);
    //
    //         for (int i = 0; i < nb_mouvements; i++) {
    //             pilot_start_move(mon_chemin[i]);
    //
    //             while (!pilot_stop_at_target()) {
    //                 usleep(10000);
    //             }
    //         }
    //         free(mon_chemin);
    //     }
    // }
    // robot_stop();
    // while (1) {
    //     usleep(100000);
    // }
}

