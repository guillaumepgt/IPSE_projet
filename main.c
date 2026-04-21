/**
 * @file main.c
 * @brief Programme principal du robot
 *
 * Ce programme lit en boucle une commande clavier et traite la commande.
 */

#include <stdio.h>
#include <stdlib.h>

#include "move.h"
#include "pilot.h"
#include "copilot.h"
#include <unistd.h>

/**
 * @brief Point d'entrée principal du programme.
 *
 * @return Toujours 0 (succès).
 */

int main()
{
    pilot_init();
    copilot_init("e3e_mission_2.tsv");

    int nb_mouvements = 0;
    move_provider_update_nb_move(&nb_mouvements);
    if (nb_mouvements > 0) {
        move_t *mon_chemin = malloc(nb_mouvements * sizeof(move_t));
        if (mon_chemin != NULL) {
            move_provider_update_all_moves(mon_chemin);
            for (int i = 0; i < nb_mouvements; i++) {
                pilot_start_move(mon_chemin[i]);
            }
            free(mon_chemin);
        }
    }
    while (1) {}
}