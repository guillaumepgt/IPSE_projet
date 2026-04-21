#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "copilot.h"
#include "move.h"

static move_t *mouvements = NULL;
static int total_mouvements = 0;

extern int copilot_init(const char* name_file) {
    FILE *fichier = fopen(name_file, "r");
    fscanf(fichier, "%d", &total_mouvements);
    mouvements = malloc(total_mouvements * sizeof(move_t));

    char direction[20];
    int valeur;
    for (int i = 0; i < total_mouvements; i++) {
        fscanf(fichier, "%s %d", direction, &valeur);
        if (strcmp(direction, "forward") == 0) {
            mouvements[i].type = MOVE_FORWARD;
        } else if (strcmp(direction, "turn") == 0) {
            mouvements[i].type = MOVE_TURN;
        }
        mouvements[i].magnitude = valeur;
    }

    fclose(fichier);
    return 1;
}

extern void move_provider_update_nb_move(int* nb) {
    if (nb != NULL) {
        *nb = total_mouvements;
    }
}

extern void move_provider_update_all_moves(move_t* tab_move) {
    if (tab_move != NULL && mouvements != NULL) {
        for (int i = 0; i < total_mouvements; i++) {
            tab_move[i] = mouvements[i];
        }
        free(mouvements);
        mouvements = NULL;
    }
}