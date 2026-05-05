/**
 * @file main.c
 * @brief Programme principal du robot
 *
 * Ce programme lit en boucle une commande clavier et traite la commande.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "pilot.h"
#include "move.h"
#include "mrpiz.h"
#include"robot_copy.h"
#include "surprises.h"
#include "copilot.h"

/**
 * @brief Point d'entrée principal du programme.
 *
 * @return Toujours 0 (succès).
 */

static int vitesse_boost= 100;
static int vitesse_classique=60;

int main()
{
    pilot_init();
    etat_surprise_t mon_etat;
    surprises_init(&mon_etat);
    
    move_t move;
    int retour_move;
    int nb;
    move_t* moves;

    copilot_init("e3e_mission_2.tsv");

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
                    if (mon_etat.effet_actif == BONUS_BOOST){
                        robot_set_speed(vitesse_boost);
                        surprises_mise_a_jour_timer(&mon_etat, 10);
                    }

                    else if(mon_etat.effet_actif == MALUS_TOUPIE){
                        robot_set_speed(vitesse_boost);
                        robot_spin();
                        mon_etat.effet_actif= AUCUN_EFFET;
                        mrpiz_led_rgb_set(MRPIZ_LED_OFF);
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
    // move_t scenario[5];
    // scenario[0].type=MOVE_FORWARD;
    // scenario[0].magnitude=65;
    // scenario[1].type=MOVE_TURN;
    // scenario[1].magnitude=90;
    // scenario[2].type=MOVE_FORWARD;
    // scenario[2].magnitude=35;
    // scenario[3].type=MOVE_TURN;
    // scenario[3].magnitude=270;
    // scenario[4].type=MOVE_FORWARD;
    // scenario[4].magnitude=45;

    // for(int i =0;i<5;i++){
    //     pilot_start_move(scenario[i]);
    //     while(!pilot_stop_at_target())
    //     {
    //         continue;
    //     }
    // }


    //move_provider_update_nb_move(&nb);
    //moves = (move_t*) calloc(nb,sizeof(move_t));
    //move_provider_update_all_moves(moves);

    // for(int i=0;i<nb;i++){
    //     pilot_start_move(*(moves+i));
    //     while(!pilot_stop_at_target()){
    //         surprises_detecter_et_appliquer(&mon_etat);
    //                     printf("ça matche xwoula");

    //         if (mon_etat.effet_actif == BONUS_BOOST){
    //             robot_set_speed(20);
    //             // robot_set_inversion(false);
    //             printf("Bonus Activé");
    //             surprises_mise_a_jour_timer(&mon_etat, 10);

    //         }


    //         else{
    //             robot_set_speed(60);
    //             robot_set_inversion(false);
    //         }


    //         usleep(1000);
    //     }
    // }
    


    // free(moves);




    // retour_move=move_provider_update_next_move(&move);
    // if(retour_move==1){
    //     pilot_start_move(move);
    //     move_provider_update_all_moves(moves);
    //     while(!pilot_stop_at_target()){
    //         continue;
    //         usleep(1000);
    //     }
    // }
    // else{
    //     break;
    // }
 






}

