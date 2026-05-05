#ifndef AUTOPILOT_H
#define AUTOPILOT_H
/**
 * @file autopilot.h
 * @brief Interface du module de pilotage automatique du robot.
 *
 * Ce module permet d'initialiser et de lancer un comportement autonome
 * basé sur les capteurs de proximité du robot.
 */


#define DEBUG 0

/**
 * @brief Initialise le module autopilot.
 *
 * Initialise les modules nécessaires au déplacement autonome, notamment
 * le module pilot, puis initialise le générateur de nombres aléatoires.
 * Il n'est pas nécessaire d'initialiser d'autres modules avant d'appeler cette fonction.
 */
void autopilot_init(void);

/**
 * @brief Lance la boucle principale du pilotage automatique.
 *
 * Le robot lit en continu ses capteurs de proximité et décide automatiquement
 * d'avancer, de tourner à gauche ou de tourner à droite selon les obstacles
 * détectés.
 *
 * Cette fonction contient une boucle infinie.
 */
void autopilot_run(void);

#endif