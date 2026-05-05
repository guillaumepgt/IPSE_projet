#!/bin/bash

echo "=== Nettoyage des processus fantômes ==="
pkill -9 -f intox_mrpiz
pkill -9 -f "go"
sleep 1

# ==========================================
# CONFIGURATION DU ROBOT UNIQUE
# ==========================================
ROBOT_NAME="MRPiZ blue"
PORT=12301
# ==========================================

echo "=== Compilation avec votre Makefile ==="
make clean
make

if [ ! -f "go" ]; then
    echo "Erreur de compilation. Arrêt du script."
    exit 1
fi

echo "=== Démarrage du pont réseau pour '$ROBOT_NAME' (Port $PORT) ==="

# CORRECTION ICI : J'ai retiré le /$ROBOT_NAME à la fin de l'URL pour éviter le bug de l'espace
env WEBOTS_ROBOT_NAME="$ROBOT_NAME" WEBOTS_CONTROLLER_URL="tcp://host.docker.internal:1234/$ROBOT_NAME" ../webots_mrpiz_v0.6.1/controllers/intox_mrpiz_ir-x86_64-linux-gnu/intox_mrpiz_ir-x86_64-linux-gnu $PORT &

# On donne 2 secondes au port pour bien s'ouvrir
sleep 2

./go