/**
 * @file autopilot.c
 * @brief Implémentation de l'autopilot.
 * @author Basile Morin
 *
 * Ce fichier contient les fonctions nécessaires à l'utilisation de l'autopilot.
 * 
 * L'autopilot est un module de pilotage autonome du robot. Il lit en continu
 * les capteurs de proximité du robot et décide automatiquement d'avancer, de
 * tourner à gauche ou de tourner à droite selon les obstacles détectés.
 */

#include "autopilot.h"
#include "pilot.h"
#include "move.h"
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief Seuil minimal pour considérer que l'avant du robot est dégagé.
 */
#define FRONT_THRESHOLD        120

/**
 * @brief Seuil minimal pour considérer que les capteurs avant-gauche
 * et avant-droite ne détectent pas d'obstacle proche.
 */
#define NEAR_SIDE_THRESHOLD    100

/**
 * @brief Seuil minimal pour considérer qu'un côté du robot est ouvert.
 */
#define SIDE_OPEN_THRESHOLD    100

/**
 * @brief Angle élémentaire de rotation utilisé par l'autopilot.
 */
#define TURN_STEP              5

/**
 * @brief Distance élémentaire d'avancement utilisée par l'autopilot.
 */
#define FORWARD_STEP           5

/**
 * @brief Durée de la pause entre chaque itération des verifications de l'atteinte de la cible, en microsecondes.
 */
#define USLEEP1ms 1000

/**
 * @brief Durée de la pause entre chaque itération de la boucle principale
 * de l'autopilot, en microsecondes.
 */
#define USLEEP50ms 50000


/**
 * @brief Vérifie si l'avant du robot est dégagé.
 *
 * @param proximityValues Structure contenant les valeurs des capteurs
 * de proximité du robot.
 *
 * @return true si la distance mesurée à l'avant est supérieure ou égale
 * au seuil défini, false sinon.
 */
static bool isFrontClear(proximity_t proximityValues)
{
    return proximityValues.front >= FRONT_THRESHOLD;
}

/**
 * @brief Vérifie si la zone avant-gauche proche du robot est dégagée.
 *
 * @param proximityValues Structure contenant les valeurs des capteurs
 * de proximité du robot.
 *
 * @return true si la zone avant-gauche est considérée comme libre,
 * false sinon.
 */
static bool isNearLeftClear(proximity_t proximityValues)
{
    return proximityValues.center_left >= NEAR_SIDE_THRESHOLD;
}

/**
 * @brief Vérifie si la zone avant-droite proche du robot est dégagée.
 *
 * @param proximityValues Structure contenant les valeurs des capteurs
 * de proximité du robot.
 *
 * @return true si la zone avant-droite est considérée comme libre,
 * false sinon.
 */
static bool isNearRightClear(proximity_t proximityValues)
{
    return proximityValues.center_right >= NEAR_SIDE_THRESHOLD;
}

/**
 * @brief Détermine si le robot peut avancer.
 *
 * Le robot peut avancer seulement si l'avant, l'avant-gauche et
 * l'avant-droite sont dégagés.
 *
 * @param proximityValues Structure contenant les valeurs des capteurs
 * de proximité du robot.
 *
 * @return true si le robot peut avancer, false sinon.
 */
static bool canGoForward(proximity_t proximityValues)
{

    return isFrontClear(proximityValues)
        && isNearLeftClear(proximityValues)
        && isNearRightClear(proximityValues);
}

/**
 * @brief Vérifie si le côté gauche du robot est ouvert.
 *
 * @param proximityValues Structure contenant les valeurs des capteurs
 * de proximité du robot.
 *
 * @return true si le côté gauche est considéré comme ouvert,
 * false sinon.
 */
static bool isLeftOpen(proximity_t proximityValues)
{
    return proximityValues.left >= SIDE_OPEN_THRESHOLD;
}

/**
 * @brief Vérifie si le côté droit du robot est ouvert.
 *
 * @param proximityValues Structure contenant les valeurs des capteurs
 * de proximité du robot.
 *
 * @return true si le côté droit est considéré comme ouvert,
 * false sinon.
 */
static bool isRightOpen(proximity_t proximityValues)
{
    return proximityValues.right >= SIDE_OPEN_THRESHOLD;
}

/**
 * @brief Fait avancer le robot d'une distance donnée.
 *
 * Crée un mouvement de type MOVE_FORWARD, lance ce mouvement via le module
 * pilot, puis attend que la distance cible soit atteinte.
 *
 * @param cm Distance d'avancement en centimètres.
 */
