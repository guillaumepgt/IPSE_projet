/**
 * @file remote.h
 * @brief Interface publique de la télécommande.
 *
 * Ce fichier contient les déclarations nécessaires à l'utilisation
 * des commandes simples pour un robot à partir d'une télécommande clavier.
 */

#ifndef _REMOTE_H_
#define _REMOTE_H_

#include <stdbool.h>

/**
 * @enum command_t
 * @brief Enumération des commandes disponibles via la télécommande.
 */
typedef enum {
    CMD_NONE = 0,       /**< Aucune commande reçue */
    CMD_AVANCER,        /**< Avancer */
    CMD_STOP,           /**< Arrêt */
    CMD_ROTATION_G,     /**< Tourner à gauche */
    CMD_ROTATION_D,     /**< Tourner à droite */
    CMD_AUG_VIT,        /**< Augmenter la vitesse> */
    CMD_RED_VIT,        /**< Réduire la vitesse> */
    CMD_NB              /**< Nombre total de commandes (non utilisée directement comme commande) */
} command_t;

void remote_set_obstacle_status(bool obs);
void remote_init(void);


/**
 * @brief Lit une touche sur l'entrée standard et retourne la commande correspondante.
 *
 * @return La commande de type `command_t` associée à la touche saisie.
 */
command_t remote_get_command(void);

#endif // _REMOTE_H_
