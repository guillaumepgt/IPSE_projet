/**
 * @file input_detector.h
 * @brief Interface publique du détecteur d'entrée.
 * @author Basile Morin
 * 
 * Ce fichier contient les déclarations nécessaires à l'utilisation
 * du détecteur d'entrée
 */
#ifndef _INPUT_DETECTOR_H_
#define _INPUT_DETECTOR_H_

/**
 * @brief Initialise le détecteur d'entrée.
 *
 * Cette fonction doit être appelée avant toute utilisation du détecteur d'entrée.
 *
 * Elle initialise les modules nécessaires à la lecture des commandes
 * utilisateur et au pilotage du robot.
 * Il n'est pas nécessaire d'initialiser d'autres modules avant d'appeler cette fonction.
 */
void input_detector_init(void);

/**
 * @brief Fonction principale du détecteur d'entrée.
 *
 * Cette fonction est infinie. Elle lit en boucle les commandes du clavier,
 * vérifie la présence éventuelle d'un obstacle, puis déclenche l'action
 * correspondante sur le robot.
 *
 * Les commandes possibles permettent notamment :
 * - d'avancer ;
 * - d'arrêter le robot ;
 * - de tourner à gauche ;
 * - de tourner à droite ;
 * - d'augmenter la vitesse ;
 * - de réduire la vitesse.
 *
 * @return Toujours 0 (succès), même si en pratique la fonction ne se termine pas.
 */
int input_detector_run(void);

#endif // _INPUT_DETECTOR_H_