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
 */
void input_detector_init(void);

/**
 * @brief Fonction principale du détecteur d'entrée.
 * Cette fonction est infinie. Elle lit en boucle les commandes du clavier et traite les commandes.
 * @return Toujours 0 (succès).
 */
int input_detector_run(void);

#endif // _INPUT_DETECTOR_H_