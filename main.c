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
#include "autopilot.h"

/**
 * @brief Point d'entrée principal du programme.
 *
 * @return Toujours 0 (succès).
 */
int main()
{
    autopilot_init();
    autopilot_run();
}

