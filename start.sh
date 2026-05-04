#!/bin/bash

echo "=== Nettoyage ==="
pkill -9 -f "go"
rm -f go.txt

ROBOTSAuto=("MRPiZ yellow" "MRPiZ orange" "MRPiZ red" "MRPiZ purple" "MRPiZ magenta" "MRPiZ pink")
ROBOTSFile=("MRPiZ green" "MRPiZ sky-blue" "MRPiZ blue" "MRPiZ black" "MRPiZ grey")
ROBOTSManuel=("MRPiZ white")

PORT=12301

echo "=== COMPILATION ==="
make clean
make

if [ ! -f "go" ]; then
    echo "Erreur de compilation. Arrêt."
    exit 1
fi

echo "=== LANCEMENT DES IA (Webots gère déjà le réseau) ==="

# 1. On connecte les 6 robots AUTO
for robot in "${ROBOTSAuto[@]}"; do
    echo "Connexion de $robot au port $PORT (AUTO)"
    ./go $PORT auto &
    PORT=$((PORT + 1))
done

# 2. On connecte les 5 robots FILE
for robot in "${ROBOTSFile[@]}"; do
    echo "Connexion de $robot au port $PORT (FILE)"
    ./go $PORT file &
    PORT=$((PORT + 1))
done

sleep 0.5
touch go.txt
echo "GOOOOO !!!"

# 3. On connecte le robot MANUEL (au premier plan pour garder le clavier)
for robot in "${ROBOTSManuel[@]}"; do
    echo ">>> VOUS AVEZ LES COMMANDES SUR '$robot' (Port $PORT) <<<"
    ./go $PORT manual
    PORT=$((PORT + 1))
done

wait