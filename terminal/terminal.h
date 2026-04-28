/**
 * @file terminal.h
 * @brief Fonctions utilitaires pour modifier le mode d'entrée du terminal (non bloquant, sans écho).
 */

#ifndef TERMINAL_H
#define TERMINAL_H

/**
 * @brief Active le mode non bloquant du terminal.
 *
 * Cette fonction désactive le mode canonique (entrée caractère par caractère)
 * et, si `mode_echo` est à 0, désactive également l'affichage des caractères tapés.
 * Elle rend également l'entrée standard (stdin) non bloquante.
 *
 * @param mode_echo Si 1, l'affichage des touches est activé ; si 0, il est désactivé.
 */
void terminal_turn_non_blocking(int mode_echo);

/**
 * @brief Restaure le mode par défaut du terminal.
 *
 * Cette fonction remet le terminal dans son mode initial (canonique et bloquant).
 * À appeler avant de quitter le programme pour éviter de laisser le terminal dans un état incorrect.
 */
void terminal_restore_mode(void);

#endif // TERMINAL_H
