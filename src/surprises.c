#include "surprises.h"
#include "lumpiz.h"
#include "mrpiz.h"
#include "robot.h"
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

static const int DUREE_EFFETS[] = {0, 5000, 1000};
static int luminosite_sol_normal = 500; // Valeur par défaut simulée
static int seuil_diff_boost = 320;
static int seuil_diff_toupie = 730;
static bool sur_une_surprise = false;

// Variable magique pour contourner le bug du simulateur
static bool capteur_virtuel_en_panne = true;

void surprises_init(etat_surprise_t *etat) {
    (*etat).effet_actif = AUCUN_EFFET;
    (*etat).temps_restant = 0;

    printf("--- Test du capteur Lumpiz ---\n");

    if(lumpiz_luminosity_init() == -1) {
        printf("⚠️ ERREUR : Capteur Lumpiz introuvable sur ce robot.\n");
        printf("⚠️ Passage en mode SIMULATION de capteur pour terminer le TP !\n");
        capteur_virtuel_en_panne = true;
        return; // On ne bloque pas le programme !
    }

    // Si par miracle le capteur finit par marcher un jour :
    capteur_virtuel_en_panne = false;
    lumpiz_luminosity_light_set(LUMPIZ_LUMINOSITY_ON);

    // Boucle pour débloquer Webots
    for(int i = 0; i < 10; i++) {
        robot_obstacle_detected();
        usleep(50000);
    }

    luminosite_sol_normal = lumpiz_luminosity_get();

    if (luminosite_sol_normal == -1) {
        printf("⚠️ ALERTE : Le capteur renvoie -1. Passage en mode SIMULATION.\n");
        capteur_virtuel_en_panne = true;
        luminosite_sol_normal = 500;
        return;
    }

    printf("✅ Lumpiz Calibré ! Luminosité du sol: %d\n", luminosite_sol_normal);
}

void surprises_detecter_et_appliquer(etat_surprise_t *etat) {
    if ((*etat).effet_actif != AUCUN_EFFET) {
        return;
    }

    int luminosite = luminosite_sol_normal;

    // --- MODE SIMULATION DU CAPTEUR ---
    if (capteur_virtuel_en_panne) {
        // On crée une surprise aléatoire très brève de temps en temps (1% de chance par itération)
        int chance = rand() % 500;
        if (chance == 1) {
            luminosite = luminosite_sol_normal + 400; // Simule une case BOOST
            printf("\n🌟 [SIMULATION] Case BOOST détectée !\n");
        } else if (chance == 2) {
            luminosite = luminosite_sol_normal + 800; // Simule une case TOUPIE
            printf("\n🌪️ [SIMULATION] Case TOUPIE détectée !\n");
        }
    }
    // --- MODE NORMAL ---
    else {
        luminosite = lumpiz_luminosity_get();
        if (luminosite == -1) return; // Sécurité
    }

    int difference = luminosite - luminosite_sol_normal;

    if (sur_une_surprise == true) {
        if (luminosite <= luminosite_sol_normal + 50) {
            sur_une_surprise = false;
        } else {
            return;
        }
    }

    if (difference > seuil_diff_toupie) {
        (*etat).effet_actif = MALUS_TOUPIE;
        sur_une_surprise = true;
        mrpiz_led_rgb_set(MRPIZ_LED_RED);
    }
    else if (difference > seuil_diff_boost) {
        (*etat).effet_actif = BONUS_BOOST;
        sur_une_surprise = true;
        mrpiz_led_rgb_set(MRPIZ_LED_GREEN);
    }
    else {
        return;
    }

    (*etat).temps_restant = DUREE_EFFETS[(*etat).effet_actif];
}

void surprises_mise_a_jour_timer(etat_surprise_t *etat, int temps_ecoule_ms) {
    if((*etat).effet_actif == AUCUN_EFFET) {
        return;
    }

    (*etat).temps_restant -= temps_ecoule_ms;

    if((*etat).temps_restant <= 0) {
        printf("Fin de l'effet surprise.\n");
        (*etat).effet_actif = AUCUN_EFFET;
        (*etat).temps_restant = 0;
        mrpiz_led_rgb_set(MRPIZ_LED_OFF);
    }
}