static void doForwardStep(int cm)
{
    move_t m;
    m.type = MOVE_FORWARD;
    m.magnitude = cm;

    pilot_start_move(m);
    while (!pilot_stop_at_target()) {
        usleep(USLEEP1ms);
    }
}

/**
 * @brief Fait tourner le robot vers la gauche.
 *
 * Crée un mouvement de type MOVE_TURN avec une magnitude négative afin
 * d'indiquer une rotation vers la gauche, puis attend que la rotation soit
 * terminée.
 *
 * @param mag Amplitude de la rotation.
 */
static void doLeftTurn(int mag)
{
    move_t m;
    m.type = MOVE_TURN;
    m.magnitude = (-1)*mag;

    pilot_start_move(m);
    while (!pilot_stop_at_target()) {
        usleep(USLEEP1ms);
    }
}

/**
 * @brief Fait tourner le robot vers la droite.
 *
 * Crée un mouvement de type MOVE_TURN avec une magnitude positive afin
 * d'indiquer une rotation vers la droite, puis attend que la rotation soit
 * terminée.
 *
 * @param mag Amplitude de la rotation.
 */
static void doRightTurn(int mag)
{
    move_t m;
    m.type = MOVE_TURN;
    m.magnitude = mag;

    pilot_start_move(m);
    while (!pilot_stop_at_target()) {
        usleep(USLEEP1ms);
    }
}


/**
 * @brief Initialise le pilotage automatique.
 *
 * Initialise le module pilot, puis initialise le générateur de nombres
 * aléatoires utilisé lorsque plusieurs choix de direction sont possibles.
 */
void autopilot_init(void)
{
    pilot_init();
    srand(time(NULL));
}

/**
 * @brief Exécute la boucle principale de l'autopilot.
 *
 * À chaque itération, le robot récupère les valeurs de ses capteurs de
 * proximité puis prend une décision :
 * - avancer si la voie est libre ;
 * - tourner à gauche si seul le côté gauche est ouvert ;
 * - tourner à droite si seul le côté droit est ouvert ;
 * - choisir aléatoirement une direction si les deux côtés sont ouverts ;
 * - choisir aléatoirement une direction si le robot est bloqué.
 *
 * Cette fonction ne se termine jamais car elle contient une boucle infinie.
 */
void autopilot_run(void){
    while (true){
        
        proximity_t proximityValues = robot_get_proximity();
        #if DEBUG
            printf("L=%d F=%d R=%d\n",proximityValues.center_left, proximityValues.front, proximityValues.center_right); 
        #endif

        if (canGoForward(proximityValues)) {
            doForwardStep(FORWARD_STEP);
            #if DEBUG
                printf("Decision: FORWARD\n");
            #endif
        }
        else {
            bool left_open = isLeftOpen(proximityValues);
            bool right_open = isRightOpen(proximityValues);
            #if DEBUG   
                printf("Can go forward: NO\n");     
                printf("Left Open?:%d Right Open?:%d\n",left_open,right_open);
            #endif

            if (left_open && right_open) {
                #if TURN_LEFT
                    int choice = DIR_GAUCHE; // Forcer à tourner à gauche en cas de blocage
                #else
                    int choice = rand() % 2; // Choix aléatoire en cas de blocage
                #endif

                #if DEBUG
                    printf("Decision: BOTH OPEN -> RANDOM\n");
                    printf("Random choice: %s\n", choice == DIR_GAUCHE ? "LEFT" : "RIGHT");
                #endif

                if (choice == DIR_GAUCHE) {
                    doLeftTurn(TURN_STEP);
                } else {
                    doRightTurn(TURN_STEP);
                }
            }
            else if (left_open) {
                doLeftTurn(TURN_STEP);
                #if DEBUG
                    printf("Decision: TURN LEFT\n");
                #endif
            }
            else if (right_open) {
                doRightTurn(TURN_STEP);
                #if DEBUG
                    printf("Decision: TURN RIGHT\n");
                #endif
            }
            else {

                #if TURN_LEFT
                    int choice = DIR_GAUCHE; // Forcer à tourner à gauche en cas de blocage
                #else
                    int choice = rand() % 2; // Choix aléatoire en cas de blocage
                #endif

                #if DEBUG
                    printf("Decision: BLOCKED -> RANDOM TURN\n");
                    printf("Random choice: %s\n", choice == DIR_GAUCHE ? "LEFT" : "RIGHT");
                #endif

                if (choice == DIR_GAUCHE) {
                    doLeftTurn(TURN_STEP);
                } else {
                    doRightTurn(TURN_STEP);
                }
            }
        }

        usleep(USLEEP50ms);
    }
}