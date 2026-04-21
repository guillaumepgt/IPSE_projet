#include "copilot.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILENAME "chemin.tsv"
static int nb_move;
static move_t* moves;


int init(char* name_file){  //FAIRE RESTE DU .C (F SUR LE TD) 
    FILE *file =fopen(FILENAME, "r");
    if(file==NULL){
        perror("Erreur ouverture fichier");
        return -1;
    }   

    if(fscanf(file,"%d\n",&nb_move)!=1){
        printf("erreur lecture nb_move");
        return -1;
    }

    moves = calloc(nb_move,sizeof(move_t));
    if(moves==NULL){
        printf("Erreur allocation memoire");
        return -1;
    }
    char line[100];
    int new_move_amplitude;
    for(int i=0; fscanf(file,"%s\t%d",line,&new_move_amplitude); i++){
        moves[i].magnitude=new_move_amplitude;
        if (strcmp(line,"forward")) moves[i].type=MOVE_FORWARD;
        if (strcmp(line,"turn")) moves[i].type=MOVE_TURN;
    }

}
