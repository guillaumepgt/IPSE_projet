/**
 * @file main.c
 * @brief Programme principal du robot
 *
 * Ce programme lit en boucle une commande clavier et traite la commande.
 */

 #include "input_detector.h"


/**
 * @brief Point d'entrée principal du programme.
 *
 * @return Toujours 0 (succès).
 */
int main()
{
    input_detector_init();
    input_detector_run();

    return 0;    
}
