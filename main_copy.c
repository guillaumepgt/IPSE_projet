/**
 * @file main.c
 * @brief Programme principal du robot
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "pilot.h"
#include "move.h"
#include "mrpiz.h"
#include "robot_copy.h"
#include "surprises.h"
#include "copilot.h"

static int vitesse_boost= 100;
static int vitesse_classique=60;

int main() {
    printf("=== Démarrage du programme C ===\n");

    // ÉTAPE 1 : Lumpiz EN PREMIER, avant pilot_init
    printf("[Trace] Étape 1 : Initialisation des surprises (Lumpiz)...\n");
    etat_surprise_t mon_etat;
    surprises_init(&mon_etat);
    printf("[Trace] Surprises OK !\n");

    // ÉTAPE 2 : Pilot ensuite
    printf("[Trace] Étape 2 : Initialisation du Pilot (MRPiZ)...\n");
    pilot_init();
    printf("[Trace] Pilot OK !\n");

    printf("[Trace] Étape 3 : Lecture du fichier TSV...\n");
    int retour_init = copilot_init("e3e_mission_2.tsv");

    if (retour_init == -1) {
        printf("ERREUR CRITIQUE : Le fichier 'e3e_mission_2.tsv' est introuvable !\n");
    } else {
        printf("SUCCES : Fichier TSV ouvert.\n");
    }

    int nb_mouvements = 0;
    move_provider_update_nb_move(&nb_mouvements);
    printf("Nombre de mouvements à effectuer : %d\n", nb_mouvements);

    if (nb_mouvements > 0) {
        move_t *mon_chemin = malloc(nb_mouvements * sizeof(move_t));
        if (mon_chemin != NULL) {
            move_provider_update_all_moves(mon_chemin);

            for (int i = 0; i < nb_mouvements; i++) {
                printf(" -> Execution du mouvement %d...\n", i+1);
                pilot_start_move(mon_chemin[i]);

                while (!pilot_stop_at_target()) {
                    surprises_detecter_et_appliquer(&mon_etat);

                    if (mon_etat.effet_actif == BONUS_BOOST){
                        robot_set_speed(vitesse_boost);
                        surprises_mise_a_jour_timer(&mon_etat, 10);
                    }
                    else if(mon_etat.effet_actif == MALUS_TOUPIE){
                        robot_set_speed(vitesse_boost);
                        robot_spin();
                        mon_etat.effet_actif= AUCUN_EFFET;
                        mrpiz_led_rgb_set(MRPIZ_LED_OFF);

                        // ! IMPORTANT : relancer vers l'avant après la toupie !
                        robot_set_speed(vitesse_classique);
                        robot_start_forward();
                    }
                    else{
                        robot_set_speed(vitesse_classique);
                    }
                    usleep(10000);
                }
            }
            free(mon_chemin);
        }
    }

    robot_stop();
    printf("=== PARCOURS TERMINE ===\n");

    // 3. LA BOUCLE DE BLOCAGE (Empêche la fermeture de la fenêtre)
    printf("Programme en attente. Appuyez sur Ctrl+C pour quitter...\n");
    while(1) {
        usleep(100000);
    }

    return 0;
}