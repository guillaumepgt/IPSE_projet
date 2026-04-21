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
    move_t move={MOVE_FORWARD,100};
    pilot_start_move(move);
    while(!pilot_stop_at_target()) usleep(100);
}

