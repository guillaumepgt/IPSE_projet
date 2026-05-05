#include "copilot.h"
#include "pilot.h"
#include "robot_copy.h"
#include "surprises.h"
#include "mrpiz.h" // Pour éteindre la LED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static move_t *mouvements = NULL;
static int total_mouvements = 0;

int copilot_init(const char* name_file) {
    FILE *fichier = fopen(name_file, "r");
    if (fichier == NULL) { return -1; }
    if (fscanf(fichier, "%d", &total_mouvements) != 1) { fclose(fichier); return -1; }

    mouvements = calloc(total_mouvements, sizeof(move_t));
    if (mouvements == NULL) { fclose(fichier); return -1; }

    char direction[50];
    int valeur;
    for (int i = 0; i < total_mouvements; i++) {
        if (fscanf(fichier, "%s %d", direction, &valeur) == 2) {
            if (strstr(direction, "forward") != NULL) mouvements[i].type = MOVE_FORWARD;
            else if (strstr(direction, "turn") != NULL) mouvements[i].type = MOVE_TURN;
            mouvements[i].magnitude = valeur;
        }
    }
    fclose(fichier);
    return 1;
}

void move_provider_update_nb_move(int* nb) { if (nb != NULL) *nb = total_mouvements; }

void move_provider_update_all_moves(move_t* tab_move) {
    if (tab_move != NULL && mouvements != NULL) {
        for (int i = 0; i < total_mouvements; i++) { tab_move[i] = mouvements[i]; }
        free(mouvements);
        mouvements = NULL;
    }
}

// ==========================================
// LA FONCTION MAÎTRESSE DU PARCOURS FICHIER
// ==========================================
void copilot_run(void) {
    etat_surprise_t mon_etat;
    surprises_init(&mon_etat);

    int vitesse_boost = 100;
    int vitesse_classique = 60;

    int nb_mouvements = 0;
    move_provider_update_nb_move(&nb_mouvements);

    if (nb_mouvements > 0) {
        move_t *mon_chemin = malloc(nb_mouvements * sizeof(move_t));
        if (mon_chemin != NULL) {
            move_provider_update_all_moves(mon_chemin);

            for (int i = 0; i < nb_mouvements; i++) {
                pilot_start_move(mon_chemin[i]);

                while (!pilot_stop_at_target()) {
                    surprises_detecter_et_appliquer(&mon_etat);

                    if (mon_etat.effet_actif == BONUS_BOOST) {
                        robot_set_speed(vitesse_boost);
                        surprises_mise_a_jour_timer(&mon_etat, 10);
                    }
                    else if (mon_etat.effet_actif == MALUS_TOUPIE) {
                        // 1. On tourne
                        int offset = robot_spin();
                        // 2. On corrige l'objectif du pilote pour ne pas rater la cible
                        pilot_offset_target(offset);
                        // 3. On remet à zéro la surprise
                        mon_etat.effet_actif = AUCUN_EFFET;
                        mrpiz_led_rgb_set(MRPIZ_LED_OFF);
                        // 4. IMPORTANT : On relance les moteurs vers l'avant car la toupie les a coupés !
                        robot_set_speed(vitesse_classique);
                        robot_start_forward();
                    }
                    else {
                        robot_set_speed(vitesse_classique);
                    }
                    usleep(10000); // 10ms
                }
            }
            free(mon_chemin);
        }
    }
    robot_stop();
    while(1) { usleep(100000); }
}