/**
 * @file input_detector.c
 * @brief  Détecteur d'entrée.
 * @author Basile Morin
 * 
 * Ce fichier contient les fonctions nécessaires à l'utilisation du détecteur d'entrée
 */
#include "remote.h"
#include "robot.h"
#include "pilot.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>



/**
 * @brief Initialise le détecteur d'entrée.
 *
 * Cette fonction doit être appelée avant toute utilisation du détecteur d'entrée.
 */
void input_detector_init(void){
    remote_init();
    pilot_init();
}


/**
 * @brief Fonction principale du détecteur d'entrée.
 * Cette fonction est infinie. Elle lit en boucle les commandes du clavier et traite les commandes.
 * @return Toujours 0 (succès).
 */
int input_detector_run(void){
    command_t my_command;
    bool obstacle_ou_pas;
    move_t move;

    while(1)
    {
        //recupération de la commande
        my_command = remote_get_command();
        obstacle_ou_pas = robot_obstacle_detected();
        usleep(50000);

        if (obstacle_ou_pas){
            robot_stop();
            if(my_command==CMD_AVANCER) my_command=CMD_NONE;
        }
        //traitement de la commande
        switch (my_command)
        {
        case CMD_NONE:
            break;
        
        case CMD_AVANCER:
            printf("Avancer\n");
            robot_start_forward();
            break;
        
        case CMD_STOP:
            printf("STOP\n");
            robot_stop();
            break;

        case CMD_ROTATION_G:
            move.type = MOVE_TURN;
            move.magnitude = -10;
            printf("Tourner à gauche\n");
            pilot_start_move(move);
            while (!pilot_stop_at_target()) {
                usleep(1000);
            }
            break;

        case CMD_ROTATION_D:
            move.type = MOVE_TURN;
            move.magnitude = 10;
            printf("Tourner à droite\n");
            pilot_start_move(move);
            while (!pilot_stop_at_target()) {
                usleep(1000);
            }
            break; 
        case CMD_AUG_VIT:
            robot_set_speed_pct(robot_get_speed_pct()+10);
            printf("Augmenter vitesse -> %d\n", robot_get_speed_pct());
            break; 
        case CMD_RED_VIT:
            robot_set_speed_pct(robot_get_speed_pct ()-10);
            printf("Réduire vitesse -> %d\n", robot_get_speed_pct());
            break; 
        default:
            break;
        }
    }
    return 0;
}
