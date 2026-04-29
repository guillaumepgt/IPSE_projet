/**
 * @file terminal.c
 * @brief Implémentation des fonctions de gestion du terminal (mode non bloquant, écho).
 */

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "terminal.h"

/// Structure utilisée pour stocker les réglages originaux du terminal.
static struct termios orig_termios;

void terminal_turn_non_blocking(int mode_echo) {
    struct termios new_termios;

    // Sauvegarde des réglages actuels
    tcgetattr(STDIN_FILENO, &orig_termios);
    new_termios = orig_termios;

    // Désactivation du mode canonique (lecture caractère par caractère)
    new_termios.c_lflag &= ~(ICANON);

    // Activation ou non de l'affichage des touches
    if (!mode_echo) {
        new_termios.c_lflag &= ~(ECHO);
    }

    // Application immédiate des nouveaux réglages
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

    // Passage de stdin en mode non bloquant
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}

void terminal_restore_mode(void) {
    // Restauration des paramètres initiaux du terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);

    // Rendre stdin à nouveau bloquant
    fcntl(STDIN_FILENO, F_SETFL, 0);
}
