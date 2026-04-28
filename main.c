/**
 * @file main.c
 * @brief Programme principal du robot
 *
 * Ce programme lit en boucle une commande clavier et traite la commande.
 */

#include <stdbool.h>
#include <stdio.h>
#include "remote.h"
#include "robot.h"


/**
 * @brief Point d'entrée principal du programme.
 *
 * @return Toujours 0 (succès).
 */
int main()
{
    command_t my_command;
    bool obstacle_ou_pas;
    remote_init();
    robot_init();
    

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
            printf("Tourner à gauche\n");
            robot_turn(DIR_GAUCHE);
            break;

        case CMD_ROTATION_D:
            printf("Tourner à droite\n");
            robot_turn(DIR_DROITE);
            break; 
        case CMD_AUG_VIT:
            printf("Augmenter vitesse\n");
            robot_set_speed_pct(robot_get_speed_pct()+10);
            break; 
        case CMD_RED_VIT:
            robot_set_speed_pct(robot_get_speed_pct ()-10);
            printf("Réduire vitesse\n");
            break; 
        default:
            break;
        }
    }
}
