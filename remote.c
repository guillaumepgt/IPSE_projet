/**
 * @file remote.c
 * @brief Implémentation de la lecture de commande depuis le clavier.
 *
 * Ce module convertit les touches pressées en commandes compréhensibles
 * par le robot.
 */

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "terminal/terminal.h"
#include "remote.h"

bool obstacle;


/**
 * @brief Tableau de correspondance entre touches clavier et commandes.
 *
 * L'index dans le tableau correspond à la valeur de l'enum `command_t`.
 */
static char keyboard_config[CMD_NB];

/**
 * @brief Convertit un caractère en commande.
 *
 * Compare la touche à chaque élément de `keyboard_config` pour déterminer
 * la commande correspondante.
 *
 * @param ascii Caractère lu (par exemple 'z', 'q', etc.)
 * @return Commande correspondante ou `CMD_NONE` si non reconnue.
 */
static command_t ascii_to_command(int ascii);


/**
 * @brief Initialise le module de lecture de commandes.
 *
 * Configure les touches par défaut ou permet à l'utilisateur de les personnaliser.
 */
void remote_init(){
    bool continuer = true;
    char c;
    printf("Voulez vous régler les touches (0 : touches classiques  1 : régler touches)\n");
    while (continuer) {
        c = getchar();
        if (c <= '1' && c >=0) continuer=false;
        else printf("Veuillez saisir un chiffre valide");
    }
    
    if (c=='1'){
        printf("\n--- CONFIGURATION DES TOUCHES ---\n");

        printf("Avancer : ");
        scanf(" %c", &keyboard_config[CMD_AVANCER]);

        printf("Stop / Reculer : ");
        scanf(" %c", &keyboard_config[CMD_STOP]);

        printf("Tourner a gauche : ");
        scanf(" %c", &keyboard_config[CMD_ROTATION_D]);

        printf("Tourner a droite : ");
        scanf(" %c", &keyboard_config[CMD_ROTATION_G]);

        printf("Augmenter la vitesse : ");
        scanf(" %c", &keyboard_config[CMD_AUG_VIT]);

        printf("Reduire la vitesse : ");
        scanf(" %c", &keyboard_config[CMD_RED_VIT]);
        printf("--> Touches configurees avec succes !\n\n");
    } else {
    keyboard_config[CMD_NONE]='\0';//CMD_NONE
    keyboard_config[1]=    'z';  //CMD_AVANCER
    keyboard_config[2]=    's';  //CMD_STOP
    keyboard_config[3]=    'q';  //CMD_ROTATION_G
    keyboard_config[4]=    'd';  //CMD_ROTATION_D
    keyboard_config[5]=    'r';  //CMD_AUG_VIT
    keyboard_config[6]=    'f';  //CMD_RED_VIT
    printf("Touches par defaut :\nAvancer : z\nStop/Reculer : s\nTourner a gauche : q\nTourner a droite : d\nAugmenter la vitesse : r\nReduire la vitesse : f\n");
    }
}

/**
 * @brief Lit une touche au clavier et retourne la commande associée.
 *
 * Utilise `getchar()` pour lire l'entrée standard et convertit le caractère
 * saisi en commande via la fonction interne `ascii_to_command`.
 *
 * @return Commande reconnue de type `command_t`.
 */
command_t remote_get_command(void)
{
    // int c = getchar();
    usleep(100); 
    terminal_turn_non_blocking(1);
    int c= getchar();
    return ascii_to_command(c);

}

/**
 * @brief Convertit un caractère en commande.
 *
 * Compare la touche à chaque élément de `keyboard_config` pour déterminer
 * la commande correspondante.
 *
 * @param ascii Caractère lu (par exemple 'z', 'q', etc.)
 * @return Commande correspondante ou `CMD_NONE` si non reconnue.
 */
static command_t ascii_to_command(int ascii){
    command_t temp = CMD_NONE;

    for(int i=0;i<CMD_NB;i++){
        if (keyboard_config[i]==ascii) {
            temp = (command_t) i;
            break;
        }
        
    }
    if((obstacle) && (temp=CMD_AVANCER)) temp=CMD_STOP;
    
    return temp;
}


void remote_set_obstacle_status(bool obs){
    obstacle=obs;
}