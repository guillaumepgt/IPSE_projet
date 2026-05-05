/**
 * @file input_detector.c
 * @brief  Détecteur d'entrée.
 * @author Basile Morin
 * 
 * Ce fichier contient les fonctions nécessaires à l'utilisation du détecteur d'entrée.
 *
 * Le détecteur d'entrée fait le lien entre les commandes clavier récupérées
 * par le module remote et les actions envoyées au robot via les modules
 * robot et pilot.
 */
#include "remote.h"
#include "robot.h"
#include "pilot.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

/** @brief Incrémentation de la vitesse de rotation vers la gauche. */
#define TURN_LEFT_INC -10

/** @brief Incrémentation de la vitesse de rotation vers la droite. */
#define TURN_RIGHT_INC 10

/** @brief Incrémentation de la vitesse d'avancement. */
#define AUGMENTATION_VITESSE 10

/** @brief Réduction de la vitesse d'avancement. */
#define REDUCTION_VITESSE -10

/** @brief Durée de la pause pendant le démarrage, en microsecondes. */
#define USLEEP1s 1000000

/** @brief Durée de la pause dans la boucle principale, en microsecondes. */
#define USLEEP50ms 50000

/** @brief Durée de la pause pendant l'attente de la fin d'un mouvement, en microsecondes. */
#define USLEEP1ms 1000




/**
 * @brief Initialise le détecteur d'entrée.
 *
 * Cette fonction doit être appelée avant toute utilisation du détecteur d'entrée.
 *
 * Elle initialise :
 * - le module remote, responsable de la lecture des commandes clavier ;
 * - le module pilot, responsable des déplacements contrôlés du robot.
 */
void input_detector_init(void){
    remote_init();
    pilot_init();
}


/**
 * @brief Fonction principale du détecteur d'entrée.
 *
 * Cette fonction est infinie. Elle lit en boucle les commandes du clavier
 * et traite les commandes.
 *
 * À chaque itération, la fonction :
 * - récupère la commande entrée par l'utilisateur ;
 * - vérifie si un obstacle est détecté devant le robot ;
 * - bloque l'avancement si un obstacle est présent ;
 * - exécute l'action correspondant à la commande reçue.
 *
 * @return Toujours 0 (succès).
 */
int input_detector_run(void){

    char *start[] = {"3...", "2...", "1...", "GO !!!"};
    for (int i = 0; i < 4; i++) {
        printf("%s\n", start[i]);
        fflush(stdout);
        usleep(USLEEP1s);
    }
    FILE *fichier = fopen("go.txt", "w");
    if (fichier != NULL) {
        fclose(fichier);
    }


    /**
     * @brief Commande actuellement lue depuis le clavier.
     */
    command_t my_command;

    /**
     * @brief Indique si un obstacle est détecté ou non devant le robot.
     */
    bool obstacle_ou_pas;

    /**
     * @brief Structure utilisée pour transmettre un mouvement au module pilot.
     *
     * Elle est utilisée principalement pour les rotations gauche et droite.
     */
    move_t move;

    while(1)
    {
        //recupération de la commande
        my_command = remote_get_command();

        // Vérification de la présence d'un obstacle devant le robot
        obstacle_ou_pas = robot_obstacle_detected();

        // Petite pause pour limiter la fréquence de lecture et de traitement
        usleep(USLEEP50ms);

        /**
         * Si un obstacle est détecté, le robot est immédiatement arrêté.
         * Si la commande demandée est d'avancer, elle est ignorée afin
         * d'éviter une collision.
         */
        if (obstacle_ou_pas){
            robot_stop();
            if(my_command==CMD_AVANCER) my_command=CMD_NONE;
        }

        //traitement de la commande
        switch (my_command)
        {
        /**
         * Aucune commande valide reçue.
         * Le robot conserve son état courant.
         */
        case CMD_NONE:
            break;
        
        /**
         * Commande d'avancement.
         * Le robot avance tout droit à la vitesse actuelle.
         */
        case CMD_AVANCER:
            printf("Avancer\n");
            robot_start_forward();
            break;
        
        /**
         * Commande d'arrêt.
         * Les moteurs du robot sont arrêtés.
         */
        case CMD_STOP:
            printf("STOP\n");
            robot_stop();
            break;

        /**
         * Commande de rotation à gauche.
         * Un mouvement de rotation négatif est envoyé au module pilot.
         */
        case CMD_ROTATION_G:
            move.type = MOVE_TURN;
            move.magnitude = TURN_LEFT_INC;
            printf("Tourner à gauche\n");
            pilot_start_move(move);
            while (!pilot_stop_at_target()) {
                usleep(USLEEP1ms);
            }
            break;

        /**
         * Commande de rotation à droite.
         * Un mouvement de rotation positif est envoyé au module pilot.
         */
        case CMD_ROTATION_D:
            move.type = MOVE_TURN;
            move.magnitude = TURN_RIGHT_INC;
            printf("Tourner à droite\n");
            pilot_start_move(move);
            while (!pilot_stop_at_target()) {
                usleep(USLEEP1ms);
            }
            break; 

        /**
         * Commande d'augmentation de vitesse.
         * La vitesse actuelle du robot est augmentée de 10%.
         */
        case CMD_AUG_VIT:
            robot_set_speed_pct(robot_get_speed_pct()+AUGMENTATION_VITESSE);
            printf("Augmenter vitesse -> %d\n", robot_get_speed_pct());
            break; 

        /**
         * Commande de réduction de vitesse.
         * La vitesse actuelle du robot est réduite de 10%.
         */
        case CMD_RED_VIT:
            robot_set_speed_pct(robot_get_speed_pct ()+REDUCTION_VITESSE);
            printf("Réduire vitesse -> %d\n", robot_get_speed_pct());
            break; 

        /**
         * Cas de sécurité si une commande inconnue est reçue.
         */
        default:
            break;
        }
    }
    return 0;
}