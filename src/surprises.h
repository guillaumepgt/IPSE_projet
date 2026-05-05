#ifndef SURPRISES_H
#define SURPRISES_H




typedef enum{
    AUCUN_EFFET,
    BONUS_BOOST,
    MALUS_TOUPIE
} type_effet_t;

typedef struct{
    type_effet_t effet_actif;
    int temps_restant;
} etat_surprise_t;




void surprises_init(etat_surprise_t *etat);
void surprises_detecter_et_appliquer(etat_surprise_t *etat);
void surprises_mise_a_jour_timer(etat_surprise_t *etat, int temps_ecoule);


#endif

