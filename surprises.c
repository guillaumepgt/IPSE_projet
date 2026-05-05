#include "surprises.h"
#include "lumpiz.h"
#include "mrpiz.h"
#include <stdio.h>
#include <unistd.h>

//=============================

static const int DUREE_EFFETS[]={0, 5000, 1000};
static int luminosite_sol_normal = 0;

static int seuil_diff_boost= 320;
static int seuil_diff_toupie= 730;

static int delai_attente_micros= 50000;

static bool sur_une_surprise=false;


//=============================
void surprises_init(etat_surprise_t *etat){
    (*etat).effet_actif = AUCUN_EFFET;
    (*etat).temps_restant = 0;


    lumpiz_luminosity_init();
    if(lumpiz_luminosity_init() ==-1){
        printf("ERREUR\n");
    }
    lumpiz_luminosity_light_set(LUMPIZ_LUMINOSITY_ON);

    usleep(delai_attente_micros);


    luminosite_sol_normal=lumpiz_luminosity_get();
    printf("Lumpiz Calibré ! Luminosité du sol: %d\n", luminosite_sol_normal);

    char const*err=lumpiz_error_msg();
    lumpiz_error_print(err);
}


void surprises_detecter_et_appliquer(etat_surprise_t *etat){
    if ((*etat).effet_actif != AUCUN_EFFET){
        return;
    }

    int luminosite=lumpiz_luminosity_get();
    printf("Luminosté: %d\n", luminosite);
    
    if(luminosite ==-1){
        printf("ERREUR\n");
        return;
    }
    
    int difference=luminosite-luminosite_sol_normal;

    if(sur_une_surprise==true){
        if(luminosite==luminosite_sol_normal){
            sur_une_surprise=false;
        }else{
            return;
        }
    }

    if (difference < seuil_diff_boost) {
        (*etat).effet_actif = BONUS_BOOST;
        sur_une_surprise=true;
        mrpiz_led_rgb_set(MRPIZ_LED_GREEN);
    }
    else if (difference > seuil_diff_toupie) {
        (*etat).effet_actif = MALUS_TOUPIE;
        sur_une_surprise=true;
        mrpiz_led_rgb_set(MRPIZ_LED_RED);
    }

    else {
        return; 
    }

    (*etat).temps_restant=DUREE_EFFETS[(*etat).effet_actif];
    printf("Surpise n°%d activée pour %d ms (Luminosité: %d)\n",(*etat).effet_actif,(*etat).temps_restant,luminosite);

}


void surprises_mise_a_jour_timer(etat_surprise_t *etat, int temps_ecoule_ms){
    if((*etat).effet_actif == AUCUN_EFFET){
        return;
    }

    (*etat).temps_restant -= temps_ecoule_ms;

    if((*etat).temps_restant <=0){
        printf("Fin de l'effet\n");
        (*etat).effet_actif = AUCUN_EFFET;
        (*etat).temps_restant = 0;
        mrpiz_led_rgb_set(MRPIZ_LED_OFF);
    }
}


