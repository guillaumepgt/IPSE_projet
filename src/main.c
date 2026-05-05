/**
 * @file main.c
 * @brief Programme principal du robot
 *
 * Ce programme lit en boucle une commande clavier et traite la commande.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    }

    if (strcmp(mode, "auto") == 0) {
        while(access("go.txt", F_OK) != 0) {
            usleep(50000);
        }
        autopilot_run();
    }
    else if (strcmp(mode, "file") == 0) {
        while(access("go.txt", F_OK) != 0) {
            usleep(50000);
        }
        int nb_mouvements = 0;
        move_provider_update_nb_move(&nb_mouvements);

        if (nb_mouvements > 0) {
            move_t *mon_chemin = malloc(nb_mouvements * sizeof(move_t));
            if (mon_chemin != NULL) {
                move_provider_update_all_moves(mon_chemin);

                for (int i = 0; i < nb_mouvements; i++) {
                    pilot_start_move(mon_chemin[i]);

                    while (!pilot_stop_at_target()) {
                        usleep(10000);
                    }
                }
                free(mon_chemin);
            }
        }
    } else if (strcmp(mode, "manual") == 0) {
        char *start[] = {"3...", "2...", "1...", "GO !!!"};
        for (int i = 0; i < 4; i++) {
            printf("%s\n", start[i]);
            fflush(stdout);
            usleep(1000000);
        }
        FILE *fichier = fopen("go.txt", "w");
        if (fichier != NULL) {
            fclose(fichier);
        }
        input_detector_run();
    }
}

