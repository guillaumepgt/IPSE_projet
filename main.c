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
#include "copilot.h"

/**
 * @brief Point d'entrée principal du programme.
 *
 * @return Toujours 0 (succès).
 */
int main()
{
    pilot_init();
    move_t move;
    int retour_move;
    int nb;
    move_t* moves;
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


    move_provider_update_nb_move(&nb);
    moves = (move_t*) calloc(nb,sizeof(move_t));
    move_provider_update_all_moves(moves);

    for(int i=0;i<nb;i++){
        pilot_start_move(*(moves+i));
        while(!pilot_stop_at_target()){
            continue;
            usleep(1000);
        }
    }
    


    free(moves);




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

