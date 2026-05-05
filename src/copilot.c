#include "copilot.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static move_t *mouvements = NULL;
static int total_mouvements = 0;

extern int copilot_init(const char* name_file) {
    FILE *fichier = fopen(name_file, "r");
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier %s\n", name_file);
        return -1;
    }

    if (fscanf(fichier, "%d", &total_mouvements) != 1) {
        printf("Erreur de lecture du nombre de mouvements\n");
        fclose(fichier);
        return -1;
    }

    mouvements = calloc(total_mouvements, sizeof(move_t));
    if (mouvements == NULL) {
        printf("Erreur d'allocation memoire\n");
        fclose(fichier);
        return -1;
    }

    char direction[50];
    int valeur;
    for (int i = 0; i < total_mouvements; i++) {
        if (fscanf(fichier, "%s %d", direction, &valeur) == 2) {

            if (strstr(direction, "forward") != NULL) {
                mouvements[i].type = MOVE_FORWARD;
            } else if (strstr(direction, "turn") != NULL) {
                mouvements[i].type = MOVE_TURN;
            }
            mouvements[i].magnitude = valeur;
        }
    }

    fclose(fichier);
    return 1;
}

void copilot_run(void) {
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
    robot_stop();
    while(1) { usleep(100000); }
}

void move_provider_update_nb_move(int* nb) {
    if (nb != NULL) {
        *nb = total_mouvements;
    }
}

void move_provider_update_all_moves(move_t* tab_move) {
    if (tab_move != NULL && mouvements != NULL) {
        for (int i = 0; i < total_mouvements; i++) {
            tab_move[i] = mouvements[i];
        }
        free(mouvements);
        mouvements = NULL;
    }
}